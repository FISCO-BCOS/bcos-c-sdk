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
 * @file keypair.c
 * @author: octopus
 * @date 2022-01-17
 */

#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include <bcos-c-sdk/bcos_sdk_c.h>
#include <bcos-c-sdk/bcos_sdk_c_uti_keypair.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void usage()
{
    printf("Desc: create or load keypair\n");
    printf("Usage: keypair <pem>\n");
    printf("Example:\n");
    printf("   ./keypair\n");
    printf("   ./keypair node.pem\n");
    exit(0);
}

int main(int argc, char** argv)
{
    const char* pemFile = NULL;
    if (argc > 1)
    {
        pemFile = argv[1];
        void* key_pair = bcos_sdk_load_keypair(pemFile);
        if (key_pair != NULL)
        {
            printf(" => load keypair success, pem: %s\n", pemFile);
            printf(" =>  address: %s\n", bcos_sdk_get_keypair_address(key_pair));
            printf(" =>  pub: %s\n", bcos_sdk_get_keypair_public_key(key_pair));
            printf(" =>  pri: %s\n", bcos_sdk_get_keypair_private_key(key_pair));

            void *key_pair_new = bcos_sdk_create_keypair_by_hex_prikey(bcos_sdk_get_keypair_type(key_pair), bcos_sdk_get_keypair_private_key(key_pair));
            printf(" =>  new key pair address: %s\n", bcos_sdk_get_keypair_address(key_pair_new));
            printf(" =>  new key pair pub: %s\n", bcos_sdk_get_keypair_public_key(key_pair_new));
            printf(" =>  new key pair pri: %s\n", bcos_sdk_get_keypair_private_key(key_pair_new));
        }
        else
        {
            printf(" => load keypair failed\n");
            printf(" =>  error: %s\n", bcos_sdk_get_last_error_msg());
        }
    }
    else
    {
        void* key_pair = bcos_sdk_create_keypair(1);
        if (key_pair != NULL)
        {
            printf(" => create keypair success\n");
            printf(" =>  address: %s\n", bcos_sdk_get_keypair_address(key_pair));
            printf(" =>  pub: %s\n", bcos_sdk_get_keypair_public_key(key_pair));
            printf(" =>  pri: %s\n", bcos_sdk_get_keypair_private_key(key_pair));

            void *key_pair_new = bcos_sdk_create_keypair_by_hex_prikey(bcos_sdk_get_keypair_type(key_pair), bcos_sdk_get_keypair_private_key(key_pair));
            printf(" =>  new key pair address: %s\n", bcos_sdk_get_keypair_address(key_pair_new));
            printf(" =>  new key pair pub: %s\n", bcos_sdk_get_keypair_public_key(key_pair_new));
            printf(" =>  new key pair pri: %s\n", bcos_sdk_get_keypair_private_key(key_pair_new));
        }
        else
        {
            printf(" => create keypair failed\n");
            printf(" =>  error: %s\n", bcos_sdk_get_last_error_msg());
        }
    }


    return EXIT_SUCCESS;
}
