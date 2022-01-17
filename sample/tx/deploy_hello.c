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
 * @file deploy_hello.c
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

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// HelloWorld Source Code:
/**
pragma solidity ^0.4.25;

contract HelloWorld{
    string public name;
    constructor() public{
       name = "Hello, World!";
    }

    function set(string n) public{
        name = n;
    }
}
*/
const static char* hwBIN =
    "608060405234801561001057600080fd5b506040805190810160405280600d81526020017f48656c6c6f2c20576f72"
    "6c6421000000000000000000000000000000000000008152506000908051906020019061005c929190610062565b50"
    "610107565b828054600181600116156101000203166002900490600052602060002090601f01602090048101928260"
    "1f106100a357805160ff19168380011785556100d1565b828001600101855582156100d1579182015b828111156100"
    "d05782518255916020019190600101906100b5565b5b5090506100de91906100e2565b5090565b61010491905b8082"
    "11156101005760008160009055506001016100e8565b5090565b90565b6102d7806101166000396000f30060806040"
    "526004361061004c576000357c0100000000000000000000000000000000000000000000000000000000900463ffff"
    "ffff1680634ed3885e146100515780636d4ce63c146100ba575b600080fd5b34801561005d57600080fd5b506100b8"
    "600480360381019080803590602001908201803590602001908080601f016020809104026020016040519081016040"
    "528093929190818152602001838380828437820191505050505050919291929050505061014a565b005b3480156100"
    "c657600080fd5b506100cf610164565b60405180806020018281038252838181518152602001915080519060200190"
    "80838360005b8381101561010f5780820151818401526020810190506100f4565b50505050905090810190601f1680"
    "1561013c5780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b8060"
    "009080519060200190610160929190610206565b5050565b6060600080546001816001161561010002031660029004"
    "80601f0160208091040260200160405190810160405280929190818152602001828054600181600116156101000203"
    "166002900480156101fc5780601f106101d1576101008083540402835291602001916101fc565b8201919060005260"
    "20600020905b8154815290600101906020018083116101df57829003601f168201915b5050505050905090565b8280"
    "54600181600116156101000203166002900490600052602060002090601f016020900481019282601f106102475780"
    "5160ff1916838001178555610275565b82800160010185558215610275579182015b82811115610274578251825591"
    "602001919060010190610259565b5b5090506102829190610286565b5090565b6102a891905b808211156102a45760"
    "0081600090555060010161028c565b5090565b905600a165627a7a7230582035f32cd11f4fdc4d12abab1850ea5b40"
    "1f853d9ef91e913e4e82d34efa18a2210029";

const static char* hwSmBIN =
    "608060405234801561001057600080fd5b506040805190810160405280600d81526020017f48656c6c6f2c20576f72"
    "6c6421000000000000000000000000000000000000008152506000908051906020019061005c929190610062565b50"
    "610107565b828054600181600116156101000203166002900490600052602060002090601f01602090048101928260"
    "1f106100a357805160ff19168380011785556100d1565b828001600101855582156100d1579182015b828111156100"
    "d05782518255916020019190600101906100b5565b5b5090506100de91906100e2565b5090565b61010491905b8082"
    "11156101005760008160009055506001016100e8565b5090565b90565b6102d7806101166000396000f30060806040"
    "526004361061004c576000357c0100000000000000000000000000000000000000000000000000000000900463ffff"
    "ffff168063299f7f9d146100515780633590b49f146100e1575b600080fd5b34801561005d57600080fd5b50610066"
    "61014a565b6040518080602001828103825283818151815260200191508051906020019080838360005b8381101561"
    "00a657808201518184015260208101905061008b565b50505050905090810190601f1680156100d357808203805160"
    "01836020036101000a031916815260200191505b509250505060405180910390f35b3480156100ed57600080fd5b50"
    "610148600480360381019080803590602001908201803590602001908080601f016020809104026020016040519081"
    "01604052809392919081815260200183838082843782019150505050505091929192905050506101ec565b005b6060"
    "60008054600181600116156101000203166002900480601f0160208091040260200160405190810160405280929190"
    "818152602001828054600181600116156101000203166002900480156101e25780601f106101b75761010080835404"
    "02835291602001916101e2565b820191906000526020600020905b8154815290600101906020018083116101c55782"
    "9003601f168201915b5050505050905090565b8060009080519060200190610202929190610206565b5050565b8280"
    "54600181600116156101000203166002900490600052602060002090601f016020900481019282601f106102475780"
    "5160ff1916838001178555610275565b82800160010185558215610275579182015b82811115610274578251825591"
    "602001919060010190610259565b5b5090506102829190610286565b5090565b6102a891905b808211156102a45760"
    "0081600090555060010161028c565b5090565b905600a165627a7a72305820ddb4a8b4c62e003ac9d6ca2396325dce"
    "a3b9441c5d2af668d4ccb883a9af271b0029";

const char* getBinary(int _sm)
{
    return _sm ? hwSmBIN : hwBIN;
}

void usage()
{
    printf("Desc: deploy HelloWorld contract\n");
    printf("Usage: deploy_hello <config> <groupID> <chain_id> <SM>\n");
    printf("Example:\n");
    printf("    ./deploy_hello ./config_sample.ini group0 chain0 false\n");
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
    if (argc < 5)
    {
        usage();
    }

    const char* config = argv[1];
    const char* group_id = argv[2];
    const char* chain_id = argv[3];
    int sm = (strstr(argv[4], "true") != NULL);

    printf("[DeployHello] params ===>>>> \n");
    printf("\t # config: %s\n", config);
    printf("\t # group_id: %s\n", group_id);
    printf("\t # chain_id: %s\n", chain_id);
    printf("\t # SM: %d\n", sm);

    void* sdk = bcos_sdk_create_by_config_file(config);
    // check success or not
    int error = bcos_sdk_get_last_error();
    if (error != 0)
    {
        printf(" bcos_sdk_create_by_config_file failed, error: %d, errorMessage: %s\n", error,
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf(" [DeployHello] start sdk ... \n");
    bcos_sdk_start(sdk);
    error = bcos_sdk_get_last_error();
    if (error != 0)
    {
        printf(" [DeployHello] bcos_sdk_start failed, error: %d, errorMessage: %s\n", error,
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    int64_t block_limit = bcos_rpc_get_block_limit(sdk, group_id);
    if (block_limit < 0)
    {
        printf(" [DeployHello] group not exist, group: %s\n", group_id);
        exit(-1);
    }

    printf(" [DeployHello] block limit: %lld\n", block_limit);

    void* key_pair = bcos_sdk_create_keypair(sm ? 2 : 1);
    if (!key_pair)
    {
        printf(" [DeployHello] create keypair failed, error: %s\n", bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    const char* address = bcos_sdk_get_keypair_address(key_pair);
    printf(" [DeployHello] new account, address: %s\n", address);

    const char* signed_tx =
        bcos_sdk_create_signed_tx(key_pair, "", getBinary(sm), chain_id, group_id, block_limit);

    if (signed_tx == NULL)
    {
        printf(
            " [DeployHello] create signed transaction failed, please check the input params. \n");
        exit(-1);
    }

    printf(" [DeployHello] create signed transaction success, %s ", signed_tx);

    bcos_rpc_send_transaction(sdk, group_id, "", signed_tx, 0, on_recv_resp_callback, NULL);

    // wait for async operation done
    sleep(3);

    // release keypair
    bcos_sdk_destroy_keypair(key_pair);

    // free signed_tx
    free((void*)signed_tx);
    // free address
    free((void*)address);

    // stop sdk
    bcos_sdk_stop(sdk);
    // release sdk
    bcos_sdk_destroy(sdk);


    return 0;
}
