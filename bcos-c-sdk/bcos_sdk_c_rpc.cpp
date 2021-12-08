#include "bcos_sdk_c_rpc.h"
#include <bcos-cpp-sdk/Sdk.h>
#include <bcos-cpp-sdk/rpc/JsonRpcImpl.h>

#include <memory>
#include <tuple>

using namespace bcos;
using namespace bcos::boostssl;
using namespace bcos::boostssl::utilities;
using namespace bcos::boostssl::utilities::protocol;

// ------------------------common send message interface begin----------------
// send message to rpc server
void bcos_rpc_generic_method_call(
    void* sdk, const char* data, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->genericMethod(data, [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
        bcos_sdk_c_handle_response(error ? error.get() : NULL, resp ? (void*)resp->data() : NULL,
            resp ? resp->size() : 0, callback, context);
    });
}

// send message to group
void bcos_rpc_generic_method_call_to_group(void* sdk, const char* group, const char* data,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->genericMethod(
        group, data, [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// send message to group of node
void bcos_rpc_generic_method_call_to_group_node(void* sdk, const char* group, const char* node,
    const char* data, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->genericMethod(group, node ? node : "", data,
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}
// ------------------------common send message interface end----------------

// ------------------------------rpc interface begin -------------------------
// call
void bcos_rpc_call(void* sdk, const char* group, const char* node, const char* to, const char* data,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->call(group, node ? node : "", to, data,
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// sendTransaction
void bcos_rpc_send_transaction(void* sdk, const char* group, const char* node, const char* data,
    int proof, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->sendTransaction(group, node ? node : "", data, proof,
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getTransaction
void bcos_rpc_get_transaction(void* sdk, const char* group, const char* node, const char* tx_hash,
    int proof, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getTransaction(group, node ? node : "", tx_hash, proof,
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getTransactionReceipt
void bcos_rpc_get_transaction_receipt(void* sdk, const char* group, const char* node,
    const char* tx_hash, int proof, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getTransactionReceipt(group, node ? node : "", tx_hash, proof,
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getBlockByHash
void bcos_rpc_get_block_by_hash(void* sdk, const char* group, const char* node,
    const char* block_hash, int only_header, int only_tx_hash,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getBlockByHash(group, node ? node : "", block_hash, only_header, only_tx_hash,
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getBlockByNumber
void bcos_rpc_get_block_by_number(void* sdk, const char* group, const char* node,
    int64_t block_number, int only_header, int only_tx_hash, bcos_sdk_c_struct_response_cb callback,
    void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getBlockByNumber(group, node ? node : "", block_number, only_header, only_tx_hash,
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getBlockHashByNumber
void bcos_rpc_get_block_hash_by_number(void* sdk, const char* group, const char* node,
    int64_t block_number, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getBlockHashByNumber(group, node ? node : "", block_number,
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getBlockLimit
int64_t bcos_rpc_get_block_limit(void* sdk, const char* group)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();
    return rpc->getBlockLimit(group);
}

// getBlockNumber
void bcos_rpc_get_block_number(void* sdk, const char* group, const char* node,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();
    rpc->getBlockNumber(group, node ? node : "",
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getCode
void bcos_rpc_get_code(void* sdk, const char* group, const char* node, const char* address,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getCode(group, node ? node : "", address,
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getSealerList
void bcos_rpc_get_sealer_list(void* sdk, const char* group, const char* node,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getSealerList(group, node ? node : "",
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getObserverList
void bcos_rpc_get_observer_list(void* sdk, const char* group, const char* node,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getObserverList(group, node ? node : "",
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getPbftView
void bcos_rpc_get_pbft_view(void* sdk, const char* group, const char* node,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getPbftView(group, node ? node : "",
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getPendingTxSize
void bcos_rpc_get_pending_tx_size(void* sdk, const char* group, const char* node,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getPendingTxSize(group, node ? node : "",
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getSyncStatus
void bcos_rpc_get_sync_status(void* sdk, const char* group, const char* node,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getSyncStatus(group, node ? node : "",
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getConsensusStatus
void bcos_rpc_get_consensus_status(void* sdk, const char* group, const char* node,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getConsensusStatus(group, node ? node : "",
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getSystemConfigByKey
void bcos_rpc_get_system_config_by_key(void* sdk, const char* group, const char* node,
    const char* key, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getSystemConfigByKey(group, node ? node : "", key,
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getTotalTransactionCount
void bcos_rpc_get_total_transaction_count(void* sdk, const char* group, const char* node,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getTotalTransactionCount(group, node ? node : "",
        [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// getGroupPeers
void bcos_rpc_get_group_peers(
    void* sdk, const char* group, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getGroupPeers(group, [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
        bcos_sdk_c_handle_response(error ? error.get() : NULL, resp ? (void*)resp->data() : NULL,
            resp ? resp->size() : 0, callback, context);
    });
}

// getPeers
void bcos_rpc_get_peers(void* sdk, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getPeers([callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
        bcos_sdk_c_handle_response(error ? error.get() : NULL, resp ? (void*)resp->data() : NULL,
            resp ? resp->size() : 0, callback, context);
    });
}

// getGroupList
void bcos_rpc_get_group_list(void* sdk, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getGroupList([callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
        bcos_sdk_c_handle_response(error ? error.get() : NULL, resp ? (void*)resp->data() : NULL,
            resp ? resp->size() : 0, callback, context);
    });
}

// getGroupInfo
void bcos_rpc_get_group_info(
    void* sdk, const char* group, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getGroupInfo(group, [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
        bcos_sdk_c_handle_response(error ? error.get() : NULL, resp ? (void*)resp->data() : NULL,
            resp ? resp->size() : 0, callback, context);
    });
}

// getGroupInfoList
void bcos_rpc_get_group_info_list(void* sdk, bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getGroupInfoList([callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
        bcos_sdk_c_handle_response(error ? error.get() : NULL, resp ? (void*)resp->data() : NULL,
            resp ? resp->size() : 0, callback, context);
    });
}

// getGroupNodeInfo
void bcos_rpc_get_group_node_info(void* sdk, const char* group, const char* node,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto rpc = sdkPointer->jsonRpc();

    rpc->getGroupNodeInfo(
        group, node, [callback, context](Error::Ptr error, std::shared_ptr<bytes> resp) {
            bcos_sdk_c_handle_response(error ? error.get() : NULL,
                resp ? (void*)resp->data() : NULL, resp ? resp->size() : 0, callback, context);
        });
}

// ------------------------------rpc interface end -------------------------