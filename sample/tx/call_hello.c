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
 * @file call_hello.c
 * @author: octopus
 * @date 2022-01-17
 */
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_rpc.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
#include <bcos-c-sdk/bcos_sdk_c.h>
#include <bcos-c-sdk/bcos_sdk_c_uti_tx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
{
    "6d4ce63c": "get()",
    "4ed3885e": "set(string)"
}

{
    "299f7f9d": "get()",
    "3590b49f": "set(string)"
}
*/

const char* pszSetFunc[] = {
    "4ed3885e00000000000000000000000000000000000000000000000000000000000000200000000000000000000000"
    "00000000000000000000000000000000000000000a48656c6c6f576f726c6400000000000000000000000000000000"
    "000000000000",
    "3590b49f00000000000000000000000000000000000000000000000000000000000000200000000000000000000000"
    "00000000000000000000000000000000000000000a48656c6c6f576f726c6400000000000000000000000000000000"
    "000000000000"};

const char* pszGetFunc[] = {"6d4ce63c", "3590b49f"};

const char* getSetData(int sm)
{
    return sm ? pszSetFunc[1] : pszSetFunc[0];
}

const char* getGetData(int sm)
{
    return sm ? pszGetFunc[1] : pszGetFunc[0];
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void usage()
{
    printf("Desc: call HelloWorld contract set function\n");
    printf("Usage: call_hello <config> <groupID> <address>\n");
    printf("Example:\n");
    printf("    ./call_hello ./config_sample.ini group0 0x...\n");
    exit(0);
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

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        usage();
    }

    const char* config = argv[1];
    const char* group_id = argv[2];
    const char* address = argv[3];

    printf("[CallHello] params ===>>>> \n");
    printf("\t # config: %s\n", config);
    printf("\t # group_id: %s\n", group_id);
    printf("\t # address: %s\n", address);

    void* sdk = bcos_sdk_create_by_config_file(config);
    // check success or not
    if (bcos_sdk_last_opr_failed())
    {
        printf(
            " bcos_sdk_create_by_config_file failed, error: %s\n", bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf(" [CallHello] start sdk ... \n");
    bcos_sdk_start(sdk);
    if (bcos_sdk_last_opr_failed())
    {
        printf(" [CallHello] bcos_sdk_start failed, error: %s\n", bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    int sm_crypto = bcos_sdk_group_sm_crypto(sdk, group_id);
    if (bcos_sdk_last_opr_failed())
    {
        printf(" [CallHello] bcos_sdk_group_sm_crypto failed, error: %s\n",
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf(" [CallHello] sm crypto: %d\n", sm_crypto);

    const char* chain_id = bcos_sdk_group_chain_id(sdk, group_id);
    if (bcos_sdk_last_opr_failed())
    {
        printf(" [CallHello] bcos_sdk_group_chain_id failed, error: %s\n",
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf(" [CallHello] chain id: %s\n", chain_id);

    int64_t block_limit = bcos_rpc_get_block_limit(sdk, group_id);
    if (block_limit < 0)
    {
        printf(" [CallHello] group not exist, group: %s\n", group_id);
        exit(-1);
    }

    printf(" [CallHello] block limit: %lld\n", block_limit);

    void* key_pair = bcos_sdk_create_keypair(sm_crypto ? 2 : 1);
    if (!key_pair)
    {
        printf(" [CallHello] create keypair failed, error: %s\n", bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    const char* account = bcos_sdk_get_keypair_address(key_pair);
    printf(" [CallHello] new account, address: %s\n", account);

    const char* signed_tx = bcos_sdk_create_signed_tx(
        key_pair, address, getSetData(sm_crypto), chain_id, group_id, block_limit);
    if (signed_tx == NULL)
    {
        printf(" [CallHello] create signed transaction failed, please check the input params. \n");
        exit(-1);
    }

    printf(" [CallHello] create signed transaction. \n");

    printf(" [CallHello] call HelloWorld set function. \n");

    bcos_rpc_send_transaction(sdk, group_id, "", signed_tx, 0, on_recv_resp_callback, NULL);

    // wait for async operation done
    sleep(3);

    printf(" [CallHello] call HelloWorld get function. \n");
    bcos_rpc_call(sdk, group_id, "", address, getGetData(sm_crypto), on_recv_resp_callback, NULL);
    // wait for async operation done
    sleep(3);

    // release keypair
    bcos_sdk_destroy_keypair(key_pair);

    // free chain_id
    free((void*)chain_id);
    // free signed_tx
    free((void*)signed_tx);
    // free account
    free((void*)account);

    // stop sdk
    bcos_sdk_stop(sdk);
    // release sdk
    bcos_sdk_destroy(sdk);


    return 0;
}
