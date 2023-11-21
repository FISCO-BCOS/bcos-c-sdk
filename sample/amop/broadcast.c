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
 * @file broadcast.c
 * @author: octopus
 * @date 2021-12-14
 */

#include <bcos-c-sdk/bcos_sdk_c.h>
#include <bcos-c-sdk/bcos_sdk_c_amop.h>
#include <bcos-c-sdk/bcos_sdk_c_common.h>
#include <bcos-c-sdk/bcos_sdk_c_error.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void usage(void)
{
    printf("Desc: broadcast amop message by command params\n");
    printf("Usage: broadcast <config> <topic> <message>\n");
    printf("Example:\n");
    printf("    ./broadcast ./config_sample.ini topic HelloWorld\n");
    exit(0);
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        usage();
    }

    const char* config = argv[1];
    const char* topic = argv[2];
    const char* msg = argv[3];

    printf(" [AMOP][Broadcast]] params ===>>>> \n");
    printf(" \t config: %s\n", config);
    printf(" \t topic: %s\n", topic);
    printf(" \t message: %s\n", msg);

    void* sdk = bcos_sdk_create_by_config_file(config);
    int error = bcos_sdk_get_last_error();
    if (error != 0)
    {
        printf(" bcos_sdk_create_by_config_file failed, error: %d, errorMessage: %s\n", error,
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    bcos_sdk_start(sdk);
    error = bcos_sdk_get_last_error();
    if (error != 0)
    {
        printf(" bcos_sdk_start failed, error: %d, errorMessage: %s\n", error,
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf(" [AMOP][Broadcast] start sdk ... \n");

    while (1)
    {
        printf("[AMOP][Broadcast] broadcast message, topic: %s, msg: %s\n", topic, msg);
        bcos_amop_broadcast(sdk, topic, (void*)msg, strlen(msg));
        sleep(10);
    }

    return EXIT_SUCCESS;
}
