package csdk

// #cgo darwin,arm64 LDFLAGS: -L${SRCDIR}/libs/darwin -lbcos-c-sdk-arm64
// #cgo darwin,amd64 LDFLAGS: -L${SRCDIR}/libs/darwin -lbcos-c-sdk-x86_64
// #cgo linux,amd64 LDFLAGS: -L${SRCDIR}/libs/linux -lbcos-c-sdk
// #cgo windows,amd64 LDFLAGS: -L${SRCDIR}/libs/win -lbcos-c-sdk
// #cgo CFLAGS: -I./
// #include "include/bcos_sdk_c.h"
// #include "include/bcos_sdk_c_error.h"
// #include "include/bcos_sdk_c_rpc.h"
// #include "include/bcos_sdk_c_uti_tx.h"
// #include <stdlib.h>
// void on_recv_resp_callback(struct bcos_sdk_c_struct_response *);
import "C"

import (
	"encoding/hex"
	"fmt"
	"time"
	"unsafe"

	"github.com/sirupsen/logrus"
)

type SDK struct {
	c          unsafe.Pointer
	SMCrypto   bool
	WASM       bool
	ChainID    *C.char
	GroupID    *C.char
	PrivateKey unsafe.Pointer
}

//export on_recv_resp_callback
func on_recv_resp_callback(resp *C.struct_bcos_sdk_c_struct_response) {
	if int(resp.error) != 0 {
		logrus.Errorf("\t something is wrong, error: %d, errorMessage: %s\n", resp.error, resp.desc)
	} else {
		logrus.Infof(" \t recv rpc resp from server ==>>>> resp: %s\n", C.GoString((*C.char)(resp.data)))
	}
}

func NewSDK(config *C.struct_bcos_sdk_c_config, groupID string, privateKey []byte) *SDK {
	defer C.free(unsafe.Pointer(config))
	sdk := C.bcos_sdk_create(config)
	if sdk == nil {
		message := C.bcos_sdk_get_last_error_msg()
		defer C.free(unsafe.Pointer(message))
		logrus.Errorf("bcos_sdk_create failed with error: %s", C.GoString(message))
		return nil
	}
	C.bcos_sdk_start(sdk)
	var wasm, smCrypto C.int
	group := C.CString(groupID)
	C.bcos_sdk_get_group_wasm_and_crypto(sdk, group, &wasm, &smCrypto)
	chainID := C.bcos_sdk_get_group_chain_id(sdk, group)
	return &SDK{
		c:        sdk,
		SMCrypto: smCrypto != 0,
		WASM:     wasm != 0,
		GroupID:  group,
		ChainID:  chainID,
	}
}

func NewSDKByConfigFile(configFile string, groupID string, privateKey []byte) *SDK {
	config := C.CString(configFile)
	sdk := C.bcos_sdk_create_by_config_file(config)
	if sdk == nil {
		message := C.bcos_sdk_get_last_error_msg()
		defer C.free(unsafe.Pointer(message))
		logrus.Errorf("bcos_sdk_create failed with error: %s", C.GoString(message))
		return nil
	}
	C.bcos_sdk_start(sdk)
	var wasm, smCrypto C.int
	group := C.CString(groupID)
	C.bcos_sdk_get_group_wasm_and_crypto(sdk, group, &wasm, &smCrypto)
	chainID := C.bcos_sdk_get_group_chain_id(sdk, group)
	return &SDK{
		c:        sdk,
		SMCrypto: smCrypto != 0,
		WASM:     wasm != 0,
		GroupID:  group,
		ChainID:  chainID,
	}
}

func (sdk *SDK) GetBlockLimit() int {
	return int(C.bcos_rpc_get_block_limit(sdk.c, sdk.GroupID))
}

func (sdk *SDK) bcos_rpc_get_group_list() {
	C.bcos_rpc_get_group_list(sdk.c, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), sdk.c)
}

func (sdk *SDK) bcos_rpc_get_sealer_list() {
	C.bcos_rpc_get_sealer_list(sdk.c, sdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), nil)
}

func (sdk *SDK) bcos_rpc_get_block_number() {
	C.bcos_rpc_get_block_number(sdk.c, sdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), sdk.c)
}

func blockNotify(group *C.char, block_number C.int64_t, ctx unsafe.Pointer) {
	fmt.Printf("Hello\n")
}

func (sdk *SDK) RegisterBlockNotify(groupID string, context unsafe.Pointer) {
	cGroupID := C.CString(groupID)
	defer C.free(unsafe.Pointer(cGroupID))
	//C.bcos_sdk_register_block_notifier(sdk.c, cGroupID, unsafe.Pointer(context), blockNotify)
}

func (sdk *SDK) Close() {
	C.bcos_sdk_stop(sdk.c)
	C.bcos_sdk_destroy(sdk.c)
	C.free(unsafe.Pointer(sdk.GroupID))
	C.free(unsafe.Pointer(sdk.ChainID))
}

// tx functions
func CreateSignedTransaction(groupID, chainID, to string, data []byte, abi string, blockLimit int64) unsafe.Pointer {
	cGroupID := C.CString(groupID)
	cChainID := C.CString(chainID)
	cTo := C.CString(to)
	hecData := hex.EncodeToString(data)
	cData := C.CString(hecData)
	cAbi := C.CString(abi)
	cBlockLimit := C.int64_t(blockLimit)
	defer C.free(unsafe.Pointer(cGroupID))
	defer C.free(unsafe.Pointer(cChainID))
	defer C.free(unsafe.Pointer(cTo))
	defer C.free(unsafe.Pointer(cData))
	defer C.free(unsafe.Pointer(cAbi))
	return C.bcos_sdk_create_transaction_data(cGroupID, cChainID, cTo, cData, cAbi, cBlockLimit)
}

func main() {
	sdk := NewSDKByConfigFile("./config_sample.ini", "group0", nil)
	logrus.Infof("group id:%s", C.GoString(sdk.GroupID))

	sdk.bcos_rpc_get_group_list()
	time.Sleep(3 * time.Second)

	sdk.bcos_rpc_get_sealer_list()
	time.Sleep(3 * time.Second)

	sdk.bcos_rpc_get_block_number()
	time.Sleep(3 * time.Second)

	logrus.Infof("Get Block Limit:%d", sdk.GetBlockLimit)
	logrus.Infof("chain id:%s", C.GoString(sdk.ChainID))

	defer sdk.Close()
}
