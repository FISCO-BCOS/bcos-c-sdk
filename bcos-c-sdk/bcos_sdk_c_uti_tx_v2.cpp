/**
 *  Copyright (C) 2022 FISCO BCOS.
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
 * @file bcos_sdk_c_uti_tx_v2.cpp
 * @author: kyonGuo
 * @date 2023/11/28
 */

#include "bcos_sdk_c_uti_tx_v2.h"
#include "bcos_sdk_c_error.h"
#include <bcos-c-sdk/bcos_sdk_c_common.h>
#include <bcos-cpp-sdk/Sdk.h>
#include <bcos-cpp-sdk/utilities/crypto/Common.h>
#include <bcos-cpp-sdk/utilities/tx/TransactionBuilderV2.h>

using namespace bcos;
using namespace bcos::cppsdk;
using namespace bcos::cppsdk::utilities;

#include <cstring>
#include <exception>
#include <system_error>

void* bcos_sdk_create_transaction_v2_data(const char* group_id, const char* chain_id,
    const char* to, const char* nonce, const unsigned char* input, long inputSize, const char* abi,
    int64_t block_limit, const char* value, const char* gas_price, int64_t gas_limit)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, NULL)
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, NULL)
    BCOS_SDK_C_PARAMS_VERIFICATION(input, NULL)
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must > 0", NULL)
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((gas_limit >= 0), "gas limit must >= 0", NULL)

    try
    {
        TransactionBuilderV2 builder;
        auto bytesData = bytes(input, input + inputSize * sizeof(byte));
        auto transactionData = builder.createTransactionData(1, group_id, chain_id, to,
            nonce ? nonce : "", std::move(bytesData), abi, block_limit, value ? value : "",
            gas_price ? gas_price : "", gas_limit);
        return transactionData.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_transaction_data") << LOG_DESC("exception")
                          << LOG_KV("group_id", group_id) << LOG_KV("chain_id", chain_id)
                          << LOG_KV("to", std::string_view(to ? to : ""))
                          << LOG_KV("nonoce", std::string_view(nonce ? nonce : ""))
                          << LOG_KV("inputSize", inputSize)
                          << LOG_KV("abi", std::string_view(abi ? abi : ""))
                          << LOG_KV("block_limit", block_limit)
                          << LOG_KV("value", std::string_view(value ? value : ""))
                          << LOG_KV("gas_price", std::string_view(gas_price ? gas_price : ""))
                          << LOG_KV("gas_limit", gas_limit) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

void* bcos_sdk_create_eip1559_transaction_data(const char* group_id, const char* chain_id,
    const char* to, const char* nonce, const unsigned char* input, long inputSize, const char* abi,
    int64_t block_limit, const char* value, int64_t gas_limit, const char* max_fee_per_gas,
    const char* max_priority_fee_per_gas)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, NULL)
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, NULL)
    BCOS_SDK_C_PARAMS_VERIFICATION(input, NULL)
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must > 0", NULL)
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((gas_limit >= 0), "gas limit must >= 0", NULL)

    try
    {
        TransactionBuilderV2 builder;
        auto bytesData = bytes(input, input + inputSize * sizeof(byte));
        auto transactionData = builder.createTransactionData(1, group_id, chain_id, to,
            nonce ? nonce : "", std::move(bytesData), abi, block_limit, value ? value : "", "",
            gas_limit, max_fee_per_gas ? max_fee_per_gas : "",
            max_priority_fee_per_gas ? max_priority_fee_per_gas : "");
        return transactionData.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING)
            << LOG_BADGE("bcos_sdk_create_eip1559_transaction_data") << LOG_DESC("exception")
            << LOG_KV("group_id", group_id) << LOG_KV("chain_id", chain_id)
            << LOG_KV("to", std::string_view(to ? to : ""))
            << LOG_KV("nonoce", std::string_view(nonce ? nonce : ""))
            << LOG_KV("inputSize", inputSize) << LOG_KV("abi", std::string_view(abi ? abi : ""))
            << LOG_KV("block_limit", block_limit)
            << LOG_KV("value", std::string_view(value ? value : ""))
            << LOG_KV("gas_limit", gas_limit)
            << LOG_KV("max_fee_per_gas", std::string_view(max_fee_per_gas ? max_fee_per_gas : ""))
            << LOG_KV("max_priority_fee_per_gas",
                   std::string_view(max_priority_fee_per_gas ? max_priority_fee_per_gas : ""))
            << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return nullptr;
}


const char* bcos_sdk_calc_transaction_data_hash_with_full_fields(int crypto_type,
    transaction_version version, const char* group_id, const char* chain_id, const char* to,
    const char* nonce, const unsigned char* input, long inputSize, const char* abi,
    int64_t block_limit, const char* value, const char* gas_price, int64_t gas_limit,
    const char* max_fee_per_gas, const char* max_priority_fee_per_gas)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, NULL)
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, NULL)
    BCOS_SDK_C_PARAMS_VERIFICATION(input, NULL)
    BCOS_SDK_C_PARAMS_VERIFICATION(nonce, NULL)
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must > 0", NULL)
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((gas_limit >= 0), "gas limit must >= 0", NULL)

    try
    {
        auto bytesData = bytes(input, input + inputSize * sizeof(byte));
        TransactionBuilderV2 builder;
        auto transactionDataHash = builder.calculateTransactionDataHash(
            crypto_type == BCOS_C_SDK_ECDSA_TYPE ? CryptoType::Secp256K1 : CryptoType::SM2, version,
            group_id, chain_id, to ? to : "", nonce, std::move(bytesData), abi ? abi : "",
            block_limit, value ? value : "", gas_price ? gas_price : "", gas_limit,
            max_fee_per_gas ? max_fee_per_gas : "",
            max_priority_fee_per_gas ? max_priority_fee_per_gas : "");
        return strdup(bcos::toHexStringWithPrefix(transactionDataHash).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING)
            << LOG_BADGE("bcos_sdk_calc_transaction_data_hash_with_full_fields")
            << LOG_DESC("exception") << LOG_KV("group_id", group_id) << LOG_KV("chain_id", chain_id)
            << LOG_KV("to", std::string_view(to ? to : ""))
            << LOG_KV("nonce", std::string_view(nonce ? nonce : ""))
            << LOG_KV("inputSize", inputSize) << LOG_KV("abi", std::string_view(abi ? abi : ""))
            << LOG_KV("block_limit", block_limit)
            << LOG_KV("value", std::string_view(value ? value : ""))
            << LOG_KV("gas_price", std::string_view(gas_price ? gas_price : ""))
            << LOG_KV("gas_limit", gas_limit)
            << LOG_KV("max_fee_per_gas", std::string_view(max_fee_per_gas ? max_fee_per_gas : ""))
            << LOG_KV("max_priority_fee_per_gas",
                   std::string_view(max_priority_fee_per_gas ? max_priority_fee_per_gas : ""))
            << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return nullptr;
}

const char* bcos_sdk_calc_transaction_data_hash_with_json(int crypto_type, const char* json)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(json, NULL)

    try
    {
        TransactionBuilderV2 builder;
        auto transactionDataHash = builder.calculateTransactionDataHashWithJson(
            crypto_type == BCOS_C_SDK_ECDSA_TYPE ? CryptoType::Secp256K1 : CryptoType::SM2, json);
        return strdup(bcos::toHexStringWithPrefix(transactionDataHash).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_calc_transaction_data_hash_with_json")
                          << LOG_DESC("exception") << LOG_KV("json", std::string_view(json))
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return nullptr;
}

const char* bcos_sdk_create_signed_transaction_with_signature(const unsigned char* signature,
    long signSize, const char* transaction_hash, transaction_version version, const char* group_id,
    const char* chain_id, const char* to, const char* nonce, const unsigned char* input,
    long inputSize, const char* abi, int64_t block_limit, const char* value, const char* gas_price,
    int64_t gas_limit, const char* max_fee_per_gas, const char* max_priority_fee_per_gas,
    int32_t attribute, const char* extra_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(signature, NULL)
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_hash, NULL)
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, NULL)
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, NULL)
    BCOS_SDK_C_PARAMS_VERIFICATION(input, NULL)
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must > 0", NULL)
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((gas_limit >= 0), "gas limit must >= 0", NULL)

    try
    {
        TransactionBuilderV2 builder;
        auto bytesData = bytes(input, input + inputSize * sizeof(byte));
        auto hash = std::string_view(transaction_hash);
        crypto::HashType tx_hash(fromHex(hash, hash.starts_with("0x") ? "0x" : ""));
        auto sign = bytes(signature, signature + signSize * sizeof(byte));

        auto transaction = builder.createTransaction(std::move(sign), tx_hash, attribute, version,
            group_id, chain_id, to ? to : "", nonce ? nonce : "", std::move(bytesData),
            abi ? abi : "", block_limit, value ? value : "", gas_price ? gas_price : "", gas_limit,
            max_fee_per_gas ? max_fee_per_gas : "",
            max_priority_fee_per_gas ? max_priority_fee_per_gas : "", extra_data ? extra_data : "");

        auto bytes = builder.encodeTransaction(*transaction);

        return strdup(bcos::toHexStringWithPrefix(*bytes).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_signed_transaction_with_signature")
                          << LOG_DESC("exception") << LOG_KV("signSize", signSize)
                          << LOG_KV("transaction_hash", transaction_hash)
                          << LOG_KV("group_id", group_id) << LOG_KV("chain_id", chain_id)
                          << LOG_KV("to", std::string_view(to ? to : ""))
                          << LOG_KV("nonce", std::string_view(nonce ? nonce : ""))
                          << LOG_KV("inputSize", inputSize)
                          << LOG_KV("abi", std::string_view(abi ? abi : ""))
                          << LOG_KV("block_limit", block_limit)
                          << LOG_KV("value", std::string_view(value ? value : ""))
                          << LOG_KV("gas_price", std::string_view(gas_price ? gas_price : ""))
                          << LOG_KV("gas_limit", gas_limit)
                          << LOG_KV("max_fee_per_gas",
                                 std::string_view(max_fee_per_gas ? max_fee_per_gas : ""));
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return nullptr;
}

void bcos_sdk_create_signed_transaction_with_full_fields(void* key_pair, const char* group_id,
    const char* chain_id, const char* to, const char* nonce, const unsigned char* input,
    long inputSize, const char* abi, int64_t block_limit, const char* value, const char* gas_price,
    int64_t gas_limit, int32_t attribute, const char* extra_data, char** tx_hash, char** signed_tx)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, )
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, )
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, )
    BCOS_SDK_C_PARAMS_VERIFICATION(input, )
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must > 0", )
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((gas_limit >= 0), "gas limit must >= 0", )
    BCOS_SDK_C_PARAMS_VERIFICATION(tx_hash, )
    BCOS_SDK_C_PARAMS_VERIFICATION(signed_tx, )

    try
    {
        TransactionBuilderV2 builder;
        auto bytesData = bytes(input, input + inputSize * sizeof(byte));
        auto result =
            builder.createSignedTransaction(*static_cast<bcos::crypto::KeyPairInterface*>(key_pair),
                attribute, 1, group_id, chain_id, to ? to : "", nonce ? nonce : "",
                std::move(bytesData), abi ? abi : "", block_limit, value ? value : "",
                gas_price ? gas_price : "", gas_limit, "", "", extra_data ? extra_data : "");
        *tx_hash = strdup(result.first.c_str());
        *signed_tx = strdup(result.second.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_signed_transaction_with_full_fields")
                          << LOG_DESC("exception") << LOG_KV("group_id", group_id)
                          << LOG_KV("chain_id", chain_id)
                          << LOG_KV("to", std::string_view(to ? to : ""))
                          << LOG_KV("nonoce", std::string_view(nonce ? nonce : ""))
                          << LOG_KV("inputSize", inputSize)
                          << LOG_KV("abi", std::string_view(abi ? abi : ""))
                          << LOG_KV("block_limit", block_limit)
                          << LOG_KV("value", std::string_view(value ? value : ""))
                          << LOG_KV("gas_price", std::string_view(gas_price ? gas_price : ""))
                          << LOG_KV("gas_limit", gas_limit) << LOG_KV("attribute", attribute)
                          << LOG_KV("extra_data", std::string_view(extra_data ? extra_data : ""))
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
}

void bcos_sdk_create_signed_eip1559_transaction_with_full_fields(void* key_pair,
    const char* group_id, const char* chain_id, const char* to, const char* nonce,
    const unsigned char* input, long inputSize, const char* abi, int64_t block_limit,
    const char* value, int64_t gas_limit, const char* max_fee_per_gas,
    const char* max_priority_fee_per_gas, int32_t attribute, const char* extra_data, char** tx_hash,
    char** signed_tx)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, )
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, )
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, )
    BCOS_SDK_C_PARAMS_VERIFICATION(input, )
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must > 0", )
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((gas_limit >= 0), "gas limit must >= 0", )
    BCOS_SDK_C_PARAMS_VERIFICATION(tx_hash, )
    BCOS_SDK_C_PARAMS_VERIFICATION(signed_tx, )

    try
    {
        TransactionBuilderV2 builder;
        auto bytesData = bytes(input, input + inputSize * sizeof(byte));
        auto result = builder.createSignedTransaction(
            *static_cast<bcos::crypto::KeyPairInterface*>(key_pair), attribute, 1, group_id,
            chain_id, to ? to : "", nonce ? nonce : "", std::move(bytesData), abi ? abi : "",
            block_limit, value ? value : "", "", gas_limit, max_fee_per_gas ? max_fee_per_gas : "",
            max_priority_fee_per_gas ? max_priority_fee_per_gas : "", extra_data ? extra_data : "");
        *tx_hash = strdup(result.first.c_str());
        *signed_tx = strdup(result.second.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING)
            << LOG_BADGE("bcos_sdk_create_signed_eip1559_transaction_with_full_fields")
            << LOG_DESC("exception") << LOG_KV("group_id", group_id) << LOG_KV("chain_id", chain_id)
            << LOG_KV("to", std::string_view(to ? to : ""))
            << LOG_KV("nonoce", std::string_view(nonce ? nonce : ""))
            << LOG_KV("inputSize", inputSize) << LOG_KV("abi", std::string_view(abi ? abi : ""))
            << LOG_KV("block_limit", block_limit)
            << LOG_KV("value", std::string_view(value ? value : ""))
            << LOG_KV("gas_limit", gas_limit)
            << LOG_KV("max_fee_per_gas", std::string_view(max_fee_per_gas ? max_fee_per_gas : ""))
            << LOG_KV("max_priority_fee_per_gas",
                   std::string_view(max_priority_fee_per_gas ? max_priority_fee_per_gas : ""))
            << LOG_KV("attribute", attribute)
            << LOG_KV("extra_data", std::string_view(extra_data ? extra_data : ""))
            << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
}
