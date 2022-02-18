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
 * @file bcos_sdk_c_util_tx.cpp
 * @author: octopus
 * @date 2022-01-17
 */

#include "bcos_sdk_c_uti_tx.h"
#include "bcos_sdk_c_error.h"
#include <bcos-cpp-sdk/utilities/tx/TransactionBuilder.h>
#include <cstring>
#include <exception>
#include <system_error>

using namespace bcos;
using namespace bcos::cppsdk;
using namespace bcos::cppsdk::utilities;

/**
 * @brief
 *
 * @param key_pair
 * @param group_id
 * @param chain_id
 * @param code
 * @param abi
 * @param block_limit
 * @param tx_hash
 * @param signed_tx
 */
void bcos_sdk_create_deploy_contract_tx(void* key_pair, const char* group_id, const char* chain_id,
    const char* code, const char* abi, int64_t block_limit, char** tx_hash, char** signed_tx)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, );
    BCOS_SDK_C_PARAMS_VERIFICATION(code, );
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, );
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, );
    BCOS_SDK_C_PARAMS_VERIFICATION(tx_hash, );
    BCOS_SDK_C_PARAMS_VERIFICATION(signed_tx, );
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "invalid block limit, it must > 0", );

    try
    {
        auto bytesData = fromHexString(code);
        auto builder = std::make_shared<TransactionBuilder>();
        auto r = builder->createDeployContractTransaction(
            *((bcos::crypto::KeyPairInterface*)key_pair), std::string(group_id),
            std::string(chain_id), *bytesData, abi ? abi : "", block_limit, 0);

        *tx_hash = strdup(r.first.c_str());
        *signed_tx = strdup(r.second.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_create_signed_tx") << LOG_DESC("exception throw")
                        << LOG_KV("key_pair", key_pair) << LOG_KV("code", code)
                        << LOG_KV("chain_id", chain_id) << LOG_KV("group_id", group_id)
                        << LOG_KV("abi", abi) << LOG_KV("block_limit", block_limit)
                        << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
}

/**
 * @brief
 *
 * @param key_pair
 * @param group_id
 * @param chain_id
 * @param to
 * @param data
 * @param abi
 * @param block_limit
 * @param attribute
 * @return const char*
 */
void bcos_sdk_create_signed_tx(void* key_pair, const char* group_id, const char* chain_id,
    const char* to, const char* data, int64_t block_limit, int32_t attribute, char** tx_hash,
    char** signed_tx)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, );
    BCOS_SDK_C_PARAMS_VERIFICATION(data, );
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, );
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, );
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "invalid block limit, it must > 0", );

    try
    {
        auto bytesData = fromHexString(data);
        auto builder = std::make_shared<TransactionBuilder>();
        auto r = builder->createSignedTransaction(*((bcos::crypto::KeyPairInterface*)key_pair),
            std::string(group_id), std::string(chain_id), std::string(to), *bytesData, block_limit,
            attribute);

        *tx_hash = strdup(r.first.c_str());
        *signed_tx = strdup(r.second.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_create_signed_tx") << LOG_DESC("exception throw")
                        << LOG_KV("key_pair", key_pair) << LOG_KV("to", to) << LOG_KV("data", data)
                        << LOG_KV("chain_id", chain_id) << LOG_KV("group_id", group_id)
                        << LOG_KV("block_limit", block_limit) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
}