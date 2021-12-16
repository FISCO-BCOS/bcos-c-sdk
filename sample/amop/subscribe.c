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
 * @file subscribe.c
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
void usage()
{
    printf("Desc: subscribe amop topic by command params\n");
    printf("Usage: subscribe <config> <topic>\n");
    printf("Example:\n");
    printf("    ./subscribe ./config_sample.ini topic\n");
    exit(0);
}

// callback for bcos_amop_subscribe_topic_with_cb
void on_recv_amop_subscribe_resp(
    const char* endpoint, const char* seq, struct bcos_sdk_c_struct_response* resp)
{
    if (resp->error != 0)
    {
        printf("\t something is wrong, error: %d, errorMessage: %s\n", resp->error, resp->desc);
    }
    else
    {
        printf(
            "\t recv message and would echo message to publish, endpoint: %s, seq: %s, msg: %s\n",
            endpoint, seq, (char*)resp->data);

        bcos_amop_send_response(resp->context, endpoint, seq, resp->data, resp->size);
    }
}

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        usage();
    }

    const char* config = argv[1];
    const char* topic = argv[2];

    printf(" [AMOP][Subscribe] params ===>>>> \n");
    printf(" \t config: %s\n", config);
    printf(" \t topic: %s\n", topic);

    void* sdk = bcos_sdk_create_by_config_file(config);
    int error = bcos_sdk_get_last_error();
    if (error != 0)
    {
        printf(" bcos_sdk_create_by_config_file failed, error: %d, errorMessage: %s\n", error,
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf(" [AMOP][Subscribe] start sdk ... \n");
    bcos_sdk_start(sdk);
    error = bcos_sdk_get_last_error();
    if (error != 0)
    {
        printf(" bcos_sdk_start failed, error: %d, errorMessage: %s\n", error,
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    bcos_amop_subscribe_topic_with_cb(sdk, topic, on_recv_amop_subscribe_resp, sdk);

    int i = 0;
    while (1)
    {
        printf(" Main thread running \n");
        sleep(10);
        i++;
    }

    return EXIT_SUCCESS;
}
