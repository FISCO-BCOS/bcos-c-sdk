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
#include "include/BuildInfo.h"
#include <bcos-boostssl/context/ContextBuilder.h>
#include <bcos-boostssl/websocket/WsService.h>
#include <bcos-cpp-sdk/Sdk.h>
#include <bcos-cpp-sdk/SdkFactory.h>
#include <bcos-utilities/BoostLog.h>
#include <cstdio>
#include <cstring>
#include <exception>
#include <memory>
#include <mutex>

/**
 * @brief: obtain the c-sdk version information
 *
 * @return const char*
 */
const char* bcos_sdk_version()
{
    std::string version;
    version += "FISCO BCOS C SDK Version : ";
    version += FISCO_BCOS_C_SDK_PROJECT_VERSION;
    version += "\n";
    version += "Build Time         : ";
    version += FISCO_BCOS_C_SDK_BUILD_TIME;
    version += "\n";

    version += "Build Type         : ";
    version += FISCO_BCOS_C_SDK_BUILD_PLATFORM;
    version += "/";
    version += FISCO_BCOS_C_SDK_BUILD_TYPE;
    version += "\n";

    version += "Git Branch         : ";
    version += FISCO_BCOS_C_SDK_BUILD_BRANCH;
    version += "\n";

    version += "Git Commit         : ";
    version += FISCO_BCOS_C_SDK_COMMIT_HASH;
    version += "\n";

    return strdup(version.c_str());
}

// construct WsConfig obj by struct Config
static std::shared_ptr<bcos::boostssl::ws::WsConfig> initWsConfig(struct bcos_sdk_c_config* config)
{
    // TODO: check params
    // init WsConfig by c Config
    auto wsConfig = std::make_shared<bcos::boostssl::ws::WsConfig>();
    wsConfig->setModel(bcos::boostssl::ws::WsModel::Client);

    auto peers = std::make_shared<bcos::boostssl::ws::EndPoints>();
    for (size_t i = 0; i < config->peers_count; i++)
    {
        auto host = config->peers[i].host;
        auto port = config->peers[i].port;
        bcos::boostssl::NodeIPEndpoint endpoint = bcos::boostssl::NodeIPEndpoint(host, port);
        peers->insert(endpoint);
    }

    wsConfig->setConnectPeers(peers);
    wsConfig->setDisableSsl(config->disable_ssl);
    wsConfig->setThreadPoolSize(config->thread_pool_size);
    wsConfig->setReconnectPeriod(config->reconnect_period_ms);
    wsConfig->setHeartbeatPeriod(config->heartbeat_period_ms);
    wsConfig->setSendMsgTimeout(config->message_timeout_ms);

    if (!config->disable_ssl)
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
    BCOS_SDK_C_PARAMS_VERIFICATION(config, NULL);
    try
    {
        // construct sdk object
        auto factory = std::make_shared<bcos::cppsdk::SdkFactory>();
        auto wsConfig = initWsConfig(config);
        auto sdk = factory->buildSdk(wsConfig, config->send_rpc_request_to_highest_block_node);
        auto sdkPointer = sdk.release();

        auto version = bcos_sdk_version();
        BCOS_LOG(INFO) << LOG_BADGE("bcos_sdk_create") << LOG_DESC("[NEWOBJ]")
                       << LOG_KV("sdk", sdkPointer) << LOG_KV("version", version)
                       << LOG_KV("peers_count", config->peers_count)
                       << LOG_KV("disable_ssl", config->disable_ssl)
                       << LOG_KV("message_timeout_ms", config->message_timeout_ms)
                       << LOG_KV("reconnect_period_ms", config->reconnect_period_ms)
                       << LOG_KV("heartbeat_period_ms", config->heartbeat_period_ms)
                       << LOG_KV("send_rpc_request_to_highest_block_node",
                              config->send_rpc_request_to_highest_block_node);
        bcos_sdk_c_free((void*)version);
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
        auto version = bcos_sdk_version();

        BCOS_LOG(INFO) << LOG_BADGE("bcos_sdk_create_by_config_file") << LOG_DESC("[NEWOBJ]")
                       << LOG_KV("sdk", sdkPointer) << LOG_KV("version", version);
        bcos_sdk_c_free((void*)version);
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
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, );

    try
    {
        ((bcos::cppsdk::Sdk*)sdk)->start();
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
    bcos_sdk_clear_last_error();
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
    bcos_sdk_clear_last_error();
    if (sdk)
    {
        ((bcos::cppsdk::Sdk*)sdk)->stop();
        delete (bcos::cppsdk::Sdk*)sdk;
    }

    BCOS_LOG(INFO) << LOG_BADGE("bcos_sdk_destroy") << LOG_KV("sdk", sdk);
}

/**
 * @brief: register block notifier of the group
 *
 * @param callback
 */
void bcos_sdk_register_block_notifier(void* sdk, const char* group, void* context,
    void (*callback)(const char* group, int64_t block_number, void* context))
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, );
    BCOS_SDK_C_PARAMS_VERIFICATION(group, );
    BCOS_SDK_C_PARAMS_VERIFICATION(callback, );

    auto service = ((bcos::cppsdk::Sdk*)sdk)->service();
    service->registerBlockNumberNotifier(
        group, [context, callback](const std::string& _group, int64_t _blockNumber) {
            callback(_group.c_str(), _blockNumber, context);
        });

    BCOS_LOG(INFO) << LOG_BADGE("bcos_sdk_register_block_notifier") << LOG_KV("sdk", sdk)
                   << LOG_KV("group", group);
}

/**
 * @brief: query group wasm && sm crypto info
 *
 * @param sdk: c sdk object pinter
 * @param group: group id
 * @param wasm: if the group runs the WASM contract engine
 *          0: No, 1: Yes
 * @param sm_crypto: if the group runs sm cryptography component
 *          0: No sm, 1: Yes
 */
void bcos_sdk_get_group_wasm_and_crypto(void* sdk, const char* group, int* wasm, int* sm_crypto)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, );
    BCOS_SDK_C_PARAMS_VERIFICATION(group, );

    auto groupInfo = ((bcos::cppsdk::Sdk*)sdk)->service()->getGroupInfo(std::string(group));
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(groupInfo, "group does not exist", );

    *wasm = groupInfo->wasm();
    *sm_crypto = groupInfo->smCryptoType();
}

/**
 * @brief: query chain id of the group
 *
 * @param sdk: c sdk object pinter
 * @param group: group id
 * @return const char* : chain id
 */
const char* bcos_sdk_get_group_chain_id(void* sdk, const char* group)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(group, NULL);

    auto groupInfo = ((bcos::cppsdk::Sdk*)sdk)->service()->getGroupInfo(std::string(group));
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(groupInfo, "group does not exist", NULL);

    auto chainID = groupInfo->chainID();
    return strdup(chainID.c_str());
}