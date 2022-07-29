package csdk

// #cgo darwin,arm64 LDFLAGS: -L${SRCDIR}/libs/darwin -lbcos-c-sdk-arm64
// #cgo darwin,amd64 LDFLAGS: -L${SRCDIR}/libs/darwin -lbcos-c-sdk-x86_64
// #cgo linux,amd64 LDFLAGS: -L${SRCDIR}/libs/linux -lbcos-c-sdk
// #cgo windows,amd64 LDFLAGS: -L${SRCDIR}/libs/win -lbcos-c-sdk
// #cgo CFLAGS: -I./
// #include "../../../bcos-c-sdk/bcos_sdk_c.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_error.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_rpc.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_uti_tx.h"
// #include <stdlib.h>
// void on_recv_resp_callback(struct bcos_sdk_c_struct_response *);
import "C"

import (
	"encoding/hex"
	"fmt"
	"github.com/sirupsen/logrus"
	"unsafe"
)

type CSDK struct {
	c          unsafe.Pointer
	SMCrypto   bool
	WASM       bool
	ChainID    *C.char
	GroupID    *C.char
	NodeID    *C.char
	Callback   *C.bcos_sdk_c_struct_response_cb
}

type ChanData struct {
	Data    chan string
}

//export on_recv_resp_callback
func on_recv_resp_callback(resp *C.struct_bcos_sdk_c_struct_response) {
	if int(resp.error) != 0 {
		logrus.Errorf("\t something is wrong, error: %d, errorMessage: %s\n", resp.error, resp.desc)
	} else {
		data := C.GoString((*C.char)(resp.data))
		chanData := (*ChanData)(unsafe.Pointer(resp.context))
		logrus.Infof(" \t  *****recv rpc resp from server ==>>>> resp -> data: %s\n", data)
		chanData.Data <- data
	}
}

func NewSDK(config *C.struct_bcos_sdk_c_config, groupID string) *CSDK {
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
	return &CSDK{
		c:        sdk,
		SMCrypto: smCrypto != 0,
		WASM:     wasm != 0,
		GroupID:  group,
		ChainID:  chainID,
	}
}

func NewSDKByConfigFile(configFile string, groupID string) *CSDK {
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
	return &CSDK{
		c:        sdk,
		SMCrypto: smCrypto != 0,
		WASM:     wasm != 0,
		GroupID:  group,
		ChainID:  chainID,
	}
}

func (sdk *CSDK) Close() {
	C.bcos_sdk_stop(sdk.c)
	C.bcos_sdk_destroy(sdk.c)
	C.free(unsafe.Pointer(sdk.GroupID))
	C.free(unsafe.Pointer(sdk.ChainID))
}

func (sdk *CSDK) GetBlockLimit() int {
	return int(C.bcos_rpc_get_block_limit(sdk.c, sdk.GroupID))
}

func (sdk *CSDK) GetGroupList(chanData *ChanData) {
	C.bcos_rpc_get_group_list(sdk.c, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (sdk *CSDK) GetSealerList(chanData *ChanData) {
	C.bcos_rpc_get_sealer_list(sdk.c, sdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (sdk *CSDK) GetBlockNumber(chanData *ChanData) {
	C.bcos_rpc_get_block_number(sdk.c, sdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

//func (sdk *CSDK) GetBlockHashByNumber(hc *ChanData, blockNumber int) {
//	C.bcos_rpc_get_block_hash_by_number(sdk.c, sdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(hc))
//}

func blockNotify(group *C.char, block_number C.int64_t, ctx unsafe.Pointer) {
	fmt.Printf("Hello\n")
}

func (sdk *CSDK) RegisterBlockNotify(groupID string, context unsafe.Pointer) {
	cGroupID := C.CString(groupID)
	defer C.free(unsafe.Pointer(cGroupID))
	//C.bcos_sdk_register_block_notifier(sdk.c, cGroupID, unsafe.Pointer(context), blockNotify)
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
