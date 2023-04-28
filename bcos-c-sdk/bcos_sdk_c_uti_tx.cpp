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
#include <bcos-c-sdk/bcos_sdk_c_common.h>
#include <bcos-cpp-sdk/Sdk.h>
#include <bcos-cpp-sdk/utilities/crypto/Common.h>
#include <bcos-cpp-sdk/utilities/tx/TransactionBuilder.h>
#include <bcos-cpp-sdk/utilities/tx/TransactionBuilderService.h>
#include <cstring>
#include <exception>
#include <memory>
#include <system_error>

using namespace bcos;
using namespace bcos::cppsdk;
using namespace bcos::cppsdk::utilities;

/**
 * @brief: convert tars transaction data to struct bcos_sdk_c_transaction_data
 *
 * @param tars_transaction_data: response error, pointer to Error::Ptr in cpp-sdk
 * @return bcos_sdk_c_transaction_data*: struct bcos_sdk_c_transaction_data pointer
 */
struct bcos_sdk_c_transaction_data* convert_tars_transaction_data_to_struct(
    bcostars::TransactionDataUniquePtr tars_transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(tars_transaction_data, NULL);

    try
    {
        struct bcos_sdk_c_transaction_data* transaction_data_struct =
            (struct bcos_sdk_c_transaction_data*)malloc(sizeof(struct bcos_sdk_c_transaction_data));
        struct bcos_sdk_c_bytes* input_bytes =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        transaction_data_struct->version = tars_transaction_data->version;
        transaction_data_struct->block_limit = tars_transaction_data->blockLimit;
        transaction_data_struct->chain_ID = my_strdup(tars_transaction_data->chainID.data());
        transaction_data_struct->group_ID = my_strdup(tars_transaction_data->groupID.data());
        transaction_data_struct->nonce = my_strdup(tars_transaction_data->nonce.data());
        transaction_data_struct->to = my_strdup(tars_transaction_data->to.data());
        transaction_data_struct->abi = my_strdup(tars_transaction_data->abi.data());
        uint32_t input_length = tars_transaction_data->input.size();
        uint8_t* buffer = (uint8_t*)malloc(input_length);
        memcpy(buffer, tars_transaction_data->input.data(), input_length);
        input_bytes->buffer = buffer;
        input_bytes->length = input_length;
        transaction_data_struct->input = input_bytes;

        return transaction_data_struct;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("convert_tars_transaction_data_to_struct")
                          << LOG_DESC("exception")
                          << LOG_KV("transaction data", tars_transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

bcostars::TransactionDataUniquePtr convert_transaction_data_struct_to_tars_struct(
    struct bcos_sdk_c_transaction_data* transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);

    try
    {
        auto tars_transaction_data = std::make_unique<bcostars::TransactionData>();
        tars_transaction_data->version = (tars::Int32)transaction_data->version;
        tars_transaction_data->blockLimit = (tars::Int64)transaction_data->block_limit;
        tars_transaction_data->chainID = std::string(transaction_data->chain_ID);
        tars_transaction_data->groupID = std::string(transaction_data->group_ID);
        tars_transaction_data->nonce = std::string(transaction_data->nonce);
        tars_transaction_data->to = std::string(transaction_data->to);
        tars_transaction_data->abi = std::string(transaction_data->abi);
        tars_transaction_data->groupID = std::string(transaction_data->group_ID);
        std::vector<tars::Char> input;
        for (size_t i = 0; i < transaction_data->input->length; ++i)
        {
            input.push_back(transaction_data->input->buffer[i]);
        }
        tars_transaction_data->input = std::move(input);

        return tars_transaction_data;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("convert_transaction_data_struct_to_tars_struct")
                          << LOG_DESC("exception") << LOG_KV("transaction data", transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

bcostars::TransactionUniquePtr convert_transaction_struct_to_tars_struct(
    struct bcos_sdk_c_transaction* transaction)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction, NULL);

    try
    {
        auto tars_transaction = std::make_unique<bcostars::Transaction>();
        tars_transaction->importTime = (tars::Int32)transaction->import_time;
        tars_transaction->attribute = (tars::Int64)transaction->attribute;
        tars_transaction->extraData = std::string(transaction->extraData);
        auto TransactionDataUniquePtr =
            convert_transaction_data_struct_to_tars_struct(&(transaction->transaction_data));
        tars_transaction->data = *TransactionDataUniquePtr;
        std::vector<tars::Char> data_hash_vec, signature_vec, sender_vec;
        for (size_t i = 0; i < transaction->data_hash.length; ++i)
        {
            data_hash_vec.push_back(transaction->data_hash.buffer[i]);
        }
        for (size_t i = 0; i < transaction->signature.length; ++i)
        {
            signature_vec.push_back(transaction->signature.buffer[i]);
        }
        for (size_t i = 0; i < transaction->sender.length; ++i)
        {
            sender_vec.push_back(transaction->sender.buffer[i]);
        }
        tars_transaction->dataHash = data_hash_vec;
        tars_transaction->signature = signature_vec;
        tars_transaction->sender = sender_vec;

        return tars_transaction;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("convert_transaction_struct_to_tars_struct")
                          << LOG_DESC("exception") << LOG_KV("transaction", transaction)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

struct bcos_sdk_c_transaction* convert_tars_transaction_to_struct(
    bcostars::TransactionUniquePtr tars_transaction, char** tx_hash, char** signed_tx)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(tars_transaction, NULL);

    try
    {
        struct bcos_sdk_c_transaction* transaction = new bcos_sdk_c_transaction;
        auto transactionDataUniquePtr =
            std::make_unique<bcostars::TransactionData>(tars_transaction->data);
        transaction->transaction_data =
            *(convert_tars_transaction_data_to_struct(std::move(transactionDataUniquePtr)));
        struct bcos_sdk_c_bytes data_hash_bytes, signature_bytes, sender_bytes;
        data_hash_bytes.buffer = (uint8_t*)tx_hash;
        data_hash_bytes.length = sizeof(tx_hash) / sizeof(char);
        signature_bytes.buffer = (uint8_t*)signed_tx;
        signature_bytes.length = sizeof(signed_tx) / sizeof(char);
        sender_bytes.buffer = (uint8_t*)(tars_transaction->sender.data());
        sender_bytes.length = tars_transaction->sender.size();
        transaction->data_hash = data_hash_bytes;
        transaction->signature = signature_bytes;
        transaction->sender = sender_bytes;
        transaction->import_time = tars_transaction->importTime;
        transaction->attribute = tars_transaction->attribute;
        transaction->extraData = (char*)tars_transaction->extraData.c_str();
        return transaction;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("convert_tars_transaction_to_struct")
                          << LOG_DESC("exception") << LOG_KV("transaction", tars_transaction)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

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
 * @param group_id
 * @param chain_id
 * @param to
 * @param data
 * @param abi
 * @param block_limit
 * @return bcos_sdk_c_transaction_data*: transaction data struct pointer, return unassigned struct
 * on failure according to the function called bcos_sdk_get_last_error(if create failed, return -1)
 */
struct bcos_sdk_c_transaction_data* bcos_sdk_create_transaction_data_struct(const char* group_id,
    const char* chain_id, const char* to, const char* data, const char* abi, int64_t block_limit)
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

        return convert_tars_transaction_data_to_struct(std::move(transactionData));
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_transaction_data_struct")
                          << LOG_DESC("exception") << LOG_KV("group_id", group_id)
                          << LOG_KV("chain_id", chain_id) << LOG_KV("to", std::string(to ? to : ""))
                          << LOG_KV("data", data) << LOG_KV("abi", std::string(abi ? abi : ""))
                          << LOG_KV("block_limit", block_limit) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

void* bcos_sdk_create_transaction_data_with_json(const char* json)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(json, NULL);

    try
    {
        auto builder = std::make_shared<TransactionBuilder>();
        auto transactionData = builder->createTransactionDataWithJson(std::string(json));
        return transactionData.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_transaction_data_with_json")
                          << LOG_DESC("exception") << LOG_KV("json", json)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

/**
 * @param json
 *              version:number
 *              groupID:string
 *              chainID:string
 *              to:string
 *              data:hex string
 *              abi:string
 *              blockLimit:number
 *              nonce:string
 * @return struct bcos_sdk_c_transaction*
 */
struct bcos_sdk_c_transaction_data* bcos_sdk_create_transaction_data_struct_with_json(
    const char* json)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(json, NULL);

    try
    {
        auto builder = std::make_shared<TransactionBuilder>();
        auto transactionData = builder->createTransactionDataWithJson(std::string(json));

        return convert_tars_transaction_data_to_struct(std::move(transactionData));
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_transaction_data_struct_with_json")
                          << LOG_DESC("exception") << LOG_KV("json", json)
                          << LOG_KV("error", errorMsg);
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
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 */
void bcos_sdk_destroy_transaction_data_struct(struct bcos_sdk_c_transaction_data* transaction_data)
{
    if (transaction_data == NULL)
    {
        return;
    }

    if (transaction_data && transaction_data->chain_ID)
    {
        bcos_sdk_c_free(transaction_data->chain_ID);
    }

    if (transaction_data && transaction_data->group_ID)
    {
        bcos_sdk_c_free(transaction_data->group_ID);
    }

    if (transaction_data && transaction_data->nonce)
    {
        bcos_sdk_c_free(transaction_data->nonce);
    }

    if (transaction_data && transaction_data->to)
    {
        bcos_sdk_c_free(transaction_data->to);
    }

    if (transaction_data && transaction_data->abi)
    {
        bcos_sdk_c_free(transaction_data->abi);
    }

    if (transaction_data && transaction_data->input)
    {
        if (transaction_data->input->buffer)
        {
            bcos_sdk_c_free(transaction_data->input->buffer);
        }
        bcos_sdk_c_free(transaction_data->input);
    }

    bcos_sdk_c_free(transaction_data);
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
 * @brief encode transaction data into hex format
 *
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 * @return const char*
 */
const char* bcos_sdk_encode_transaction_data_to_hex(
    struct bcos_sdk_c_transaction_data* transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);

    try
    {
        auto tars_transaction_data =
            convert_transaction_data_struct_to_tars_struct(transaction_data);
        auto builder = std::make_shared<TransactionBuilder>();
        auto transactionData = builder->encodeTransactionData(*tars_transaction_data);
        return strdup(toHexString(*transactionData)->c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_encode_transaction_data_to_hex")
                          << LOG_DESC("exception") << LOG_KV("transaction_data", transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

/**
 * @brief convert transaction data into json format
 *
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 * @return const char*
 */
const char* bcos_sdk_transaction_data_to_json(struct bcos_sdk_c_transaction_data* transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);

    try
    {
        auto tars_transaction_data =
            convert_transaction_data_struct_to_tars_struct(transaction_data);
        auto json_str = tars_transaction_data->writeToJsonString();
        return strdup(json_str.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_transaction_data_to_json") << LOG_DESC("exception")
                          << LOG_KV("transaction_data", transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

const char* bcos_sdk_decode_transaction_data(const char* transaction_hex_str)
{
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_hex_str, NULL);
    try
    {
        auto builder = std::make_unique<TransactionBuilder>();
        auto tx_bytes = fromHexString(transaction_hex_str);
        auto json = builder->decodeTransactionDataToJsonObj(*tx_bytes);
        return strdup(json.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_decode_transaction_data") << LOG_DESC("exception")
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
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        auto builder = std::make_shared<TransactionBuilder>();
        auto transactionDataHash = builder->calculateTransactionDataHash(
            crypto_type == BCOS_C_SDK_ECDSA_TYPE ? CryptoType::Secp256K1 : CryptoType::SM2,
            *(bcostars::TransactionData*)transaction_data);
        return strdup(bcos::toHexStringWithPrefix(transactionDataHash).c_str());
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
 * @param crypto_type: int
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 * @return const char*
 */
const char* bcos_sdk_calc_transaction_data_struct_hash(
    int crypto_type, struct bcos_sdk_c_transaction_data* transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        auto tars_transaction_data =
            convert_transaction_data_struct_to_tars_struct(transaction_data);
        auto builder = std::make_shared<TransactionBuilder>();
        auto transactionDataHash = builder->calculateTransactionDataHash(
            crypto_type == BCOS_C_SDK_ECDSA_TYPE ? CryptoType::Secp256K1 : CryptoType::SM2,
            *tars_transaction_data);
        return strdup(bcos::toHexStringWithPrefix(transactionDataHash).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_calc_transaction_data_struct_hash")
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
 * @param transaction_hash
 * @return const char*
 */
const char* bcos_sdk_sign_transaction_data_hash(void* keypair, const char* transaction_hash)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(keypair, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_hash, NULL);
    try
    {
        auto builder = std::make_shared<TransactionBuilder>();
        bcos::crypto::HashType hashType(transaction_hash);
        auto signData =
            builder->signTransactionDataHash(*(crypto::KeyPairInterface*)keypair, hashType);
        return strdup(bcos::toHexStringWithPrefix(*signData).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_sign_transaction_data_hash")
                          << LOG_DESC("exception") << LOG_KV("keypair", keypair)
                          << LOG_KV("transaction_hash", transaction_hash)
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
void bcos_sdk_create_signed_transaction(void* key_pair, const char* group_id, const char* chain_id,
    const char* to, const char* data, const char* abi, int64_t block_limit, int32_t attribute,
    char** tx_hash, char** signed_tx)
{
    return bcos_sdk_create_signed_transaction_ver_extra_data(key_pair, group_id, chain_id, to, data,
        abi, block_limit, attribute, NULL, tx_hash, signed_tx);
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
 * @return struct bcos_sdk_c_transaction*
 */
struct bcos_sdk_c_transaction* bcos_sdk_create_signed_transaction_struct(void* key_pair,
    const char* group_id, const char* chain_id, const char* to, const char* data, const char* abi,
    int64_t block_limit, int32_t attribute, char** tx_hash, char** signed_tx)
{
    return bcos_sdk_create_signed_transaction_struct_ver_extra_data(key_pair, group_id, chain_id,
        to, data, abi, block_limit, attribute, NULL, tx_hash, signed_tx);
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
 * @param extra_data
 * @param tx_hash
 * @param signed_tx
 */
void bcos_sdk_create_signed_transaction_ver_extra_data(void* key_pair, const char* group_id,
    const char* chain_id, const char* to, const char* data, const char* abi, int64_t block_limit,
    int32_t attribute, const char* extra_data, char** tx_hash, char** signed_tx)
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
            std::string(abi ? abi : ""), block_limit, attribute,
            extra_data ? std::string(extra_data) : std::string());

        *tx_hash = strdup(r.first.c_str());
        *signed_tx = strdup(r.second.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_signed_transaction_ver_extra_data")
                          << LOG_DESC("exception") << LOG_KV("key_pair", key_pair)
                          << LOG_KV("group_id", group_id) << LOG_KV("chain_id", chain_id)
                          << LOG_KV("to", to) << LOG_KV("data", data) << LOG_KV("abi", abi)
                          << LOG_KV("block_limit", block_limit) << LOG_KV("attribute", attribute)
                          << LOG_KV("extra_data", extra_data) << LOG_KV("error", errorMsg);
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
 * @param extra_data
 * @param tx_hash
 * @param signed_tx
 * @return struct bcos_sdk_c_transaction*
 */
struct bcos_sdk_c_transaction* bcos_sdk_create_signed_transaction_struct_ver_extra_data(
    void* key_pair, const char* group_id, const char* chain_id, const char* to, const char* data,
    const char* abi, int64_t block_limit, int32_t attribute, const char* extra_data, char** tx_hash,
    char** signed_tx)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(data, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(tx_hash, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(signed_tx, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must >= 0", NULL);

    try
    {
        auto bytesData = fromHexString(data);
        auto builder = std::make_shared<TransactionBuilder>();
        auto r = builder->createSignedTransaction(*((bcos::crypto::KeyPairInterface*)key_pair),
            std::string(group_id), std::string(chain_id), std::string(to ? to : ""), *bytesData,
            std::string(abi ? abi : ""), block_limit, attribute,
            extra_data ? std::string(extra_data) : std::string());
        // obtain tx_hash/signed_tx
        *tx_hash = strdup(r.first.c_str());
        *signed_tx = strdup(r.second.c_str());
        // create transaction_data
        bcos::crypto::HashType hashType(*tx_hash);
        void* transaction_data =
            bcos_sdk_create_transaction_data(group_id, chain_id, to, data, abi, block_limit);
        auto tars_transaction = builder->createTransaction(
            *(bcostars::TransactionData*)transaction_data, *fromHexString(r.second), hashType,
            attribute, extra_data ? std::string(extra_data) : std::string());
        // create transaction
        struct bcos_sdk_c_transaction* transaction =
            convert_tars_transaction_to_struct(std::move(tars_transaction), tx_hash, signed_tx);
        return transaction;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_signed_transaction_struct_ver_extra_data")
                          << LOG_DESC("exception") << LOG_KV("key_pair", key_pair)
                          << LOG_KV("group_id", group_id) << LOG_KV("chain_id", chain_id)
                          << LOG_KV("to", to) << LOG_KV("data", data) << LOG_KV("abi", abi)
                          << LOG_KV("block_limit", block_limit) << LOG_KV("attribute", attribute)
                          << LOG_KV("extra_data", extra_data) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
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
const char* bcos_sdk_create_signed_transaction_with_signed_data(void* transaction_data,
    const char* signed_transaction_data, const char* transaction_data_hash, int32_t attribute)
{
    return bcos_sdk_create_signed_transaction_with_signed_data_ver_extra_data(
        transaction_data, signed_transaction_data, transaction_data_hash, attribute, NULL);
}

/**
 * @brief
 *
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 * @param signed_transaction_data
 * @param transaction_data_hash
 * @param attribute
 * @return const char*
 */
const char* bcos_sdk_create_signed_transaction_with_struct_and_signed_data(
    struct bcos_sdk_c_transaction_data* transaction_data, const char* signed_transaction_data,
    const char* transaction_data_hash, int32_t attribute)
{
    return bcos_sdk_create_signed_transaction_with_struct_and_signed_data_ver_extra_data(
        transaction_data, signed_transaction_data, transaction_data_hash, attribute, NULL);
}

/**
 * @brief
 *
 * @param transaction_data
 * @param signed_transaction_data
 * @param transaction_data_hash
 * @param attribute
 * @param extra_data
 * @return const char*
 */
const char* bcos_sdk_create_signed_transaction_with_signed_data_ver_extra_data(
    void* transaction_data, const char* signed_transaction_data, const char* transaction_data_hash,
    int32_t attribute, const char* extra_data)
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
            bcos::crypto::HashType(transaction_data_hash), attribute,
            extra_data ? std::string(extra_data) : std::string());
        return strdup(bcos::toHexStringWithPrefix(*signedBytes).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING)
            << LOG_BADGE("bcos_sdk_create_signed_transaction_with_signed_data_ver_extra_data")
            << LOG_DESC("exception") << LOG_KV("signed_transaction_data", signed_transaction_data)
            << LOG_KV("transaction_data_hash", transaction_data_hash)
            << LOG_KV("attribute", attribute) << LOG_KV("extra_data", extra_data)
            << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return NULL;
}

/**
 * @brief
 *
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 * @param signed_transaction_data
 * @param transaction_data_hash
 * @param attribute
 * @param extra_data
 * @return const char*
 */
const char* bcos_sdk_create_signed_transaction_with_struct_and_signed_data_ver_extra_data(
    struct bcos_sdk_c_transaction_data* transaction_data, const char* signed_transaction_data,
    const char* transaction_data_hash, int32_t attribute, const char* extra_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(signed_transaction_data, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data_hash, NULL);

    try
    {
        auto builder = std::make_shared<TransactionBuilder>();
        auto tars_transaction_data =
            convert_transaction_data_struct_to_tars_struct(transaction_data);
        auto signedBytes = builder->createSignedTransaction(*tars_transaction_data,
            *fromHexString(signed_transaction_data), bcos::crypto::HashType(transaction_data_hash),
            attribute, extra_data ? std::string(extra_data) : std::string());
        return strdup(bcos::toHexStringWithPrefix(*signedBytes).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING)
            << LOG_BADGE(
                   "bcos_sdk_create_signed_transaction_with_struct_and_signed_data_ver_extra_data")
            << LOG_DESC("exception") << LOG_KV("transaction_data", transaction_data)
            << LOG_KV("signed_transaction_data", signed_transaction_data)
            << LOG_KV("transaction_data_hash", transaction_data_hash)
            << LOG_KV("attribute", attribute) << LOG_KV("extra_data", extra_data)
            << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return NULL;
}

/**
 * @brief encode transaction into hex format
 *
 * @param transaction: struct bcos_sdk_c_transaction*
 * @return const char*
 */
const char* bcos_sdk_encode_transaction_to_hex(struct bcos_sdk_c_transaction* transaction)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction, NULL);

    try
    {
        auto tars_transaction = convert_transaction_struct_to_tars_struct(transaction);
        auto builder = std::make_shared<TransactionBuilder>();
        auto transaction = builder->encodeTransaction(*tars_transaction);
        return strdup(toHexString(*transaction)->c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_encode_transaction_to_hex")
                          << LOG_DESC("exception") << LOG_KV("transaction", transaction)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

/**
 * @brief
 *
 * @param transaction_hex_str
 * @return const char*
 */
const char* bcos_sdk_decode_transaction_by_hex(const char* transaction_hex_str)
{
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_hex_str, NULL);
    try
    {
        auto builder = std::make_unique<TransactionBuilder>();
        auto tx_bytes = fromHexString(transaction_hex_str);
        auto json = builder->decodeTransactionToJsonObj(*tx_bytes);
        return strdup(json.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_decode_transaction_by_hex")
                          << LOG_DESC("exception") << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

/**
 * @brief convert transaction into json format
 *
 * @param transaction: struct bcos_sdk_c_transaction*
 * @return const char*
 */
const char* bcos_sdk_transaction_to_json(struct bcos_sdk_c_transaction* transaction)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction, NULL);

    try
    {
        auto tars_transaction = convert_transaction_struct_to_tars_struct(transaction);
        auto json_str = tars_transaction->writeToJsonString();
        return strdup(json_str.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_transaction_to_json") << LOG_DESC("exception")
                          << LOG_KV("transaction", transaction) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

/**
 * @brief
 *
 * @param sdk
 * @param group_id
 * @return void*
 */
void* bcos_sdk_create_transaction_builder_service(void* sdk, const char* group_id)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(sdk, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, NULL);

    try
    {
        auto service = ((bcos::cppsdk::Sdk*)sdk)->service();
        auto txBuilder = std::make_shared<bcos::cppsdk::utilities::TransactionBuilder>();
        auto transactionBuilderService =
            std::make_unique<bcos::cppsdk::utilities::TransactionBuilderService>(
                service, std::string(group_id), txBuilder);
        return transactionBuilderService.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_transaction_builder_service")
                          << LOG_DESC("exception") << LOG_KV("group_id", group_id)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return NULL;
}

/**
 * @brief
 *
 * @param sdk
 * @param group_id
 * @return void*
 */
void bcos_sdk_destroy_transaction_builder_service(void* service)
{
    bcos_sdk_clear_last_error();
    if (service)
    {
        delete (bcos::cppsdk::utilities::TransactionBuilderService*)service;
    }
}

/**
 * @brief
 *
 * @param tx_builder_service
 * @param to
 * @param data
 * @param abi
 * @return void*
 */
void* bcos_sdk_create_transaction_data_with_tx_builder_service(
    void* tx_builder_service, const char* to, const char* data, const char* abi)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(tx_builder_service, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(data, NULL);

    try
    {
        auto service = (bcos::cppsdk::utilities::TransactionBuilderService*)tx_builder_service;
        auto bytesData = fromHexString(data);
        auto transactionData = service->createTransactionData(
            to ? std::string(to) : "", *bytesData, abi ? std::string(abi) : "");
        return transactionData.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_transaction_data_with_tx_builder_service")
                          << LOG_DESC("exception") << LOG_KV("to", to) << LOG_KV("data", data)
                          << LOG_KV("abi", abi ? abi : "") << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return NULL;
}

/**
 * @brief
 *
 * @param tx_builder_service
 * @param key_pair
 * @param to
 * @param data
 * @param abi
 * @param attribute
 * @param tx_hash
 * @param signed_tx
 * @return void*
 */
void bcos_sdk_create_signed_transaction_with_tx_builder_service(void* tx_builder_service,
    void* key_pair, const char* to, const char* data, const char* abi, int32_t attribute,
    char** tx_hash, char** signed_tx)
{
    return bcos_sdk_create_signed_transaction_with_tx_builder_service_ver_extra_data(
        tx_builder_service, key_pair, to, data, abi, attribute, NULL, tx_hash, signed_tx);
}

/**
 * @brief
 *
 * @param tx_builder_service
 * @param key_pair
 * @param to
 * @param data
 * @param abi
 * @param attribute
 * @param extra_data
 * @param tx_hash
 * @param signed_tx
 * @return void*
 */
void bcos_sdk_create_signed_transaction_with_tx_builder_service_ver_extra_data(
    void* tx_builder_service, void* key_pair, const char* to, const char* data, const char* abi,
    int32_t attribute, const char* extra_data, char** tx_hash, char** signed_tx)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(tx_builder_service, );
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, );
    BCOS_SDK_C_PARAMS_VERIFICATION(data, );
    BCOS_SDK_C_PARAMS_VERIFICATION(tx_hash, );
    BCOS_SDK_C_PARAMS_VERIFICATION(signed_tx, );

    try
    {
        auto bytesData = fromHexString(data);
        auto service = (bcos::cppsdk::utilities::TransactionBuilderService*)tx_builder_service;

        auto r = service->createSignedTransaction(*((bcos::crypto::KeyPairInterface*)key_pair),
            std::string(to ? to : ""), *bytesData, std::string(abi ? abi : ""), attribute);

        *tx_hash = strdup(r.first.c_str());
        *signed_tx = strdup(r.second.c_str());
    }
    catch (std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING)
            << LOG_BADGE(
                   "bcos_sdk_create_signed_transaction_with_tx_builder_service_ver_extra_data")
            << LOG_DESC("exception") << LOG_KV("to", to) << LOG_KV("data", data)
            << LOG_KV("abi", abi ? abi : "") << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
}