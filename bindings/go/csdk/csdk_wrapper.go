package csdk

// #cgo darwin,arm64 LDFLAGS: -L/usr/local/lib/ -L${SRCDIR}/libs -lbcos-c-sdk-aarch64
// #cgo darwin,amd64 LDFLAGS: -L/usr/local/lib/ -L${SRCDIR}/libs -lbcos-c-sdk
// #cgo linux,amd64 LDFLAGS: -L/usr/local/lib/ -L${SRCDIR}/libs -lbcos-c-sdk
// #cgo linux,arm64 LDFLAGS: -L/usr/local/lib/ -L${SRCDIR}/libs -lbcos-c-sdk-aarch64
// #cgo windows,amd64 LDFLAGS: -L${SRCDIR}/libs -L${SRCDIR}/libs/win -lbcos-c-sdk
// #cgo CFLAGS: -I./
// #include <stdlib.h>
// #include <stdbool.h>
// #include "../../../bcos-c-sdk/bcos_sdk_c_common.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_error.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_rpc.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_uti_tx.h"
// void bcos_sdk_create_signed_transaction_with_full_fields(void* key_pair, const char* group_id,
//     const char* chain_id, const char* to, const char* nonce, const unsigned char* input,
//     long inputSize, const char* abi, int64_t block_limit, const char* value, const char* gas_price,
//     int64_t gas_limit, int32_t attribute, const char* extra_data, char** tx_hash, char** signed_tx);
// void* bcos_sdk_create_transaction_v1_data(
//   const char* group_id,
//   const char* chain_id,
//   const char* to,
//   const char* nonce,
//   const unsigned char* input,
//   long inputSize,
//   const char* abi,
//   int64_t block_limit,
//   const char* value,
//   const char* gas_price,
//   int64_t gas_limit);
// #include "../../../bcos-c-sdk/bcos_sdk_c_amop.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_event_sub.h"
// #include "../../../bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
// void on_recv_resp_callback(struct bcos_sdk_c_struct_response *);
// void on_recv_event_resp_callback(struct bcos_sdk_c_struct_response *);
// void on_recv_amop_publish_resp(struct bcos_sdk_c_struct_response *);
// void on_recv_amop_subscribe_resp(char*, char*, struct bcos_sdk_c_struct_response *);
// void on_recv_notify_resp_callback(char*, int64_t, void* );
import "C"

import (
	"encoding/hex"
	"fmt"
	"strings"
	"sync"
	"time"
	"unsafe"

	cache "github.com/patrickmn/go-cache"
)

type CSDK struct {
	sdk             unsafe.Pointer
	smCrypto        bool
	wasm            bool
	chainID         *C.char
	groupID         *C.char
	keyPair         unsafe.Pointer
	privateKeyBytes []byte
	// keyPairMutex guards keyPair pointer lifetime.
	// Sign/read paths take RLock; SetPrivateKey/Close take Lock so they exclude concurrent signs.
	keyPairMutex sync.RWMutex
}

var contextCache = cache.New(5*time.Minute, 10*time.Minute)
var indexCache = cache.New(5*time.Minute, 10*time.Minute)

const C_SDK_ECDSA_CRYPTO = C.int(0)
const C_SDK_SM_CRYPTO = C.int(1)

type AmopMessageContext struct {
	Peer string
	Seq  string
	Data []byte
}

type Response struct {
	Result interface{}
	Err    error
}

type CallbackChan struct {
	sdk     unsafe.Pointer
	Data    chan Response
	Handler interface{}
}

//export on_recv_notify_resp_callback
func on_recv_notify_resp_callback(group *C.char, block C.int64_t, context unsafe.Pointer) {
	chanData := getContext(context, false)
	// b := make([]byte, 8)
	// binary.LittleEndian.PutUint64(b, uint64(block))
	chanData.Handler.(func(string, uint64))(C.GoString(group), uint64(block))
	// chanData.Data <- Response{b, nil}
}

//export on_recv_amop_subscribe_resp
func on_recv_amop_subscribe_resp(endpoint *C.char, seq *C.char, resp *C.struct_bcos_sdk_c_struct_response) {
	chanData := getContext(resp.context, false)
	if int(resp.error) != 0 {
		chanData.Data <- Response{nil, fmt.Errorf("something is wrong, error: %d, errorMessage: %s", resp.error, C.GoString(resp.desc))}
	} else {
		// C.bcos_amop_send_response(unsafe.Pointer(chanData.sdk), endpoint, seq, resp.data, resp.size)
		data := C.GoBytes(unsafe.Pointer(resp.data), C.int(resp.size))
		peer := C.GoString(endpoint)
		seq := C.GoString(seq)
		chanData.Handler.(func(string, string, []byte))(peer, seq, data)
		// chanData.Data <- Response{AmopMessageContext{Peer: peer, Seq: seq, Data: data}, nil}
	}
}

//export on_recv_amop_publish_resp
func on_recv_amop_publish_resp(resp *C.struct_bcos_sdk_c_struct_response) {
	on_callback_once(resp)
}

//export on_recv_resp_callback
func on_recv_resp_callback(resp *C.struct_bcos_sdk_c_struct_response) {
	on_callback_once(resp)
}

//export on_recv_event_resp_callback
func on_recv_event_resp_callback(resp *C.struct_bcos_sdk_c_struct_response) {
	chanData := getContext(resp.context, false)
	if int(resp.error) != 0 {
		chanData.Handler.(func([]byte, error))(nil, fmt.Errorf("something is wrong, error: %d, errorMessage: %s", resp.error, C.GoString(resp.desc)))
	} else {
		data := C.GoBytes(unsafe.Pointer(resp.data), C.int(resp.size))
		chanData.Handler.(func([]byte, error))(data, nil)
	}
}

func on_callback_once(resp *C.struct_bcos_sdk_c_struct_response) {
	chanData := getContext(resp.context, true)
	if chanData == nil {
		panic("callback channel is nil")
	}
	var respData Response
	if int(resp.error) != 0 {
		respData.Err = fmt.Errorf("something is wrong, error: %d, errorMessage: %s", resp.error, C.GoString(resp.desc))
		respData.Result = []byte{}
	} else {
		respData.Result = C.GoBytes(unsafe.Pointer(resp.data), C.int(resp.size))
	}
	if chanData.Data != nil {
		chanData.Data <- respData
	} else {
		go chanData.Handler.(func([]byte, error))(respData.Result.([]byte), respData.Err)
	}
}

func setContext(callback *CallbackChan) unsafe.Pointer {
	pointer := fmt.Sprintf("%p", callback)
	contextCache.Set(pointer, callback, cache.NoExpiration)
	cPointer := C.CString(pointer)
	return unsafe.Pointer(cPointer)
}

func getContext(index unsafe.Pointer, delete bool) *CallbackChan {
	p := C.GoString((*C.char)(unsafe.Pointer(index)))
	context, found := contextCache.Get(p)
	if found {
		if delete {
			C.free(index)
			contextCache.Delete(p)
		}
		return context.(*CallbackChan)
	}
	return nil
}

// Endpoint is a BCOS RPC peer (host + port).
type Endpoint struct {
	Host string
	Port int
}

// SdkOptions configures [common] for programmatic connections (INI-equivalent defaults when nil).
type SdkOptions struct {
	ThreadPoolSize                   *int
	MessageTimeoutMs                 *int
	SendRpcRequestToHighestBlockNode *bool
}

func intDefault(p *int, fallback int) int {
	if p != nil {
		return *p
	}
	return fallback
}

func boolDefault(p *bool, fallback bool) bool {
	if p != nil {
		return *p
	}
	return fallback
}

func applyTlsToConfig(config *C.struct_bcos_sdk_c_config, isSmSsl, disableSsl bool, tlsCaPath, tlsKeyPath, tlsCertPath, tlsSmEnKey, tlsSEnCert string) {
	if disableSsl {
		config.disable_ssl = C.int(1)
		return
	}
	cTlsCaPath := C.CString(tlsCaPath)
	cTlsKeyPath := C.CString(tlsKeyPath)
	cTlsCertPath := C.CString(tlsCertPath)
	if isSmSsl {
		C.bcos_sdk_c_free(unsafe.Pointer(config.sm_cert_config.ca_cert))
		config.sm_cert_config.ca_cert = cTlsCaPath
		C.bcos_sdk_c_free(unsafe.Pointer(config.sm_cert_config.node_key))
		config.sm_cert_config.node_key = cTlsKeyPath
		C.bcos_sdk_c_free(unsafe.Pointer(config.sm_cert_config.node_cert))
		config.sm_cert_config.node_cert = cTlsCertPath
		C.bcos_sdk_c_free(unsafe.Pointer(config.sm_cert_config.en_node_key))
		cTlsSmEnKey := C.CString(tlsSmEnKey)
		config.sm_cert_config.en_node_key = cTlsSmEnKey
		C.bcos_sdk_c_free(unsafe.Pointer(config.sm_cert_config.en_node_cert))
		cTlsSmEnCert := C.CString(tlsSEnCert)
		config.sm_cert_config.en_node_cert = cTlsSmEnCert
	} else {
		C.bcos_sdk_c_free(unsafe.Pointer(config.cert_config.ca_cert))
		config.cert_config.ca_cert = cTlsCaPath
		C.bcos_sdk_c_free(unsafe.Pointer(config.cert_config.node_key))
		config.cert_config.node_key = cTlsKeyPath
		C.bcos_sdk_c_free(unsafe.Pointer(config.cert_config.node_cert))
		config.cert_config.node_cert = cTlsCertPath
	}
}

func startSDKFromConfig(groupID string, privateKey []byte, config *C.struct_bcos_sdk_c_config) (*CSDK, error) {
	sdk := C.bcos_sdk_create(config)
	if sdk == nil {
		message := C.bcos_sdk_get_last_error_msg()
		return nil, fmt.Errorf("bcos_sdk_create failed with error: %s", C.GoString(message))
	}
	C.bcos_sdk_start(sdk)
	if C.bcos_sdk_get_last_error() != 0 {
		message := C.bcos_sdk_get_last_error_msg()
		return nil, fmt.Errorf("bcos_sdk_start failed with error: %s", C.GoString(message))
	}

	var wasm, smCrypto C.int
	group := C.CString(groupID)
	C.bcos_sdk_get_group_wasm_and_crypto(sdk, group, &wasm, &smCrypto)
	keyPair := C.bcos_sdk_create_keypair_by_private_key(smCrypto, unsafe.Pointer(&privateKey[0]), C.uint(len(privateKey)))
	if keyPair == nil {
		message := C.bcos_sdk_get_last_error_msg()
		C.bcos_sdk_c_free(unsafe.Pointer(group))
		return nil, fmt.Errorf("bcos_sdk_create_keypair_by_private_key failed with error: %s", C.GoString(message))
	}
	chainID := C.bcos_sdk_get_group_chain_id(sdk, group)
	return &CSDK{
		sdk:             sdk,
		smCrypto:        smCrypto != 0,
		wasm:            wasm != 0,
		groupID:         group,
		chainID:         chainID,
		privateKeyBytes: privateKey,
		keyPair:         keyPair,
	}, nil
}

// NewSDKWithPeers connects to multiple RPC peers with TLS settings and optional [common] overrides.
func NewSDKWithPeers(groupID string, peers []Endpoint, isSmSsl bool, privateKey []byte, disableSsl bool, tlsCaPath, tlsKeyPath, tlsCertPath, tlsSmEnKey, tlsSEnCert string, opts *SdkOptions) (*CSDK, error) {
	if len(peers) == 0 {
		return nil, fmt.Errorf("peers must not be empty")
	}
	for i, p := range peers {
		if p.Host == "" || p.Port <= 0 || p.Port > 65535 {
			return nil, fmt.Errorf("invalid peer at index %d: %s:%d", i, p.Host, p.Port)
		}
	}

	cIsSmSsl := C.int(0)
	if isSmSsl {
		cIsSmSsl = C.int(1)
	}
	firstHost := C.CString(peers[0].Host)
	defer C.free(unsafe.Pointer(firstHost))
	config := C.bcos_sdk_create_config(cIsSmSsl, firstHost, C.int(peers[0].Port))
	if config == nil {
		return nil, fmt.Errorf("bcos_sdk_create_config failed")
	}
	defer C.bcos_sdk_c_config_destroy(unsafe.Pointer(config))

	if len(peers) > 1 {
		epBase := (*C.struct_bcos_sdk_c_endpoint)(C.malloc(C.size_t(len(peers)) * C.size_t(unsafe.Sizeof(C.struct_bcos_sdk_c_endpoint{}))))
		for i, p := range peers {
			ep := (*C.struct_bcos_sdk_c_endpoint)(unsafe.Add(unsafe.Pointer(epBase), uintptr(i)*unsafe.Sizeof(C.struct_bcos_sdk_c_endpoint{})))
			ep.host = C.CString(p.Host)
			ep.port = C.uint16_t(p.Port)
		}
		if config.peers_count > 0 && config.peers != nil {
			first := (*C.struct_bcos_sdk_c_endpoint)(unsafe.Pointer(config.peers))
			C.bcos_sdk_c_free(unsafe.Pointer(first.host))
			C.bcos_sdk_c_free(unsafe.Pointer(config.peers))
		}
		config.peers = epBase
		config.peers_count = C.size_t(len(peers))
	}

	config.thread_pool_size = C.int(intDefault(optsThreadPool(opts), 8))
	config.message_timeout_ms = C.int(intDefault(optsMessageTimeout(opts), 10000))
	sendHighest := boolDefault(optsSendToHighest(opts), true)
	if sendHighest {
		config.send_rpc_request_to_highest_block_node = C.int(1)
	} else {
		config.send_rpc_request_to_highest_block_node = C.int(0)
	}

	applyTlsToConfig(config, isSmSsl, disableSsl, tlsCaPath, tlsKeyPath, tlsCertPath, tlsSmEnKey, tlsSEnCert)
	return startSDKFromConfig(groupID, privateKey, config)
}

func optsThreadPool(opts *SdkOptions) *int {
	if opts == nil {
		return nil
	}
	return opts.ThreadPoolSize
}

func optsMessageTimeout(opts *SdkOptions) *int {
	if opts == nil {
		return nil
	}
	return opts.MessageTimeoutMs
}

func optsSendToHighest(opts *SdkOptions) *bool {
	if opts == nil {
		return nil
	}
	return opts.SendRpcRequestToHighestBlockNode
}

func NewSDK(groupID string, host string, port int, isSmSsl bool, privateKey []byte, disableSsl bool, tlsCaPath, tlsKeyPath, tlsCertPath, tlsSmEnKey, tlsSEnCert string) (*CSDK, error) {
	cHost := C.CString(host)
	defer C.free(unsafe.Pointer(cHost)) // Fix memory leak: free C string allocated by C.CString
	cPort := C.int(port)
	cIsSmSsl := C.int(0)
	if isSmSsl {
		cIsSmSsl = C.int(1)
	}
	config := C.bcos_sdk_create_config(cIsSmSsl, cHost, cPort)
	defer C.bcos_sdk_c_config_destroy(unsafe.Pointer(config))

	applyTlsToConfig(config, isSmSsl, disableSsl, tlsCaPath, tlsKeyPath, tlsCertPath, tlsSmEnKey, tlsSEnCert)
	config.message_timeout_ms = C.int(-1)
	return startSDKFromConfig(groupID, privateKey, config)
}

func NewSDKByConfigFile(configFile string, groupID string, privateKey []byte) (*CSDK, error) {
	config := C.CString(configFile)
	defer C.free(unsafe.Pointer(config))
	sdk := C.bcos_sdk_create_by_config_file(config)
	if sdk == nil {
		message := C.bcos_sdk_get_last_error_msg()
		//defer C.free(unsafe.Pointer(message))
		return nil, fmt.Errorf("bcos sdk create by config file failed with error: %s", C.GoString(message))
	}

	C.bcos_sdk_start(sdk)
	if C.bcos_sdk_get_last_error() != 0 {
		message := C.bcos_sdk_get_last_error_msg()
		//defer C.free(unsafe.Pointer(message))
		return nil, fmt.Errorf("bcos sdk start failed with error: %s", C.GoString(message))
	}

	var wasm, smCrypto C.int
	CGroupID := C.CString(groupID)
	C.bcos_sdk_get_group_wasm_and_crypto(sdk, CGroupID, &wasm, &smCrypto)
	keyPair := C.bcos_sdk_create_keypair_by_private_key(smCrypto, unsafe.Pointer(&privateKey[0]), C.uint(len(privateKey)))
	if keyPair == nil {
		message := C.bcos_sdk_get_last_error_msg()
		C.bcos_sdk_c_free(unsafe.Pointer(CGroupID))
		return nil, fmt.Errorf("bcos_sdk_create_keypair_by_private_key failed with error: %s", C.GoString(message))
	}
	chainID := C.bcos_sdk_get_group_chain_id(sdk, CGroupID)
	return &CSDK{
		sdk:             sdk,
		smCrypto:        smCrypto != 0,
		wasm:            wasm != 0,
		groupID:         CGroupID,
		chainID:         chainID,
		privateKeyBytes: privateKey,
		keyPair:         keyPair,
	}, nil
}

func (csdk *CSDK) Close() {
	csdk.keyPairMutex.Lock()
	defer csdk.keyPairMutex.Unlock()
	C.bcos_sdk_stop(csdk.sdk)
	C.bcos_sdk_destroy(csdk.sdk)
	C.bcos_sdk_c_free(unsafe.Pointer(csdk.groupID))
	C.bcos_sdk_c_free(unsafe.Pointer(csdk.chainID))
	C.bcos_sdk_destroy_keypair(csdk.keyPair)
}

func (csdk *CSDK) GroupID() string {
	return C.GoString(csdk.groupID)
}

func (csdk *CSDK) ChainID() string {
	return C.GoString(csdk.chainID)
}

func (csdk *CSDK) PrivateKeyBytes() []byte {
	return csdk.privateKeyBytes
}

// SetPrivateKey set private key
func (csdk *CSDK) SetPrivateKey(privateKeyBytes []byte) error {
	cryptoType := C_SDK_ECDSA_CRYPTO
	if csdk.smCrypto {
		cryptoType = C_SDK_SM_CRYPTO
	}
	keyPair := C.bcos_sdk_create_keypair_by_private_key(cryptoType, unsafe.Pointer(&privateKeyBytes[0]), C.uint(len(privateKeyBytes)))
	if keyPair == nil {
		message := C.bcos_sdk_get_last_error_msg()
		return fmt.Errorf("SetPrivateKey bcos_sdk_create_keypair_by_private_key failed with error: %s", C.GoString(message))
	}
	csdk.keyPairMutex.Lock()
	defer csdk.keyPairMutex.Unlock()
	C.bcos_sdk_destroy_keypair(csdk.keyPair)
	csdk.keyPair = keyPair
	return nil
}

func (csdk *CSDK) SMCrypto() bool {
	return csdk.smCrypto
}

func (csdk *CSDK) WASM() bool {
	return csdk.wasm
}

func (csdk *CSDK) Call(chanData *CallbackChan, to string, data string) {
	cData := C.CString(data)
	cTo := C.CString(to)
	defer C.free(unsafe.Pointer(cData))
	defer C.free(unsafe.Pointer(cTo))
	C.bcos_rpc_call(csdk.sdk, csdk.groupID, nil, cTo, cData, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetTransaction(chanData *CallbackChan, txHash string, withProof bool) {
	cTxhash := C.CString(txHash)
	cProof := C.int(0)
	if withProof {
		cProof = C.int(1)
	}
	defer C.free(unsafe.Pointer(cTxhash))
	C.bcos_rpc_get_transaction(csdk.sdk, csdk.groupID, nil, cTxhash, cProof, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetTransactionReceipt(chanData *CallbackChan, txHash string, withProof bool) {
	cTxhash := C.CString(txHash)
	cProof := C.int(0)
	if withProof {
		cProof = C.int(1)
	}
	defer C.free(unsafe.Pointer(cTxhash))
	C.bcos_rpc_get_transaction_receipt(csdk.sdk, csdk.groupID, nil, cTxhash, cProof, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetBlockLimit() int {
	return int(C.bcos_rpc_get_block_limit(csdk.sdk, csdk.groupID))
}

func (csdk *CSDK) GetCode(chanData *CallbackChan, address string) {
	cAddress := C.CString(address)
	defer C.free(unsafe.Pointer(cAddress))
	C.bcos_rpc_get_code(csdk.sdk, csdk.groupID, nil, cAddress, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetSealerList(chanData *CallbackChan) {
	C.bcos_rpc_get_sealer_list(csdk.sdk, csdk.groupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetObserverList(chanData *CallbackChan) {
	C.bcos_rpc_get_observer_list(csdk.sdk, csdk.groupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetPbftView(chanData *CallbackChan) {
	C.bcos_rpc_get_pbft_view(csdk.sdk, csdk.groupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetPendingTxSize(chanData *CallbackChan) {
	C.bcos_rpc_get_pending_tx_size(csdk.sdk, csdk.groupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetSyncStatus(chanData *CallbackChan) {
	C.bcos_rpc_get_sync_status(csdk.sdk, csdk.groupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetConsensusStatus(chanData *CallbackChan) {
	C.bcos_rpc_get_consensus_status(csdk.sdk, csdk.groupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetGroupPeers(chanData *CallbackChan) {
	C.bcos_rpc_get_group_peers(csdk.sdk, csdk.groupID, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetPeers(chanData *CallbackChan) {
	C.bcos_rpc_get_peers(csdk.sdk, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetBlockNumber(chanData *CallbackChan) {
	C.bcos_rpc_get_block_number(csdk.sdk, csdk.groupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetBlockHashByNumber(chanData *CallbackChan, blockNumber int64) {
	cBlockNumber := C.int64_t(blockNumber)
	C.bcos_rpc_get_block_hash_by_number(csdk.sdk, csdk.groupID, nil, cBlockNumber, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetBlockByHash(chanData *CallbackChan, blockHash string, onlyHeader, onlyTxHash bool) {
	cBlockHash := C.CString(blockHash)
	cOnlyHeader := C.int(0)
	if onlyHeader {
		cOnlyHeader = C.int(1)
	}
	cOnlyTxHash := C.int(0)
	if onlyTxHash {
		cOnlyTxHash = C.int(1)
	}
	defer C.free(unsafe.Pointer(cBlockHash))
	C.bcos_rpc_get_block_by_hash(csdk.sdk, csdk.groupID, nil, cBlockHash, cOnlyHeader, cOnlyTxHash, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetBlockByNumber(chanData *CallbackChan, blockNumber int64, onlyHeader, onlyTxHash bool) {
	cBlockNumber := C.int64_t(blockNumber)
	cOnlyHeader := C.int(0)
	if onlyHeader {
		cOnlyHeader = C.int(1)
	}
	cOnlyTxHash := C.int(0)
	if onlyTxHash {
		cOnlyTxHash = C.int(1)
	}
	C.bcos_rpc_get_block_by_number(csdk.sdk, csdk.groupID, nil, cBlockNumber, cOnlyHeader, cOnlyTxHash, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetGroupList(chanData *CallbackChan) {
	C.bcos_rpc_get_group_list(csdk.sdk, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetGroupInfo(chanData *CallbackChan) {
	C.bcos_rpc_get_group_info(csdk.sdk, csdk.groupID, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetNodeInfo(chanData *CallbackChan, nodeID string) {
	cNodeID := C.CString(nodeID)
	defer C.free(unsafe.Pointer(cNodeID))
	C.bcos_rpc_get_group_node_info(csdk.sdk, csdk.groupID, cNodeID, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetGroupInfoList(chanData *CallbackChan) {
	C.bcos_rpc_get_group_info_list(csdk.sdk, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetTotalTransactionCount(chanData *CallbackChan) {
	C.bcos_rpc_get_total_transaction_count(csdk.sdk, csdk.groupID, nil, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

func (csdk *CSDK) GetSystemConfigByKey(chanData *CallbackChan, key string) {
	cKey := C.CString(key)
	defer C.free(unsafe.Pointer(cKey))
	C.bcos_rpc_get_system_config_by_key(csdk.sdk, csdk.groupID, nil, cKey, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
}

// SendRPCRequest to specific group or node, group and node can be empty
func (csdk *CSDK) SendRPCRequest(group, node, request string, chanData *CallbackChan) error {
	cGroup := C.CString(group)
	defer C.free(unsafe.Pointer(cGroup))
	cNode := C.CString(node)
	defer C.free(unsafe.Pointer(cNode))
	cRequest := C.CString(request)
	defer C.free(unsafe.Pointer(cRequest))
	C.bcos_rpc_generic_method_call_to_group_node(csdk.sdk, cGroup, cNode, cRequest, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
	if C.bcos_sdk_is_last_opr_success() == 0 {
		return fmt.Errorf("SendRPCRequest, error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	return nil
}

// // amop
// func (csdk *CSDK) SubscribeAmopTopicDefaultHandler(topic []string) {
// 	cTopic := C.CString(topic)
// 	defer C.free(unsafe.Pointer(cTopic))
// 	cLen := C.size_t(len(topic))
// 	C.bcos_amop_subscribe_topic(csdk.sdk, &cTopic, cLen)
// }

func (csdk *CSDK) SubscribeAmopTopic(chanData *CallbackChan, topic string) {
	cTopic := C.CString(topic)
	defer C.free(unsafe.Pointer(cTopic))
	chanData.sdk = csdk.sdk
	index := setContext(chanData)
	indexCache.Set(topic, index, cache.NoExpiration)
	C.bcos_amop_subscribe_topic_with_cb(csdk.sdk, cTopic, C.bcos_sdk_c_struct_response_cb(C.on_recv_amop_subscribe_resp), index)
}

func (csdk *CSDK) UnsubscribeAmopTopic(topic string) {
	cTopic := C.CString(topic)
	defer C.free(unsafe.Pointer(cTopic))
	cLen := C.size_t(len(topic))
	C.bcos_amop_unsubscribe_topic(csdk.sdk, &cTopic, cLen)
	index, found := indexCache.Get(topic)
	if found {
		getContext(index.(unsafe.Pointer), true)
	}
}

func (csdk *CSDK) SendAmopResponse(peer, seq string, data []byte) {
	cPeer := C.CString(peer)
	cSeq := C.CString(seq)
	cData := C.CBytes(data)
	cLen := C.size_t(len(data))
	defer C.free(unsafe.Pointer(cPeer))
	defer C.free(unsafe.Pointer(cSeq))
	defer C.free(unsafe.Pointer(cData))
	C.bcos_amop_send_response(csdk.sdk, cPeer, cSeq, cData, cLen)
}

func (csdk *CSDK) PublishAmopTopicMsg(chanData *CallbackChan, topic string, data []byte, timeout int) {
	cTopic := C.CString(topic)
	cData := C.CBytes(data)
	cLen := C.size_t(len(data))
	cTimeout := C.uint32_t(timeout)
	defer C.free(unsafe.Pointer(cTopic))
	defer C.free(unsafe.Pointer(cData))
	C.bcos_amop_publish(csdk.sdk, cTopic, cData, cLen, cTimeout, C.bcos_sdk_c_struct_response_cb(C.on_recv_amop_publish_resp), setContext(chanData))
}

func (csdk *CSDK) BroadcastAmopMsg(topic string, data []byte) {
	cTopic := C.CString(topic)
	cData := C.CBytes(data)
	cLen := C.size_t(len(data))
	defer C.free(unsafe.Pointer(cTopic))
	defer C.free(unsafe.Pointer(cData))
	C.bcos_amop_broadcast(csdk.sdk, cTopic, cData, cLen)
}

// event
func (csdk *CSDK) SubscribeEvent(chanData *CallbackChan, params string) string {
	cParams := C.CString(params)
	defer C.free(unsafe.Pointer(cParams))
	chanData.sdk = csdk.sdk
	index := setContext(chanData)
	taskID := C.GoString(C.bcos_event_sub_subscribe_event(csdk.sdk, csdk.groupID, cParams, C.bcos_sdk_c_struct_response_cb(C.on_recv_event_resp_callback), index))
	indexCache.Set(taskID, index, cache.NoExpiration)
	return taskID
}

func (csdk *CSDK) UnsubscribeEvent(taskId string) { // TODO: CallbackChan task from contextCache
	cTaskId := C.CString(taskId)
	defer C.free(unsafe.Pointer(cTaskId))
	C.bcos_event_sub_unsubscribe_event(csdk.sdk, cTaskId)
	index, found := indexCache.Get(taskId)
	if found {
		getContext(index.(unsafe.Pointer), true)
	}
}

func (csdk *CSDK) RegisterBlockNotifier(chanData *CallbackChan) { // TODO: implement UnRegisterBlockNotifier
	C.bcos_sdk_register_block_notifier(csdk.sdk, csdk.groupID, setContext(chanData), C.bcos_sdk_c_struct_response_cb(C.on_recv_notify_resp_callback))
}

func (csdk *CSDK) CreateAndSendTransaction(chanData *CallbackChan, to string, data, abi, extraData string, withProof bool) ([]byte, error) {
	cTo := C.CString(to)
	defer C.free(unsafe.Pointer(cTo))
	cProof := C.int(0)
	if withProof {
		cProof = C.int(1)
	}
	cData := C.CString(data)
	defer C.free(unsafe.Pointer(cData))
	cAbi := C.CString(abi)
	defer C.free(unsafe.Pointer(cAbi))
	cExtraData := C.CString(extraData)
	defer C.free(unsafe.Pointer(cExtraData))
	var tx_hash *C.char
	var signed_tx *C.char
	block_limit := C.bcos_rpc_get_block_limit(csdk.sdk, csdk.groupID)
	if block_limit < 0 {
		return nil, fmt.Errorf("group not exist, group: %s", C.GoString(csdk.groupID))
	}
	csdk.keyPairMutex.RLock()
	C.bcos_sdk_create_signed_transaction_ver_extra_data(csdk.keyPair, csdk.groupID, csdk.chainID, cTo, cData, cAbi, block_limit, 0, cExtraData, &tx_hash, &signed_tx)
	if C.bcos_sdk_is_last_opr_success() == 0 {
		csdk.keyPairMutex.RUnlock()
		return nil, fmt.Errorf("bcos_sdk_create_signed_transaction, error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	csdk.keyPairMutex.RUnlock()
	defer C.bcos_sdk_c_free(unsafe.Pointer(tx_hash))
	defer C.bcos_sdk_c_free(unsafe.Pointer(signed_tx))
	txHash, err := hex.DecodeString(strings.TrimPrefix(C.GoString(tx_hash), "0x"))
	if err != nil {
		return nil, err
	}
	C.bcos_rpc_send_transaction(csdk.sdk, csdk.groupID, nil, signed_tx, cProof, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))

	if C.bcos_sdk_is_last_opr_success() == 0 {
		return txHash, fmt.Errorf("bcos rpc send transaction failed, error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	return txHash, nil
}

// SignedTxPair is the result of bcos_sdk_create_signed_transaction_with_full_fields.
type SignedTxPair struct {
	TxHash   string
	SignedTx []byte
}

// NewKeyPairFromPrivateKey creates an ephemeral SM2/secp256k1 keypair (caller must DestroyKeyPair).
func (csdk *CSDK) NewKeyPairFromPrivateKey(privateKey []byte) (unsafe.Pointer, error) {
	if len(privateKey) != 32 {
		return nil, fmt.Errorf("private key must be 32 bytes, got %d", len(privateKey))
	}
	cryptoType := C_SDK_ECDSA_CRYPTO
	if csdk.smCrypto {
		cryptoType = C_SDK_SM_CRYPTO
	}
	keyPair := C.bcos_sdk_create_keypair_by_private_key(cryptoType, unsafe.Pointer(&privateKey[0]), C.uint(len(privateKey)))
	if keyPair == nil {
		return nil, fmt.Errorf("bcos_sdk_create_keypair_by_private_key: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	return keyPair, nil
}

// DestroyKeyPair releases a keypair created by NewKeyPairFromPrivateKey.
func (csdk *CSDK) DestroyKeyPair(keyPair unsafe.Pointer) {
	if keyPair != nil {
		C.bcos_sdk_destroy_keypair(keyPair)
	}
}

// CreateSignedTransactionWithDefaultKeyPair signs using the connection's default keypair.
func (csdk *CSDK) CreateSignedTransactionWithDefaultKeyPair(
	blockLimit int64,
	to string,
	nonce string,
	input []byte,
	abi string,
	attribute int32,
	extraData string,
) (*SignedTxPair, error) {
	csdk.keyPairMutex.RLock()
	defer csdk.keyPairMutex.RUnlock()
	return csdk.createSignedTransactionWithFullFields(csdk.keyPair, blockLimit, to, nonce, input, abi, attribute, "", "", 0, extraData)
}

// CreateSignedTransactionWithPrivateKey signs with an explicit 32-byte private key (ephemeral keypair).
func (csdk *CSDK) CreateSignedTransactionWithPrivateKey(
	privateKey []byte,
	blockLimit int64,
	to string,
	nonce string,
	input []byte,
	abi string,
	attribute int32,
	extraData string,
) (*SignedTxPair, error) {
	keyPair, err := csdk.NewKeyPairFromPrivateKey(privateKey)
	if err != nil {
		return nil, err
	}
	defer csdk.DestroyKeyPair(keyPair)
	return csdk.createSignedTransactionWithFullFields(keyPair, blockLimit, to, nonce, input, abi, attribute, "", "", 0, extraData)
}

func (csdk *CSDK) createSignedTransactionWithFullFields(
	keyPair unsafe.Pointer,
	blockLimit int64,
	to string,
	nonce string,
	input []byte,
	abi string,
	attribute int32,
	value string,
	gasPrice string,
	gasLimit int64,
	extraData string,
) (*SignedTxPair, error) {
	if keyPair == nil {
		return nil, fmt.Errorf("key pair is nil")
	}
	if len(input) == 0 {
		return nil, fmt.Errorf("input is empty")
	}
	cTo := C.CString(to)
	cNonce := C.CString(nonce)
	cAbi := C.CString(abi)
	cValue := C.CString(value)
	cGasPrice := C.CString(gasPrice)
	cExtra := C.CString(extraData)
	defer C.free(unsafe.Pointer(cTo))
	defer C.free(unsafe.Pointer(cNonce))
	defer C.free(unsafe.Pointer(cAbi))
	defer C.free(unsafe.Pointer(cValue))
	defer C.free(unsafe.Pointer(cGasPrice))
	defer C.free(unsafe.Pointer(cExtra))

	var txHash *C.char
	var signedTx *C.char
	C.bcos_sdk_create_signed_transaction_with_full_fields(
		keyPair,
		csdk.groupID,
		csdk.chainID,
		cTo,
		cNonce,
		(*C.uchar)(unsafe.Pointer(&input[0])),
		C.long(len(input)),
		cAbi,
		C.int64_t(blockLimit),
		cValue,
		cGasPrice,
		C.int64_t(gasLimit),
		C.int32_t(attribute),
		cExtra,
		&txHash,
		&signedTx,
	)
	if C.bcos_sdk_is_last_opr_success() == 0 {
		return nil, fmt.Errorf("bcos_sdk_create_signed_transaction_with_full_fields: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	defer C.bcos_sdk_c_free(unsafe.Pointer(txHash))
	defer C.bcos_sdk_c_free(unsafe.Pointer(signedTx))

	encoded, err := hex.DecodeString(strings.TrimPrefix(C.GoString(signedTx), "0x"))
	if err != nil {
		return nil, fmt.Errorf("decode signed tx: %w", err)
	}
	return &SignedTxPair{
		TxHash:   C.GoString(txHash),
		SignedTx: encoded,
	}, nil
}

func (csdk *CSDK) CreateEncodedTransactionDataV1(blockLimit int64, to string, input []byte, abi string) ([]byte, []byte, error) {
	cTo := C.CString(to)
	cInput := C.CString(string(input))
	cAbi := C.CString(abi)
	defer C.free(unsafe.Pointer(cTo))
	defer C.free(unsafe.Pointer(cInput))
	defer C.free(unsafe.Pointer(cAbi))
	inputHex := hex.EncodeToString(input)
	cInputHex := C.CString(inputHex)
	defer C.free(unsafe.Pointer(cInputHex))
	encodedTransactionPointer := C.bcos_sdk_create_transaction_data(csdk.groupID, csdk.chainID, cTo, cInputHex, cAbi, C.int64_t(blockLimit))
	defer C.bcos_sdk_destroy_transaction_data(encodedTransactionPointer)
	if C.bcos_sdk_is_last_opr_success() == 0 {
		return nil, nil, fmt.Errorf("bcos_sdk_create_transaction_data error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	encodedTransactionData := C.bcos_sdk_encode_transaction_data(encodedTransactionPointer)
	defer C.bcos_sdk_c_free(unsafe.Pointer(encodedTransactionData))
	if C.bcos_sdk_is_last_opr_success() == 0 {
		return nil, nil, fmt.Errorf("bcos_sdk_create_transaction_data encode error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	data, err := hex.DecodeString(strings.TrimPrefix(C.GoString(encodedTransactionData), "0x"))
	if err != nil {
		return nil, nil, err
	}
	cryptoType := C.int(0)
	if csdk.smCrypto {
		cryptoType = C.int(1)
	}
	dataHashHex := C.bcos_sdk_calc_transaction_data_hash(cryptoType, encodedTransactionPointer)
	defer C.bcos_sdk_c_free(unsafe.Pointer(dataHashHex))
	dataHash, err := hex.DecodeString(strings.TrimPrefix(C.GoString(dataHashHex), "0x"))
	if err != nil {
		return nil, nil, err
	}
	if C.bcos_sdk_is_last_opr_success() == 0 {
		return nil, nil, fmt.Errorf("bcos_sdk_create_transaction_data hash error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	return data, dataHash, nil
}

// CreateEncodedTransactionDataV1WithNonceV1Data uses bcos_sdk_create_transaction_v1_data to build
// encoded transaction data with explicit nonce.
func (csdk *CSDK) CreateEncodedTransactionDataV1WithNonceV1Data(blockLimit int64, to string, input []byte, abi string, nonce string, value string, gasPrice string) ([]byte, []byte, error) {
	cTo := C.CString(to)
	cNonce := C.CString(nonce)
	cAbi := C.CString(abi)
	defer C.free(unsafe.Pointer(cTo))
	defer C.free(unsafe.Pointer(cNonce))
	defer C.free(unsafe.Pointer(cAbi))

	var inputPtr unsafe.Pointer
	if len(input) > 0 {
		inputPtr = unsafe.Pointer(&input[0])
	} else {
		// Avoid passing non-nil pointer for empty input; createTransactionDataV1 expects input bytes.
		inputPtr = unsafe.Pointer(&[]byte{0}[0])
	}

	cValue := C.CString(value)
	cGasPrice := C.CString(gasPrice)
	defer C.free(unsafe.Pointer(cValue))
	defer C.free(unsafe.Pointer(cGasPrice))

	encodedTransactionPointer := C.bcos_sdk_create_transaction_v1_data(
		csdk.groupID,
		csdk.chainID,
		cTo,
		cNonce,
		(*C.uchar)(inputPtr),
		C.long(len(input)),
		cAbi,
		C.int64_t(blockLimit),
		cValue,
		cGasPrice,
		C.int64_t(0), // gas_limit >= 0; 0 acts as placeholder for builder.
	)
	defer C.bcos_sdk_destroy_transaction_data(encodedTransactionPointer)

	if C.bcos_sdk_is_last_opr_success() == 0 {
		return nil, nil, fmt.Errorf("bcos_sdk_create_transaction_v1_data error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}

	encodedTransactionData := C.bcos_sdk_encode_transaction_data(encodedTransactionPointer)
	defer C.bcos_sdk_c_free(unsafe.Pointer(encodedTransactionData))
	if C.bcos_sdk_is_last_opr_success() == 0 {
		return nil, nil, fmt.Errorf("bcos_sdk_create_transaction_v1_data encode error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}

	data, err := hex.DecodeString(strings.TrimPrefix(C.GoString(encodedTransactionData), "0x"))
	if err != nil {
		return nil, nil, err
	}

	cryptoType := C.int(0)
	if csdk.smCrypto {
		cryptoType = C.int(1)
	}
	dataHashHex := C.bcos_sdk_calc_transaction_data_hash(cryptoType, encodedTransactionPointer)
	defer C.bcos_sdk_c_free(unsafe.Pointer(dataHashHex))
	if C.bcos_sdk_is_last_opr_success() == 0 {
		return nil, nil, fmt.Errorf("bcos_sdk_calc_transaction_data_hash error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	dataHash, err := hex.DecodeString(strings.TrimPrefix(C.GoString(dataHashHex), "0x"))
	if err != nil {
		return nil, nil, err
	}

	return data, dataHash, nil
}

func (csdk *CSDK) CreateEncodedSignature(hash []byte) ([]byte, error) {
	hexHash := hex.EncodeToString(hash)
	cHexHash := C.CString(hexHash)
	defer C.free(unsafe.Pointer(cHexHash))
	csdk.keyPairMutex.RLock()
	defer csdk.keyPairMutex.RUnlock()
	signatureHex := C.bcos_sdk_sign_transaction_data_hash(csdk.keyPair, cHexHash)
	defer C.bcos_sdk_c_free(unsafe.Pointer(signatureHex))
	if C.bcos_sdk_is_last_opr_success() == 0 {
		return nil, fmt.Errorf("bcos_sdk_sign_transaction_data_hash error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	signatureBytes, err := hex.DecodeString(strings.TrimPrefix(C.GoString(signatureHex), "0x"))
	if err != nil {
		return nil, err
	}
	return signatureBytes, nil
}

// KeyPairAddress returns the address of the current default keypair.
func (csdk *CSDK) KeyPairAddress() (string, error) {
	csdk.keyPairMutex.RLock()
	defer csdk.keyPairMutex.RUnlock()
	if csdk.keyPair == nil {
		return "", fmt.Errorf("key pair is nil")
	}
	addr := C.bcos_sdk_get_keypair_address(csdk.keyPair)
	if addr == nil {
		return "", fmt.Errorf("bcos_sdk_get_keypair_address failed: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	return C.GoString(addr), nil
}

// KeyPairPublicKey returns the hex public key of the current default keypair (no 0x prefix).
func (csdk *CSDK) KeyPairPublicKey() (string, error) {
	csdk.keyPairMutex.RLock()
	defer csdk.keyPairMutex.RUnlock()
	if csdk.keyPair == nil {
		return "", fmt.Errorf("key pair is nil")
	}
	pk := C.bcos_sdk_get_keypair_public_key(csdk.keyPair)
	if pk == nil {
		return "", fmt.Errorf("bcos_sdk_get_keypair_public_key failed: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	return strings.TrimPrefix(C.GoString(pk), "0x"), nil
}

func (csdk *CSDK) CreateEncodedTransaction(transactionData, dataHash, signature []byte, attribute int32, extraData string) ([]byte, error) {
	cExtraData := C.CString("")
	if len(extraData) != 0 {
		cExtraData := C.CString(extraData)
		defer C.free(unsafe.Pointer(cExtraData))
	}
	cDataHash := C.CString(hex.EncodeToString(dataHash))
	defer C.free(unsafe.Pointer(cDataHash))
	cSignature := C.CString(hex.EncodeToString(signature))
	defer C.free(unsafe.Pointer(cSignature))
	cTransactionData := C.CString(hex.EncodeToString(transactionData))
	defer C.free(unsafe.Pointer(cTransactionData))
	cJson := C.bcos_sdk_decode_transaction_data(cTransactionData)
	defer C.bcos_sdk_c_free(unsafe.Pointer(cJson))
	txDataPointer := C.bcos_sdk_create_transaction_data_with_json(cJson)
	defer C.bcos_sdk_destroy_transaction_data(txDataPointer)
	encodedTransactionHex := C.bcos_sdk_create_signed_transaction_with_signed_data_ver_extra_data(txDataPointer, cSignature, cDataHash, C.int(attribute), cExtraData)
	if C.bcos_sdk_is_last_opr_success() == 0 {
		return nil, fmt.Errorf("bcos_sdk_create_transaction_data_from_tx_data error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	data, err := hex.DecodeString(strings.TrimPrefix(C.GoString(encodedTransactionHex), "0x"))
	if err != nil {
		return nil, err
	}
	return data, nil
}

func (csdk *CSDK) SendEncodedTransaction(chanData *CallbackChan, encodedTransaction []byte, withProof bool) error {
	cEncodedTransaction := C.CString(hex.EncodeToString(encodedTransaction))
	defer C.free(unsafe.Pointer(cEncodedTransaction))
	cProof := C.int(0)
	if withProof {
		cProof = C.int(1)
	}
	C.bcos_rpc_send_transaction(csdk.sdk, csdk.groupID, nil, cEncodedTransaction, cProof, C.bcos_sdk_c_struct_response_cb(C.on_recv_resp_callback), setContext(chanData))
	if C.bcos_sdk_is_last_opr_success() == 0 {
		return fmt.Errorf("bcos_rpc_send_raw_transaction error: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	return nil
}

// NewSignOnlyCSDK builds a CSDK that can sign/verify without a live RPC connection.
// Use CloseSignOnly to free resources (not Close, which expects a network SDK).
func NewSignOnlyCSDK(smCrypto bool, privateKey []byte, groupID, chainID string) (*CSDK, error) {
	if len(privateKey) != 32 {
		return nil, fmt.Errorf("private key must be 32 bytes")
	}
	cryptoType := C_SDK_ECDSA_CRYPTO
	if smCrypto {
		cryptoType = C_SDK_SM_CRYPTO
	}
	keyPair := C.bcos_sdk_create_keypair_by_private_key(cryptoType, unsafe.Pointer(&privateKey[0]), C.uint(len(privateKey)))
	if keyPair == nil {
		return nil, fmt.Errorf("bcos_sdk_create_keypair_by_private_key: %s", C.GoString(C.bcos_sdk_get_last_error_msg()))
	}
	return &CSDK{
		smCrypto:        smCrypto,
		groupID:         C.CString(groupID),
		chainID:         C.CString(chainID),
		keyPair:         keyPair,
		privateKeyBytes: append([]byte(nil), privateKey...),
	}, nil
}

// CloseSignOnly releases keypair/strings created by NewSignOnlyCSDK.
func (csdk *CSDK) CloseSignOnly() {
	if csdk == nil {
		return
	}
	csdk.keyPairMutex.Lock()
	defer csdk.keyPairMutex.Unlock()
	if csdk.keyPair != nil {
		C.bcos_sdk_destroy_keypair(csdk.keyPair)
		csdk.keyPair = nil
	}
	if csdk.groupID != nil {
		C.free(unsafe.Pointer(csdk.groupID))
		csdk.groupID = nil
	}
	if csdk.chainID != nil {
		C.free(unsafe.Pointer(csdk.chainID))
		csdk.chainID = nil
	}
}

