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
 * @file bcos_sdk_c_amop.cpp
 * @author: octopus
 * @date 2021-12-15
 */

#include "bcos_sdk_c_amop.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos_sdk_c_common.h"
#include <bcos-cpp-sdk/Sdk.h>
#include <bcos-cpp-sdk/amop/AMOP.h>
#include <bcos-utilities/Common.h>
#include <set>
#include <string>

// ------------------------------amop interface begin ----------------------

using namespace bcos;
using namespace bcos::boostssl;

void bcos_amop_subscribe_topic(void* sdk, char** topics, size_t topic_count)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, );
    BCOS_SDK_C_PARAMS_VERIFICATION(topics, );
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((topic_count > 0), "topic count == 0", );

    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto amop = sdkPointer->amop();

    std::set<std::string> setTopics;
    for (size_t i = 0; i < topic_count; i++)
    {
        setTopics.insert(std::string(topics[i]));
    }

    amop->subscribe(setTopics);
}

void bcos_amop_subscribe_topic_with_cb(
    void* sdk, const char* topic, bcos_sdk_c_amop_subscribe_cb cb, void* context)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, );
    BCOS_SDK_C_PARAMS_VERIFICATION(topic, );
    BCOS_SDK_C_PARAMS_VERIFICATION(cb, );

    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto amop = sdkPointer->amop();
    amop->subscribe(std::string(topic),
        [context, cb](Error::Ptr error, const std::string& endpoint, const std::string& seq,
            bytesConstRef data, std::shared_ptr<bcos::boostssl::ws::WsSession> session) {
            std::ignore = session;
            // create resp obj
            bcos_sdk_c_struct_response resp;
            resp.context = context;
            if (error && error->errorCode() != 0)
            {
                resp.error = error->errorCode();
                resp.desc = (char*)error->errorMessage().data();
            }
            else
            {
                resp.error = 0;
                resp.data = (void*)data.data();
                resp.size = data.size();
            }

            if (cb)
            {
                // callback
                cb(endpoint.c_str(), seq.c_str(), &resp);
            }
        });
}

void bcos_amop_unsubscribe_topic(void* sdk, char** topics, size_t topic_count)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, );
    BCOS_SDK_C_PARAMS_VERIFICATION(topics, );
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((topic_count > 0), "topic count == 0", );

    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto amop = sdkPointer->amop();

    std::set<std::string> setTopics;
    for (size_t i = 0; i < topic_count; i++)
    {
        setTopics.insert(std::string(topics[i]));
    }

    amop->unsubscribe(setTopics);
}

void bcos_amop_publish(void* sdk, const char* topic, void* data, size_t size, uint32_t timeout,
    bcos_sdk_c_amop_publish_cb cb, void* context)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, );
    BCOS_SDK_C_PARAMS_VERIFICATION(topic, );
    BCOS_SDK_C_PARAMS_VERIFICATION(data, );
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((size > 0), "data size == 0", );

    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto amop = sdkPointer->amop();
    amop->publish(std::string(topic), bytesConstRef((byte*)data, size), timeout,
        [cb, context](Error::Ptr error, std::shared_ptr<bcos::boostssl::ws::WsMessage> msg,
            std::shared_ptr<bcos::boostssl::ws::WsSession> session) {
            std::ignore = session;

            void* data = msg ? msg->payload()->data() : NULL;
            size_t size = msg ? msg->payload()->size() : 0;

            bcos_sdk_c_handle_response(error ? error.get() : NULL, data, size, cb, context);
        });
}

void bcos_amop_broadcast(void* sdk, const char* topic, void* data, size_t size)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, );
    BCOS_SDK_C_PARAMS_VERIFICATION(topic, );
    BCOS_SDK_C_PARAMS_VERIFICATION(data, );
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((size > 0), "data size == 0", );

    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto amop = sdkPointer->amop();
    amop->broadcast(std::string(topic), bytesConstRef((byte*)data, size));
}

void bcos_amop_set_subscribe_topic_cb(void* sdk, bcos_sdk_c_amop_subscribe_cb cb, void* context)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, );
    BCOS_SDK_C_PARAMS_VERIFICATION(cb, );

    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto amop = sdkPointer->amop();
    amop->setSubCallback(
        [context, cb](Error::Ptr error, const std::string& endpoint, const std::string& seq,
            bytesConstRef data, std::shared_ptr<bcos::boostssl::ws::WsSession> session) {
            std::ignore = session;
            // create resp obj
            bcos_sdk_c_struct_response resp;
            resp.context = context;
            if (error && error->errorCode() != 0)
            {
                resp.error = error->errorCode();
                resp.desc = (char*)error->errorMessage().data();
            }
            else
            {
                resp.error = 0;
                resp.data = (void*)data.data();
                resp.size = data.size();
            }

            // callback
            cb(endpoint.c_str(), seq.c_str(), &resp);
        });
}

void bcos_amop_send_response(void* sdk, const char* peer, const char* seq, void* data, size_t size)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, );
    BCOS_SDK_C_PARAMS_VERIFICATION(peer, );
    BCOS_SDK_C_PARAMS_VERIFICATION(seq, );
    BCOS_SDK_C_PARAMS_VERIFICATION(data, );
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((size > 0), "data size == 0", );

    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto amop = sdkPointer->amop();
    amop->sendResponse(std::string(peer), std::string(seq), bytesConstRef((byte*)data, size));
}

// ------------------------------amop interface end -------------------------