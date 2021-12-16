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
 * @file bcos_sdk_c_common.cpp
 * @author: octopus
 * @date 2021-12-15
 */

#include "bcos_sdk_c_common.h"
#include <bcos-boostssl/utilities/Common.h>
#include <bcos-boostssl/utilities/Error.h>

using namespace bcos;
using namespace bcos::boostssl;
using namespace bcos::boostssl::utilities;
using namespace bcos::boostssl::utilities::protocol;

struct bcos_sdk_c_config* bcos_sdk_c_config_create_empty()
{
    struct bcos_sdk_c_config* config =
        (struct bcos_sdk_c_config*)malloc(sizeof(struct bcos_sdk_c_config));
    config->thread_pool_size = -1;
    config->message_timeout_ms = -1;
    config->heartbeat_period_ms = -1;
    config->reconnect_period_ms = -1;
    config->disable_ssl = 0;
    config->cert_config = NULL;
    config->sm_cert_config = NULL;
    config->peers = NULL;
    config->peers_count = 0;

    return config;
}

void bcos_sdk_c_cert_config_destroy(void* p)
{
    if (p == NULL)
    {
        return;
    }

    struct bcos_sdk_c_cert_config* config = (struct bcos_sdk_c_cert_config*)p;
    if (config && config->ca_cert)
    {
        free(config->ca_cert);
    }

    if (config && config->node_cert)
    {
        free(config->node_cert);
    }

    if (config && config->node_key)
    {
        free(config->node_key);
    }

    free(config);
}

void bcos_sdk_c_sm_cert_config_destroy(void* p)
{
    if (p == NULL)
    {
        return;
    }

    struct bcos_sdk_c_sm_cert_config* config = (struct bcos_sdk_c_sm_cert_config*)p;
    if (config && config->ca_cert)
    {
        free(config->ca_cert);
    }

    if (config && config->node_cert)
    {
        free(config->node_cert);
    }

    if (config && config->node_key)
    {
        free(config->node_key);
    }

    if (config && config->en_node_key)
    {
        free(config->en_node_key);
    }

    if (config && config->en_node_cert)
    {
        free(config->en_node_cert);
    }

    free(config);
}

void bcos_sdk_c_config_destroy(void* p)
{
    if (p == NULL)
    {
        return;
    }

    struct bcos_sdk_c_config* config = (struct bcos_sdk_c_config*)p;

    bcos_sdk_c_cert_config_destroy(config->cert_config);
    bcos_sdk_c_sm_cert_config_destroy(config->sm_cert_config);

    if (config->peers && config->peers_count > 0)
    {
        for (size_t i = 0; i < config->peers_count; i++)
        {
            free((void*)config->peers[i].host);
        }
    }

    free((void*)config->peers);
    free((void*)config);
}

void bcos_sdk_c_handle_response(
    void* error, void* data, size_t size, bcos_sdk_c_struct_response_cb callback, void* context)
{
    // auto resp = new bcos_sdk_c_struct_response();
    bcos_sdk_c_struct_response temp_resp;
    auto resp = &temp_resp;
    resp->context = context;

    auto errorPtr = (Error*)error;
    if (errorPtr && errorPtr->errorCode() != CommonError::SUCCESS)
    {
        resp->error = errorPtr->errorCode();
        resp->desc = (char*)errorPtr->errorMessage().c_str();
        resp->data = NULL;
        resp->size = 0;
    }
    else
    {
        resp->error = CommonError::SUCCESS;
        resp->desc = NULL;
        resp->data = data ? (byte*)data : NULL;
        resp->size = size;
    }

    callback(resp);
}