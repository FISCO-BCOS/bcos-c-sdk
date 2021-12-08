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
        contextConfig->setIsCertPath(false);

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

// create bcos sdk object by config
void* bcos_sdk_create(struct bcos_sdk_c_config* config)
{
    bcos_sdk_clear_last_error();
    try
    {
        // construct sdk object
        auto factory = std::make_shared<bcos::cppsdk::SdkFactory>();
        factory->setConfig(initWsConfig(config));
        auto sdk = factory->buildSdk();
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

// start the bcos sdk
void bcos_sdk_start(void* sdk)
{
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

// stop the bcos sdk
void bcos_sdk_stop(void* sdk)
{
    if (sdk)
    {
        ((bcos::cppsdk::Sdk*)sdk)->stop();
    }

    BCOS_LOG(INFO) << LOG_BADGE("bcos_sdk_stop") << LOG_KV("sdk", sdk);
}

// destroy the bcos sdk object
void bcos_sdk_destroy(void* sdk)
{
    if (sdk)
    {
        delete (bcos::cppsdk::Sdk*)sdk;
    }

    BCOS_LOG(INFO) << LOG_BADGE("bcos_sdk_destroy") << LOG_KV("sdk", sdk);
}