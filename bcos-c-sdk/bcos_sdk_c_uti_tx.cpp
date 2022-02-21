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
#include <bcos-cpp-sdk/utilities/crypto/Common.h>
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
 * @param group_id
 * @param chain_id
 * @param to
 * @param data
 * @param abi
 * @param block_limit
 * @return void*
 */
void* bcos_sdk_create_transaction_data(const char* group_id, const char* chain_id, const char* to,
    const char* data, const char* abi, int64_t block_limit)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, NULL);
    // BCOS_SDK_C_PARAMS_VERIFICATION(to, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(data, NULL);
    // BCOS_SDK_C_PARAMS_VERIFICATION(abi, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must >= 0", NULL);

    try
    {
        auto bytesData = fromHexString(data);
        auto builder = std::make_shared<TransactionBuilder>();
        auto transactionData =
            builder->createTransactionData(std::string(group_id), std::string(chain_id),
                std::string(to ? to : ""), *bytesData, std::string(abi ? abi : ""), block_limit);
        return transactionData.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_transaction_data") << LOG_DESC("exception")
                          << LOG_KV("group_id", group_id) << LOG_KV("chain_id", chain_id)
                          << LOG_KV("to", std::string(to ? to : "")) << LOG_KV("data", data)
                          << LOG_KV("abi", std::string(abi ? abi : ""))
                          << LOG_KV("block_limit", block_limit) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

/**
 * @brief
 *
 * @param transaction_data
 */
void bcos_sdk_destroy_transaction_data(void* transaction_data)
{
    if (transaction_data)
    {
        delete (bcostars::TransactionData*)transaction_data;
        transaction_data = NULL;
    }
}

/**
 * @brief
 *
 * @param transaction_data
 * @return const char*
 */
const char* bcos_sdk_encode_transaction_data(void* transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);

    try
    {
        auto builder = std::make_shared<TransactionBuilder>();
        auto transactionData =
            builder->encodeTransactionData(*(bcostars::TransactionData*)transaction_data);
        return strdup(toHexString(*transactionData)->c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_encode_transaction_data") << LOG_DESC("exception")
                          << LOG_KV("transaction_data", transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

/**
 * @brief
 *
 * @param crypto_type
 * @param transaction_data
 * @return const char*
 */
const char* bcos_sdk_calc_transaction_data_hash(int crypto_type, void* transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((crypto_type == 1 || crypto_type == 2),
        "invalid crypto type, it must be 1(ecdsa crypto type) or 2(sm crypto type)", NULL);

    try
    {
        auto builder = std::make_shared<TransactionBuilder>();
        auto transactionDataHash = builder->calculateTransactionDataHash(
            crypto_type == 1 ? CryptoType::Secp256K1 : CryptoType::SM2,
            *(bcostars::TransactionData*)transaction_data);
        return strdup(toHexString(transactionDataHash)->c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_calc_transaction_data_hash")
                          << LOG_DESC("exception") << LOG_KV("crypto_type", crypto_type)
                          << LOG_KV("transaction_data", transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

/**
 * @brief
 *
 * @param keypair
 * @param transcation_hash
 * @return const char*
 */
const char* bcos_sdk_sign_transaction_data_hash(void* keypair, const char* transcation_hash)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(keypair, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transcation_hash, NULL);
    try
    {
        auto builder = std::make_shared<TransactionBuilder>();
        bcos::crypto::HashType hashType(transcation_hash);
        auto signData =
            builder->signTransactionDataHash(*(crypto::KeyPairInterface*)keypair, hashType);
        return strdup(toHexString(*signData)->c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_sign_transaction_data_hash")
                          << LOG_DESC("exception") << LOG_KV("keypair", keypair)
                          << LOG_KV("transcation_hash", transcation_hash)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
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
 * @param tx_hash
 * @param signed_tx
 */
void bcos_sdk_create_signed_tx(void* key_pair, const char* group_id, const char* chain_id,
    const char* to, const char* data, const char* abi, int64_t block_limit, int32_t attribute,
    char** tx_hash, char** signed_tx)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, );
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, );
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, );
    BCOS_SDK_C_PARAMS_VERIFICATION(data, );
    BCOS_SDK_C_PARAMS_VERIFICATION(tx_hash, );
    BCOS_SDK_C_PARAMS_VERIFICATION(signed_tx, );

    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must >= 0", );

    try
    {
        auto bytesData = fromHexString(data);
        auto builder = std::make_shared<TransactionBuilder>();
        auto r = builder->createSignedTransaction(*((bcos::crypto::KeyPairInterface*)key_pair),
            std::string(group_id), std::string(chain_id), std::string(to ? to : ""), *bytesData,
            std::string(abi ? abi : ""), block_limit, attribute);

        *tx_hash = strdup(r.first.c_str());
        *signed_tx = strdup(r.second.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_signed_tx") << LOG_DESC("exception")
                          << LOG_KV("key_pair", key_pair) << LOG_KV("group_id", group_id)
                          << LOG_KV("chain_id", chain_id) << LOG_KV("to", to)
                          << LOG_KV("data", data) << LOG_KV("abi", abi)
                          << LOG_KV("block_limit", block_limit) << LOG_KV("attribute", attribute)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
}

/**
 * @brief
 *
 * @param transaction_data
 * @param signed_transaction_data
 * @param transaction_data_hash
 * @param attribute
 * @return const char*
 */
const char* bcos_sdk_create_signed_tx_with_signed_data(void* transaction_data,
    const char* signed_transaction_data, const char* transaction_data_hash, int32_t attribute)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(signed_transaction_data, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data_hash, NULL);

    try
    {
        auto builder = std::make_shared<TransactionBuilder>();
        auto signedBytes = builder->createSignedTransaction(
            *(bcostars::TransactionData*)transaction_data, *fromHexString(signed_transaction_data),
            bcos::crypto::HashType(transaction_data_hash), attribute);
        return strdup(toHexString(*signedBytes)->c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_signed_tx_with_signed_data")
                          << LOG_DESC("exception")
                          << LOG_KV("signed_transaction_data", signed_transaction_data)
                          << LOG_KV("transaction_data_hash", transaction_data_hash)
                          << LOG_KV("attribute", attribute) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return NULL;
}