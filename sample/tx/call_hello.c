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
#include <bcos-c-sdk/bcos_sdk_c_common.h>
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
    printf("Usage: call_hello <config> <group_id> <contract address>\n");
    printf("Example:\n");
    printf(
        "    ./call_hello ./config_sample.ini group0 713aB8e2926bCE2CF29efB0Dd9A82F02BCFc96e6\n");
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
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(
            " bcos_sdk_create_by_config_file failed, error: %s\n", bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf(" [CallHello] start sdk ... \n");
    bcos_sdk_start(sdk);
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(" [CallHello] bcos_sdk_start failed, error: %s\n", bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    int sm_crypto = 0;
    int wasm = 0;

    bcos_sdk_get_group_wasm_and_crypto(sdk, group_id, &wasm, &sm_crypto);
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(" [CallHello] bcos_sdk_group_sm_crypto failed, error: %s\n",
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf(" [CallHello] wasm: %d, sm crypto: %d\n", wasm, sm_crypto);

    const char* chain_id = bcos_sdk_get_group_chain_id(sdk, group_id);
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(" [CallHello] bcos_sdk_get_group_chain_id failed, error: %s\n",
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf(" [CallHello] chain id: %s\n", chain_id);

    long long block_limit = bcos_rpc_get_block_limit(sdk, group_id);
    if (block_limit < 0)
    {
        printf(" [CallHello] group not exist, group: %s\n", group_id);
        exit(-1);
    }

    printf(" [CallHello] block limit: %lld\n", block_limit);

    void* key_pair = bcos_sdk_create_keypair(sm_crypto);
    if (!key_pair)
    {
        printf(" [CallHello] create keypair failed, error: %s\n", bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf(" [CallHello] bcos_sdk_get_keypair_type: %d\n", bcos_sdk_get_keypair_type(key_pair));

    const char* account = bcos_sdk_get_keypair_address(key_pair);
    printf(" [CallHello] new account, address: %s\n", account);

    void* transaction_data = bcos_sdk_create_transaction_data(
        group_id, chain_id, address, getSetData(sm_crypto), "", block_limit);
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(" [CallHello] bcos_sdk_create_transaction_data failed, error: %s\n",
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }
    printf(" [CallHello] create transaction data\n");

    const char* transaction_data_hash =
        bcos_sdk_calc_transaction_data_hash(sm_crypto ? 1 : 0, transaction_data);
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(" [CallHello] bcos_sdk_calc_transaction_data_hash failed, error: %s\n",
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }
    printf(" [CallHello] calc transaction data hash, hash: %s\n", transaction_data_hash);

    const char* signed_data = bcos_sdk_sign_transaction_data_hash(key_pair, transaction_data_hash);
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(" [CallHello] bcos_sdk_sign_transaction_data_hash failed, error: %s\n",
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }
    printf(" [CallHello] sign transaction data hash, signed data: %s\n", signed_data);

    const char* signed_tx = bcos_sdk_create_signed_transaction_with_signed_data(
        transaction_data, signed_data, transaction_data_hash, 0);
    if (!bcos_sdk_is_last_opr_success())
    {
        printf(
            " [CallHello] bcos_sdk_create_signed_transaction_with_signed_data failed, error: %s\n",
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }
    printf(" [CallHello] create signed transaction, signed tx: %s\n", signed_tx);

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
    // release transaction data
    bcos_sdk_destroy_transaction_data(transaction_data);

    // free chain_id
    free((void*)chain_id);
    // free account
    free((void*)account);
    // free transaction_data_hash
    free((void*)transaction_data_hash);
    // free signed_data
    free((void*)signed_data);
    // free tx
    free((void*)signed_tx);

    // stop sdk
    bcos_sdk_stop(sdk);
    // release sdk
    bcos_sdk_destroy(sdk);


    return 0;
}
