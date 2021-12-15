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
 * @file bcos_sdk_c.cpp
 * @author: octopus
 * @date 2021-12-15
 */

#include "bcos_sdk_c.h"
#include "bcos_sdk_c_error.h"
#include <bcos-boostssl/context/ContextBuilder.h>
#include <bcos-boostssl/utilities/BoostLog.h>
#include <bcos-boostssl/websocket/WsService.h>
#include <bcos-cpp-sdk/Sdk.h>
#include <bcos-cpp-sdk/SdkFactory.h>
#include <cstdio>
#include <exception>
#include <memory>
#include <mutex>

// construct WsConfig obj by struct Config
static std::shared_ptr<bcos::boostssl::ws::WsConfig> initWsConfig(struct bcos_sdk_c_config* config)
{
    // init WsConfig by c Config
    auto wsConfig = std::make_shared<bcos::boostssl::ws::WsConfig>();
    wsConfig->setModel(bcos::boostssl::ws::WsModel::Client);

    auto peers = std::make_shared<bcos::boostssl::ws::EndPoints>();
    for (size_t i = 0; i < config->peers_count; i++)
    {
        bcos::boostssl::ws::EndPoint ep;
        ep.host = config->peers[i].host;
        ep.port = config->peers[i].port;
        peers->push_back(ep);
    }

    wsConfig->setConnectedPeers(peers);
    wsConfig->setDisableSsl(config->disableSsl);
    wsConfig->setThreadPoolSize(config->thread_pool_size);
    wsConfig->setReconnectPeriod(config->reconnect_period_ms);
    wsConfig->setHeartbeatPeriod(config->heartbeat_period_ms);
    wsConfig->setSendMsgTimeout(config->message_timeout_ms);

    if (!config->disableSsl)
    {
        auto contextConfig = std::make_shared<bcos::boostssl::context::ContextConfig>();
        contextConfig->setSslType(config->ssl_type);
        contextConfig->setIsCertPath(config->is_cert_path ? true : false);

        if (contextConfig->sslType() != "sm_ssl")
        {
            bcos::boostssl::context::ContextConfig::CertConfig certConfig;
            certConfig.caCert = config->cert_config->ca_cert;
            certConfig.nodeCert = config->cert_config->node_cert;
            certConfig.nodeKey = config->cert_config->node_key;
            contextConfig->setCertConfig(certConfig);
        }
        else
        {
            bcos::boostssl::context::ContextConfig::SMCertConfig smCertConfig;
            smCertConfig.caCert = config->sm_cert_config->ca_cert;
            smCertConfig.nodeKey = config->sm_cert_config->node_key;
            smCertConfig.nodeCert = config->sm_cert_config->node_cert;
            smCertConfig.enNodeCert = config->sm_cert_config->en_node_cert;
            smCertConfig.enNodeKey = config->sm_cert_config->en_node_key;
            contextConfig->setSmCertConfig(smCertConfig);
        }
        wsConfig->setContextConfig(contextConfig);
    }

    return wsConfig;
}

/**
 * @brief: create bcos sdk object by config object
 *
 * @param config: config for sdk, refer to the bcos_sdk_c_config definition
 * @return void*: pointer to sdk object, to call the sdk interface
 * Note: if a null pointer is returned,bcos_sdk_get_last_error and bcos_sdk_get_last_error_msg can
 * be used to get the error code and error message
 */
void* bcos_sdk_create(struct bcos_sdk_c_config* config)
{
    bcos_sdk_clear_last_error();
    try
    {
        // construct sdk object
        auto factory = std::make_shared<bcos::cppsdk::SdkFactory>();
        auto wsConfig = initWsConfig(config);
        auto sdk = factory->buildSdk(wsConfig);
        auto sdkPointer = sdk.release();
        BCOS_LOG(INFO) << LOG_BADGE("bcos_sdk_create") << LOG_DESC("[NEWOBJ]")
                       << LOG_KV("sdk", sdkPointer);
        return sdkPointer;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_create") << LOG_KV("errorMsg", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

/**
 * @brief: create bcos sdk object by config file
 *
 * @param config_file: config file, refer to sample/config/config_sample.ini for details
 * @return void*: pointer to sdk object, to call the sdk interface
 */
void* bcos_sdk_create_by_config_file(const char* config_file)
{
    bcos_sdk_clear_last_error();
    try
    {
        // construct sdk object
        auto factory = std::make_shared<bcos::cppsdk::SdkFactory>();
        auto sdk = factory->buildSdk(config_file);
        auto sdkPointer = sdk.release();
        BCOS_LOG(INFO) << LOG_BADGE("bcos_sdk_create_by_config_file") << LOG_DESC("[NEWOBJ]")
                       << LOG_KV("sdk", sdkPointer);
        return sdkPointer;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_create_by_config_file")
                        << LOG_KV("configFile", config_file) << LOG_KV("errorMsg", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

/**
 * @brief: start the bcos sdk
 *
 * @param sdk: pointer to sdk, create by bcos_sdk_create or bcos_sdk_create_by_config_file
 * @return:
 * Note: bcos_sdk_get_last_error can be used to check whether the start operation is success or not
 */
void bcos_sdk_start(void* sdk)
{
    bcos_sdk_clear_last_error();
    try
    {
        if (sdk)
        {
            ((bcos::cppsdk::Sdk*)sdk)->start();
        }
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_start") << LOG_KV("sdk", sdk)
                        << LOG_KV("errorMsg", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    BCOS_LOG(INFO) << LOG_BADGE("bcos_sdk_start") << LOG_KV("sdk", sdk);
}

/**
 * @brief: stop the sdk
 *
 * @param sdk
 */
void bcos_sdk_stop(void* sdk)
{
    if (sdk)
    {
        ((bcos::cppsdk::Sdk*)sdk)->stop();
    }

    BCOS_LOG(INFO) << LOG_BADGE("bcos_sdk_stop") << LOG_KV("sdk", sdk);
}

/**
 * @brief: stop and release the resource of sdk
 *
 * @param sdk
 */
void bcos_sdk_destroy(void* sdk)
{
    if (sdk)
    {
        delete (bcos::cppsdk::Sdk*)sdk;
    }

    BCOS_LOG(INFO) << LOG_BADGE("bcos_sdk_destroy") << LOG_KV("sdk", sdk);
}