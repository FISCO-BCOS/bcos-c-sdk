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
#include <bcos-utilities/Common.h>
#include <bcos-utilities/Error.h>

using namespace bcos;

/**
 * @brief free char* pointer
 * Note: The *p must be created by malloc or it should have serious bad effects
 *
 * @param p
 */
void bcos_sdk_c_free(void* p)
{
    if (p)
    {
        free(p);
        p = NULL;
    }
}

struct bcos_sdk_c_config* bcos_sdk_c_config_create_empty()
{
    struct bcos_sdk_c_config* config =
        (struct bcos_sdk_c_config*)malloc(sizeof(struct bcos_sdk_c_config));
    config->thread_pool_size = -1;
    config->message_timeout_ms = -1;
    config->heartbeat_period_ms = -1;
    config->reconnect_period_ms = -1;
    config->disable_ssl = 0;
    config->send_rpc_request_to_highest_block_node = 1;
    config->cert_config = NULL;
    config->sm_cert_config = NULL;
    config->peers = NULL;
    config->peers_count = 0;

    return config;
}

char* my_strdup(const char* s)
{
    size_t len = strlen(s) + 1;
    char* result = (char*)malloc(len);
    if (result == (char*)0)
        return (char*)0;
    return (char*)memcpy(result, s, len);
}

struct bcos_sdk_c_config* bcos_sdk_create_config(int sm_ssl, char* host, int port)
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
    //
    config->send_rpc_request_to_highest_block_node = 1;

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

void bcos_sdk_c_cert_config_destroy(void* p)
{
    if (p == NULL)
    {
        return;
    }

    struct bcos_sdk_c_cert_config* config = (struct bcos_sdk_c_cert_config*)p;
    if (config && config->ca_cert)
    {
        bcos_sdk_c_free(config->ca_cert);
    }

    if (config && config->node_cert)
    {
        bcos_sdk_c_free(config->node_cert);
    }

    if (config && config->node_key)
    {
        bcos_sdk_c_free(config->node_key);
    }

    bcos_sdk_c_free(config);
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
        bcos_sdk_c_free(config->ca_cert);
    }

    if (config && config->node_cert)
    {
        bcos_sdk_c_free(config->node_cert);
    }

    if (config && config->node_key)
    {
        bcos_sdk_c_free(config->node_key);
    }

    if (config && config->en_node_key)
    {
        bcos_sdk_c_free(config->en_node_key);
    }

    if (config && config->en_node_cert)
    {
        bcos_sdk_c_free(config->en_node_cert);
    }

    bcos_sdk_c_free(config);
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
            bcos_sdk_c_free((void*)config->peers[i].host);
        }
    }

    bcos_sdk_c_free((void*)config->peers);
    bcos_sdk_c_free((void*)config);
}

void bcos_sdk_c_handle_response(
    void* error, void* data, size_t size, bcos_sdk_c_struct_response_cb callback, void* context)
{
    if (!callback)
    {
        return;
    }
    // auto resp = new bcos_sdk_c_struct_response();
    bcos_sdk_c_struct_response temp_resp;
    auto resp = &temp_resp;
    resp->context = context;

    auto errorPtr = (Error*)error;
    if (errorPtr && errorPtr->errorCode() != 0)
    {
        resp->error = errorPtr->errorCode();
        resp->desc = (char*)errorPtr->errorMessage().c_str();
        resp->data = NULL;
        resp->size = 0;
    }
    else
    {
        resp->error = 0;
        resp->desc = NULL;
        resp->data = data ? (byte*)data : NULL;
        resp->size = size;
    }

    callback(resp);
}
