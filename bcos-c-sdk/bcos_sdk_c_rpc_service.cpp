/*
 *  Copyright (C) 2023 FISCO BCOS.
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
 * @file bcos_sdk_c_tars_rpc.cpp
 * @author: octopus
 * @date 2023-02-26
 */

#include "bcos-c-sdk/bcos_sdk_c_rpc_service.h"
#include "bcos-c-sdk/bcos_sdk_c_common.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_rpc.h"
#include <bcos-cpp-sdk/Sdk.h>
#include <bcos-crypto/interfaces/crypto/KeyPairInterface.h>
#include <cstring>

using namespace bcos;
using namespace bcos::boostssl;

// sendTransaction
const char* bcos_rpc_service_send_transaction(void* sdk, void* key_pair, const char* group,
    const char* node, const char* to, void* data, int size, const char* abi, int attribute,
    const char* extra_data, bcos_sdk_c_struct_response_cb callback, void* context)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(group, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(data, NULL);

    std::string groupID = group;
    std::string nodeName = (node ? std::string(node) : "");
    std::string toAddr = to ? std::string(to) : "";
    // TODO: reduce memory copy
    bcos::bytes abiData((bcos::byte*)data, (bcos::byte*)data + size);
    std::string abiContent = (abi ? std::string(abi) : "");
    std::string extraData = extra_data ? std::string(extra_data) : "";

    std::string transactionHash =
        ((bcos::cppsdk::Sdk*)sdk)
            ->jsonRpcService()
            ->sendTransaction(*((bcos::crypto::KeyPairInterface*)key_pair), groupID, nodeName,
                toAddr, std::move(abiData), abiContent, attribute, extraData,
                [callback, context](bcos::Error::Ptr _error, std::shared_ptr<bcos::bytes> _resp) {
                    bcos_sdk_c_handle_response(_error ? _error.get() : NULL,
                        _resp ? (void*)_resp->data() : NULL, _resp ? _resp->size() : 0, callback,
                        context);
                });

    return strdup(transactionHash.c_str());
}
