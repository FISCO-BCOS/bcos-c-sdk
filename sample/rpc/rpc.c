/*
 *  Copyright (C) 2021 FISCO BCOS.
 *  SPDX-License-Identifier: Apache-2.0
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 * @file rpc.c
 * @author: octopus
 * @date 2021-08-24
 */

#include <bcos-c-sdk/bcos_sdk_c.h>
#include <bcos-c-sdk/bcos_sdk_c_common.h>
#include <bcos-c-sdk/bcos_sdk_c_error.h>
#include <bcos-c-sdk/bcos_sdk_c_rpc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void usage()
{
    printf("Desc: rpc methods call test\n");
    printf("Usage: rpc <host> <port> <ssl type> <group>\n");
    printf("Example:\n");
    printf("   ./rpc 127.0.0.1 20200 ssl group\n");
    printf("   ./rpc 127.0.0.1 20200 sm_ssl group\n");
    exit(0);
}

char* my_strdup(const char* s)
{
    size_t len = strlen(s) + 1;
    char* result = (char*)malloc(len);
    if (result == (char*)0)
        return (char*)0;
    return (char*)memcpy(result, s, len);
}

// callback for rpc interfaces
void on_recv_resp_callback(struct bcos_sdk_c_struct_response* resp)
{
    if (resp->error != 0)
    {
        printf("\t something is wrong, error: %d, errorMessage: %s\n", resp->error, resp->desc);
        exit(-1);
    }
    else
    {
        printf(" \t recv rpc resp from server ===>>>> resp: %s\n", (char*)resp->data);
    }
}

struct bcos_sdk_c_config* create_config(int sm_ssl, char* host, int port)
{
    // create c-sdk config object
    struct bcos_sdk_c_config* config = bcos_sdk_c_config_create_empty();
    // set thread pool size
    config->thread_pool_size = 8;
    // set message timeout(unit: ms)
    config->message_timeout_ms = 10000;

    // --- set connected peers ---------
    struct bcos_sdk_c_endpoint* ep =
        (struct bcos_sdk_c_endpoint*)malloc(sizeof(struct bcos_sdk_c_endpoint));
    ep->host = my_strdup(host);
    ep->port = port;

    config->peers = ep;
    config->peers_count = 1;
    // --- set connected peers ---------

    // do not disable ssl
    config->disable_ssl = 0;

    // set ssl type
    config->ssl_type = my_strdup(sm_ssl ? "sm_ssl" : "ssl");

    // --- set ssl cert ---------
    // cert config items is the path of file ,not the content
    config->is_cert_path = 1;

    if (sm_ssl)
    {  // sm ssl connection cert config
        struct bcos_sdk_c_sm_cert_config* sm_cert_config =
            (struct bcos_sdk_c_sm_cert_config*)malloc(sizeof(struct bcos_sdk_c_sm_cert_config));
        sm_cert_config->ca_cert = my_strdup("./conf/sm_ca.crt");
        sm_cert_config->node_cert = my_strdup("./conf/sm_sdk.crt");
        sm_cert_config->node_key = my_strdup("./conf/sm_sdk.key");
        sm_cert_config->en_node_key = my_strdup("./conf/sm_ensdk.key");
        sm_cert_config->en_node_cert = my_strdup("./conf/sm_ensdk.crt");

        config->sm_cert_config = sm_cert_config;
        config->cert_config = NULL;
    }
    else
    {  // ssl connection cert config
        struct bcos_sdk_c_cert_config* cert_config =
            (struct bcos_sdk_c_cert_config*)malloc(sizeof(struct bcos_sdk_c_cert_config));
        cert_config->ca_cert = my_strdup("./conf/ca.crt");
        cert_config->node_cert = my_strdup("./conf/sdk.crt");
        cert_config->node_key = my_strdup("./conf/sdk.key");

        config->sm_cert_config = NULL;
        config->cert_config = cert_config;
    }
    // --- set ssl cert ---------

    return config;
}

int main(int argc, char** argv)
{
    if (argc < 5)
    {
        usage();
    }

    // option params
    const char* host = argv[1];
    int port = atoi(argv[2]);
    const char* type = argv[3];
    const char* group = argv[4];

    printf(" [RPC] params ===>>>> \n");
    printf(" \t # host: %s\n", host);
    printf(" \t # port: %d\n", port);
    printf(" \t # type: %s\n", type);
    printf(" \t # group: %s\n", group);

    int is_sm_ssl = 1;
    char* pos = strstr(type, "sm_ssl");
    if (pos == NULL)
    {
        is_sm_ssl = 0;
    }

    struct bcos_sdk_c_config* config = create_config(is_sm_ssl, (char*)host, port);
    void* sdk = bcos_sdk_create(config);
    // check success or not
    int error = bcos_sdk_get_last_error();
    if (error != 0)
    {
        printf(" bcos_sdk_create_by_config_file failed, error: %d, errorMessage: %s\n", error,
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }
    bcos_sdk_c_config_destroy(config);  // release resource

    printf("[RPC] start sdk...\n");
    bcos_sdk_start(sdk);
    // check success or not
    error = bcos_sdk_get_last_error();
    if (error != 0)
    {
        printf("[RPC] bcos_sdk_start failed, error: %d, errorMessage: %s\n", error,
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    int64_t genesisBlkNumber = 0;

    printf(" # getBlockNumber\n");
    // getBlockNumber
    bcos_rpc_get_block_number(sdk, group, NULL, on_recv_resp_callback, sdk);

    sleep(3);

    // printf(" # sendTransaction\n");
    // sendTransaction
    // bcos_rpc_send_transaction(sdk, group, NULL, "aaaaa", 0, on_recv_resp_callback, sdk);

    // sleep(3);

    // printf(" # call\n");
    // call
    // bcos_rpc_call(sdk, group, NULL, "aaaaa", "aaaaa", on_recv_resp_callback, sdk);

    // sleep(3);

    printf(" # getBlockByNumber\n");
    // getBlockByNumber
    bcos_rpc_get_block_by_number(
        sdk, group, NULL, genesisBlkNumber, 0, 0, on_recv_resp_callback, sdk);

    sleep(3);

    printf(" # getSealerList\n");
    // getSealerList
    bcos_rpc_get_sealer_list(sdk, group, NULL, on_recv_resp_callback, sdk);

    sleep(3);

    printf(" # getObserverList\n");
    // getObserverList
    bcos_rpc_get_observer_list(sdk, group, NULL, on_recv_resp_callback, sdk);

    sleep(3);

    printf(" # getPbftView\n");
    // getPbftView
    bcos_rpc_get_pbft_view(sdk, group, NULL, on_recv_resp_callback, sdk);

    sleep(3);

    printf(" # getPendingTxSize\n");
    // getPendingTxSize
    bcos_rpc_get_pending_tx_size(sdk, group, NULL, on_recv_resp_callback, sdk);

    sleep(3);

    printf(" # getTotalTransactionCount\n");
    // getTotalTransactionCount
    bcos_rpc_get_total_transaction_count(sdk, group, NULL, on_recv_resp_callback, sdk);

    sleep(3);

    printf(" # getGroupList\n");
    // getGroupList
    bcos_rpc_get_group_list(sdk, on_recv_resp_callback, sdk);

    sleep(3);

    printf(" # getGroupInfo\n");
    // getGroupInfo
    bcos_rpc_get_group_info(sdk, group, on_recv_resp_callback, sdk);

    sleep(3);

    printf(" # getGroupInfoList\n");
    // getGroupInfoList
    bcos_rpc_get_group_info_list(sdk, on_recv_resp_callback, sdk);

    sleep(3);

    printf("[RPC] stop sdk...\n");
    bcos_sdk_stop(sdk);
    bcos_sdk_destroy(sdk);

    return EXIT_SUCCESS;
}
