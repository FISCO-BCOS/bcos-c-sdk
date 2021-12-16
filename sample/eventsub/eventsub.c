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
 * @file eventsub.c
 * @author: octopus
 * @date 2021-12-14
 */

#include <bcos-c-sdk/bcos_sdk_c.h>
#include <bcos-c-sdk/bcos_sdk_c_common.h>
#include <bcos-c-sdk/bcos_sdk_c_error.h>
#include <bcos-c-sdk/bcos_sdk_c_event_sub.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void usage()
{
    printf("Desc: subscribe contract events by command params\n");
    printf("Usage: eventsub <config> <group> <from> <to> <address>[Optional]\n");
    printf("Example:\n");
    printf("    ./eventsub ./config_sample.ini group -1 -1\n");
    exit(0);
}

// callback for bcos_event_sub_subscribe_event
void on_event_sub_callback(struct bcos_sdk_c_struct_response* resp)
{
    if (resp->error != 0)
    {
        printf("\t something is wrong, error: %d, errorMessage: %s\n", resp->error, resp->desc);
        exit(-1);
    }
    else
    {
        printf(" \t recv events from server ===>>>> events: %s\n", (char*)resp->data);
    }
}

int main(int argc, char** argv)
{
    if (argc < 5)
    {
        usage();
    }

    // option params
    const char* config = argv[1];
    const char* group = argv[2];
    int from = atoi(argv[3]);
    int to = atoi(argv[4]);

    const char* address = "";
    if (argc > 5)
    {
        address = argv[5];
    }

    printf(" [EventSub] params ===>>>> \n");
    printf(" \t # config: %s\n", config);
    printf(" \t # group: %s\n", group);
    printf(" \t # from: %d\n", from);
    printf(" \t # to: %d\n", to);
    printf(" \t # address: %s\n", address);

    void* sdk = bcos_sdk_create_by_config_file(config);
    // check success or not
    int error = bcos_sdk_get_last_error();
    if (error != 0)
    {
        printf(" bcos_sdk_create_by_config_file failed, error: %d, errorMessage: %s\n", error,
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf("[EventSub] start sdk...\n");
    bcos_sdk_start(sdk);
    // check success or not
    error = bcos_sdk_get_last_error();
    if (error != 0)
    {
        printf(" bcos_sdk_start failed, error: %d, errorMessage: %s\n", error,
            bcos_sdk_get_last_error_msg());
        exit(-1);
    }

    printf("[EventSub] start sdk ... \n");

    char params[1024] = {0};
    snprintf(params, sizeof(params),
        "{\"addresses\":["
        "\"%s\"],\"fromBlock\":%d,\"toBlock\":%d,"
        "\"topics\":[]}",
        address, from, to);

    printf("[EventSub] params: %s\n", params);

    bcos_event_sub_subscribe_event(sdk, group, params, on_event_sub_callback, sdk);

    int i = 0;
    while (1)
    {
        printf(" Main thread running ");
        sleep(10);
        i++;
    }

    return EXIT_SUCCESS;
}
