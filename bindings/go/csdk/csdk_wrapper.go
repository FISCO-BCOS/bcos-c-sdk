package csdk

// #cgo darwin,arm64 LDFLAGS: -L/usr/local/lib/bcos-c-sdk/libs/darwin -lbcos-c-sdk-aarch64
// #cgo darwin,amd64 LDFLAGS: -L/usr/local/lib/bcos-c-sdk/libs/darwin -lbcos-c-sdk
// #cgo linux,amd64 LDFLAGS: -L/usr/local/lib/bcos-c-sdk/libs/linux -lbcos-c-sdk
// #cgo linux,arm64 LDFLAGS: -L/usr/local/lib/bcos-c-sdk/libs/linux -lbcos-c-sdk-aarch64
// #cgo windows,amd64 LDFLAGS: -L${SRCDIR}/libs/win -lbcos-c-sdk
// #cgo CFLAGS: -I./
// #include "../../../bcos-c-sdk/bcos_sdk_c_common.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_error.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_rpc.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_uti_tx.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_amop.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_event_sub.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
// void on_recv_resp_callback(struct bcos_sdk_c_struct_response *);
// void on_recv_event_resp_callback(struct bcos_sdk_c_struct_response *);
// void on_recv_amop_publish_resp(struct bcos_sdk_c_struct_response *);
// void on_recv_amop_subscribe_resp(char* ,char* , struct bcos_sdk_c_struct_response *);
// void on_recv_notify_resp_callback(char* , int64_t , void* );
import "C"

import (
	"strconv"
	"unsafe"

	"github.com/sirupsen/logrus"
)

type CSDK struct {
	Sdk           unsafe.Pointer
	SMCrypto      C.int
	WASM          bool
	ChainID       *C.char
	GroupID       *C.char
	NodeID        *C.char
	KeyPair       unsafe.Pointer
	PrivateKey    *C.char
	PrivateKeyLen C.uint
	Callback      *C.bcos_sdk_c_struct_response_cb
}

type ChanData struct {
	Sdk  unsafe.Pointer
	Data chan string
}

const (
	timeout = 1000
	proof   = 1 //是否返回交易回执证明, 0：不返回，1：返回
)

//export on_recv_notify_resp_callback
func on_recv_notify_resp_callback(group *C.char, block C.int64_t, context unsafe.Pointer) {
	//logrus.Infof(" \t recv block notifier from server ===>>>> group: %s, block number: %d\n", C.GoString(group), block)
	chanData := (*ChanData)(unsafe.Pointer(context))
	blockNum := strconv.Itoa(int(block))
	chanData.Data <- blockNum
}

//export on_recv_amop_subscribe_resp
func on_recv_amop_subscribe_resp(endpoint *C.char, seq *C.char, resp *C.struct_bcos_sdk_c_struct_response) {
	if int(resp.error) != 0 {
		logrus.Errorf("\t something is wrong, error: %d, errorMessage: %s\n", resp.error, resp.desc)
	} else {
		data := C.GoString((*C.char)(resp.data))
		chanData := (*ChanData)(unsafe.Pointer(resp.context))
		//logrus.Infof("\t recv amop message and would echo message to publish, endpoint: %s, seq: %s, msg size: %d\n", C.GoString(endpoint), C.GoString(seq), resp.size)
		//logrus.Infof(" \t  *****recv amop resp from server ==>>>> resp -> data: %s\n", data[:resp.size])
		C.bcos_amop_send_response(unsafe.Pointer(chanData.Sdk), endpoint, seq, resp.data, resp.size)
		chanData.Data <- data[:resp.size]
	}
}

//export on_recv_amop_publish_resp
func on_recv_amop_publish_resp(resp *C.struct_bcos_sdk_c_struct_response) {
	if int(resp.error) != 0 {
		logrus.Errorf("\t something is wrong, error: %d, errorMessage: %s\n", resp.error, C.GoString(resp.desc))
	} else {
		data := C.GoString((*C.char)(resp.data))
		chanData := (*ChanData)(unsafe.Pointer(resp.context))
		//logrus.Infof(" \t  *****recv amop publish resp from server ==>>>> resp -> data: %s\n", data[:resp.size])
		//logrus.Infof(" \t  *****recv amop publish resp from server ==>>>> resp -> size: %d\n", resp.size)
		chanData.Data <- data[:resp.size]
	}
}

//export on_recv_resp_callback
func on_recv_resp_callback(resp *C.struct_bcos_sdk_c_struct_response) {
	if int(resp.error) != 0 {
		logrus.Errorf("\t something is wrong, error: %d, errorMessage: %s\n", resp.error, C.GoString(resp.desc))
	} else {
		data := C.GoString((*C.char)(resp.data))
		chanData := (*ChanData)(unsafe.Pointer(resp.context))
		//logrus.Infof(" \t  *****recv rpc resp from server ==>>>> resp -> data: %s\n", data[:resp.size])
		//logrus.Infof(" \t  *****recv rpc resp from server ==>>>> resp -> size: %d\n", resp.size)
		chanData.Data <- data[:resp.size]
	}
}

//export on_recv_event_resp_callback
func on_recv_event_resp_callback(resp *C.struct_bcos_sdk_c_struct_response) {
	if int(resp.error) != 0 {
		logrus.Errorf("\t something is wrong, error: %d, errorMessage: %s\n", resp.error, C.GoString(resp.desc))
	} else {
		data := C.GoString((*C.char)(resp.data))
		chanData := (*ChanData)(unsafe.Pointer(resp.context))
		//logrus.Infof(" \t  *****recv rpc resp from server ==>>>> resp -> data: %s\n", data[:resp.size])
		//logrus.Infof(" \t  *****recv rpc resp from server ==>>>> resp -> size: %d\n", resp.size)
		chanData.Data <- data[:resp.size]
	}
}

func NewSDK(groupID string, host string, port int, isSmSsl int, privateKey string) *CSDK {
	cHost := C.CString(host)
	cPort := C.int(port)
	cIsSmSsl := C.int(isSmSsl)
	cPrivateKey := C.CString(privateKey)
	cPrivateKeyLen := C.uint(len(privateKey))
	config := C.bcos_sdk_create_config(cIsSmSsl, cHost, cPort)
	defer C.bcos_sdk_c_config_destroy(unsafe.Pointer(config))

	sdk := C.bcos_sdk_create(config)
	if sdk == nil {
		message := C.bcos_sdk_get_last_error_msg()
		//defer C.free(unsafe.Pointer(message))
		logrus.Errorf("bcos_sdk_create failed with error: %s", C.GoString(message))
		return nil
	}
	C.bcos_sdk_start(sdk)
	var wasm, smCrypto C.int
	group := C.CString(groupID)
	C.bcos_sdk_get_group_wasm_and_crypto(sdk, group, &wasm, &smCrypto)
	chainID := C.bcos_sdk_get_group_chain_id(sdk, group)
	keyPair := C.bcos_sdk_create_keypair_by_private_key(smCrypto, unsafe.Pointer(cPrivateKey), cPrivateKeyLen)
	return &CSDK{
		Sdk:           sdk,
		SMCrypto:      smCrypto,
		WASM:          wasm != 0,
		GroupID:       group,
		ChainID:       chainID,
		PrivateKey:    cPrivateKey,
		PrivateKeyLen: cPrivateKeyLen,
		KeyPair:       keyPair,
	}
}

func NewSDKByConfigFile(configFile string, groupID string, privateKey string) *CSDK {
	config := C.CString(configFile)
	cPrivateKey := C.CString(privateKey)
	cPrivateKeyLen := C.uint(len(privateKey))
	defer C.free(unsafe.Pointer(config))
	sdk := C.bcos_sdk_create_by_config_file(config)
	if sdk == nil {
		message := C.bcos_sdk_get_last_error_msg()
		//defer C.free(unsafe.Pointer(message))
		logrus.Errorf("bcos sdk create by config file failed with error: %s", C.GoString(message))
		return nil
	}

	C.bcos_sdk_start(sdk)
	error := C.bcos_sdk_get_last_error()
	if error != 0 {
		message := C.bcos_sdk_get_last_error_msg()
		//defer C.free(unsafe.Pointer(message))
		logrus.Errorf("bcos sdk start failed with error: %s", C.GoString(message))
		return nil
	}

	var wasm, smCrypto C.int
	CGroupID := C.CString(groupID)
	C.bcos_sdk_get_group_wasm_and_crypto(sdk, CGroupID, &wasm, &smCrypto)
	chainID := C.bcos_sdk_get_group_chain_id(sdk, CGroupID)
	keyPair := C.bcos_sdk_create_keypair_by_private_key(smCrypto, unsafe.Pointer(cPrivateKey), cPrivateKeyLen)
	return &CSDK{
		Sdk:           sdk,
		SMCrypto:      smCrypto,
		WASM:          wasm != 0,
		GroupID:       CGroupID,
		ChainID:       chainID,
		PrivateKey:    cPrivateKey,
		PrivateKeyLen: cPrivateKeyLen,
		KeyPair:       keyPair,
		//NodeID:  C.CString("7c9e8d63a5451ef71e567216f1e2db1478147b9e3eca1c2889f864dc6711d291d3cf458606e39cad5a5dd876ab8cdc3a7dc8f227e9aff1ff1f309329a64f87a7"),
	}
}

func (csdk *CSDK) Close() {
	C.bcos_sdk_stop(csdk.Sdk)
	C.bcos_sdk_destroy(csdk.Sdk)
	C.bcos_sdk_c_free(unsafe.Pointer(csdk.GroupID))
	C.bcos_sdk_c_free(unsafe.Pointer(csdk.ChainID))
	C.free(unsafe.Pointer(csdk.PrivateKey))
	C.bcos_sdk_destroy_keypair(csdk.KeyPair)
}

func (csdk *CSDK) Call(hc *ChanData, to string, data string) {
	cData := C.CString(data)
	cTo := C.CString(to)
	defer C.free(unsafe.Pointer(cData))
	defer C.free(unsafe.Pointer(cTo))
	C.bcos_rpc_call(csdk.Sdk, csdk.GroupID, nil, cTo, cData, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(hc))
}

func (csdk *CSDK) GetTransaction(chanData *ChanData, txHash string) {
	cTxhash := C.CString(txHash)
	cProof := C.int(proof)
	defer C.free(unsafe.Pointer(cTxhash))
	C.bcos_rpc_get_transaction(csdk.Sdk, csdk.GroupID, nil, cTxhash, cProof, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetTransactionReceipt(hc *ChanData, txHash string) {
	cTxhash := C.CString(txHash)
	cProof := C.int(proof)
	defer C.free(unsafe.Pointer(cTxhash))
	C.bcos_rpc_get_transaction_receipt(csdk.Sdk, csdk.GroupID, nil, cTxhash, cProof, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(hc))
}

func (csdk *CSDK) GetBlockLimit() int {
	return int(C.bcos_rpc_get_block_limit(csdk.Sdk, csdk.GroupID))
}

func (csdk *CSDK) GetGroupList(chanData *ChanData) {
	C.bcos_rpc_get_group_list(csdk.Sdk, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetGroupInfo(chanData *ChanData) {
	C.bcos_rpc_get_group_info(csdk.Sdk, csdk.GroupID, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetCode(chanData *ChanData, address string) {
	cAddress := C.CString(address)
	defer C.free(unsafe.Pointer(cAddress))
	C.bcos_rpc_get_code(csdk.Sdk, csdk.GroupID, nil, cAddress, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetSealerList(chanData *ChanData) {
	C.bcos_rpc_get_sealer_list(csdk.Sdk, csdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetObserverList(chanData *ChanData) {
	C.bcos_rpc_get_observer_list(csdk.Sdk, csdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetPbftView(chanData *ChanData) {
	C.bcos_rpc_get_pbft_view(csdk.Sdk, csdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetPendingTxSize(chanData *ChanData) {
	C.bcos_rpc_get_pending_tx_size(csdk.Sdk, csdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetSyncStatus(chanData *ChanData) {
	C.bcos_rpc_get_sync_status(csdk.Sdk, csdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetConsensusStatus(chanData *ChanData) {
	C.bcos_rpc_get_consensus_status(csdk.Sdk, csdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetGroupPeers(chanData *ChanData) {
	C.bcos_rpc_get_group_peers(csdk.Sdk, csdk.GroupID, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetPeers(chanData *ChanData) {
	C.bcos_rpc_get_peers(csdk.Sdk, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetBlockNumber(chanData *ChanData) {
	C.bcos_rpc_get_block_number(csdk.Sdk, csdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))
}

func (csdk *CSDK) GetBlockHashByNumber(hc *ChanData, blockNumber int64) {
	cBlockNumber := C.int64_t(blockNumber)
	C.bcos_rpc_get_block_hash_by_number(csdk.Sdk, csdk.GroupID, nil, cBlockNumber, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(hc))
}

func (csdk *CSDK) GetBlockByhash(hc *ChanData, blockHash string, onlyHeader int32, onlyTxHash int32) {
	cBlockHash := C.CString(blockHash)
	cOnlyHeader := C.int(onlyHeader)
	cOnlyTxHash := C.int(onlyTxHash)
	defer C.free(unsafe.Pointer(cBlockHash))
	C.bcos_rpc_get_block_by_hash(csdk.Sdk, csdk.GroupID, nil, cBlockHash, cOnlyHeader, cOnlyTxHash, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(hc))
}

func (csdk *CSDK) GetBlockByNumber(hc *ChanData, blockNumber int64, onlyHeader int32, onlyTxHash int32) {
	cBlockNumber := C.int64_t(blockNumber)
	cOnlyHeader := C.int(onlyHeader)
	cOnlyTxHash := C.int(onlyTxHash)
	C.bcos_rpc_get_block_by_number(csdk.Sdk, csdk.GroupID, nil, cBlockNumber, cOnlyHeader, cOnlyTxHash, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(hc))
}

func (csdk *CSDK) GetGroupnodeInfo(hc *ChanData, nodeName string) {
	cNodeName := C.CString(nodeName)
	defer C.free(unsafe.Pointer(cNodeName))
	C.bcos_rpc_get_group_node_info(csdk.Sdk, csdk.GroupID, cNodeName, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(hc))
}

func (csdk *CSDK) GetGroupNodeInfoList(hc *ChanData) {
	C.bcos_rpc_get_group_info_list(csdk.Sdk, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(hc))
}

func (csdk *CSDK) GetTotalTransactionCount(hc *ChanData) {
	C.bcos_rpc_get_total_transaction_count(csdk.Sdk, csdk.GroupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(hc))
}

func (csdk *CSDK) GetSystemConfigByKey(hc *ChanData, key string) {
	cKey := C.CString(key)
	defer C.free(unsafe.Pointer(cKey))
	C.bcos_rpc_get_system_config_by_key(csdk.Sdk, csdk.GroupID, nil, cKey, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(hc))
}

// amop
func (csdk *CSDK) SubscribeTopic(chanData *ChanData, topic string) {
	cTopic := C.CString(topic)
	defer C.free(unsafe.Pointer(cTopic))
	cLen := C.size_t(len(topic))
	C.bcos_amop_subscribe_topic(csdk.Sdk, &cTopic, cLen)
}

func (csdk *CSDK) SubscribeTopicWithCb(chanData *ChanData, topic string) {
	cTopic := C.CString(topic)
	defer C.free(unsafe.Pointer(cTopic))
	chanData.Sdk = csdk.Sdk
	C.bcos_amop_subscribe_topic_with_cb(csdk.Sdk, cTopic, C.bcos_sdk_c_struct_response_cb(C.on_recv_amop_subscribe_resp), unsafe.Pointer(chanData))
}

func (csdk *CSDK) UnsubscribeTopicWithCb(chanData *ChanData, topic string) {
	cTopic := C.CString(topic)
	defer C.free(unsafe.Pointer(cTopic))
	cLen := C.size_t(len(topic))
	C.bcos_amop_unsubscribe_topic(csdk.Sdk, &cTopic, cLen)
}

func (csdk *CSDK) PublishTopicMsg(chanData *ChanData, topic string, data string) {
	cTopic := C.CString(topic)
	cData := C.CString(data)
	cLen := C.size_t(len(data))
	cTimeout := C.uint32_t(timeout)
	defer C.free(unsafe.Pointer(cTopic))
	defer C.free(unsafe.Pointer(cData))
	C.bcos_amop_publish(csdk.Sdk, cTopic, unsafe.Pointer(cData), cLen, cTimeout, C.bcos_sdk_c_struct_response_cb(C.on_recv_amop_publish_resp), unsafe.Pointer(chanData))
}

func (csdk *CSDK) BroadcastAmopMsg(chanData *ChanData, topic string, data string) {
	cTopic := C.CString(topic)
	cData := C.CString(data)
	cLen := C.size_t(len(data))
	defer C.free(unsafe.Pointer(cTopic))
	defer C.free(unsafe.Pointer(cData))
	C.bcos_amop_broadcast(csdk.Sdk, cTopic, unsafe.Pointer(cData), cLen)
}

// event
func (csdk *CSDK) SubscribeEvent(chanData *ChanData, params string) string {
	cParams := C.CString(params)
	defer C.free(unsafe.Pointer(cParams))
	return C.GoString(C.bcos_event_sub_subscribe_event(csdk.Sdk, csdk.GroupID, cParams, C.bcos_sdk_c_struct_response_cb(C.on_recv_event_resp_callback), unsafe.Pointer(chanData)))
}

func (csdk *CSDK) UnsubscribeEvent(chanData *ChanData, taskId string) {
	cTaskId := C.CString(taskId)
	defer C.free(unsafe.Pointer(cTaskId))
	C.bcos_event_sub_unsubscribe_event(csdk.Sdk, cTaskId)
}

func (csdk *CSDK) RegisterBlockNotifier(chanData *ChanData) {
	C.bcos_sdk_register_block_notifier(csdk.Sdk, csdk.GroupID, unsafe.Pointer(chanData), C.bcos_sdk_c_struct_response_cb(C.on_recv_notify_resp_callback))
}

func (csdk *CSDK) SendTransaction(chanData *ChanData, to string, data string) {
	cTo := C.CString(to)
	cProof := C.int(proof)
	cData := C.CString(data)
	cNull := C.CString("")
	var tx_hash *C.char
	var signed_tx *C.char
	defer C.free(unsafe.Pointer(cTo))
	defer C.free(unsafe.Pointer(cData))
	defer C.free(unsafe.Pointer(cNull)) //todo
	defer C.bcos_sdk_c_free(unsafe.Pointer(tx_hash))
	defer C.bcos_sdk_c_free(unsafe.Pointer(signed_tx))
	block_limit := C.bcos_rpc_get_block_limit(csdk.Sdk, csdk.GroupID)
	if block_limit < 0 {
		logrus.Errorf("group not exist, group: %s\n", C.GoString(csdk.GroupID))
		return
	}

	C.bcos_sdk_create_signed_transaction(csdk.KeyPair, csdk.GroupID, csdk.ChainID, cTo, cData, cNull, block_limit, 0, &tx_hash, &signed_tx)

	if C.bcos_sdk_is_last_opr_success() == 0 {
		logrus.Errorf("bcos_sdk_create_signed_transaction_with_signed_data failed, error: %s\n", C.GoString(C.bcos_sdk_get_last_error_msg()))
		return
	}

	C.bcos_rpc_send_transaction(csdk.Sdk, csdk.GroupID, nil, signed_tx, cProof, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), unsafe.Pointer(chanData))

	if C.bcos_sdk_is_last_opr_success() == 0 {
		logrus.Errorf("bcos rpc send transaction failed, error: %s\n", C.GoString(C.bcos_sdk_get_last_error_msg()))
		return
	}
}
