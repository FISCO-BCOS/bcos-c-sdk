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
 * @file bcos_sdk_c_uti_tx_struct_v2.cpp
 * @author: kyonGuo
 * @date 2024/3/7
 */

#include "bcos_sdk_c_uti_tx_struct_v2.h"
#include "bcos_sdk_c_error.h"
#include <bcos-c-sdk/bcos_sdk_c_common.h>
#include <bcos-cpp-sdk/Sdk.h>
#include <bcos-cpp-sdk/utilities/crypto/Common.h>
#include <bcos-cpp-sdk/utilities/tx/TransactionBuilderV2.h>
#include <bcos-cpp-sdk/utilities/tx/TransactionUtils.h>
#include <utilities/tx/TransactionBuilderV1.h>

#include <cstring>
#include <exception>
#include <memory>
#include <system_error>

using namespace bcos;
using namespace bcos::cppsdk;
using namespace bcos::cppsdk::utilities;

// tx data struct
struct bcos_sdk_c_transaction_data_v2* transaction_data_copy_v2(
    const bcos_sdk_c_transaction_data_v2* tx_data_src)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(tx_data_src, nullptr);

    try
    {
        struct bcos_sdk_c_transaction_data_v2* transaction_data_struct_v2 =
            (struct bcos_sdk_c_transaction_data_v2*)malloc(
                sizeof(struct bcos_sdk_c_transaction_data_v2));
        transaction_data_struct_v2->base_v1.base.version = tx_data_src->base_v1.base.version;
        transaction_data_struct_v2->base_v1.base.block_limit =
            tx_data_src->base_v1.base.block_limit;
        transaction_data_struct_v2->base_v1.base.chain_id =
            my_strdup(tx_data_src->base_v1.base.chain_id);
        transaction_data_struct_v2->base_v1.base.group_id =
            my_strdup(tx_data_src->base_v1.base.group_id);
        transaction_data_struct_v2->base_v1.base.nonce = my_strdup(tx_data_src->base_v1.base.nonce);
        transaction_data_struct_v2->base_v1.base.to = my_strdup(tx_data_src->base_v1.base.to);
        transaction_data_struct_v2->base_v1.base.abi = my_strdup(tx_data_src->base_v1.base.abi);
        transaction_data_struct_v2->base_v1.base.input =
            bytes_struct_copy(tx_data_src->base_v1.base.input);
        transaction_data_struct_v2->base_v1.value = my_strdup(tx_data_src->base_v1.value);
        transaction_data_struct_v2->base_v1.gas_price = my_strdup(tx_data_src->base_v1.gas_price);
        transaction_data_struct_v2->base_v1.gas_limit = tx_data_src->base_v1.gas_limit;
        transaction_data_struct_v2->base_v1.max_fee_per_gas =
            my_strdup(tx_data_src->base_v1.max_fee_per_gas);
        transaction_data_struct_v2->base_v1.max_priority_fee_per_gas =
            my_strdup(tx_data_src->base_v1.max_priority_fee_per_gas);
        if (tx_data_src->extension && tx_data_src->extension->buffer)
        {
            transaction_data_struct_v2->extension = bytes_struct_copy(tx_data_src->extension);
        }

        return transaction_data_struct_v2;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("transaction_data_copy_v1") << LOG_DESC("exception")
                          << LOG_KV("tx_data_src", tx_data_src) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}

struct bcos_sdk_c_transaction_data_v2* convert_tars_transaction_data_to_struct_v2(
    bcostars::TransactionDataUniquePtr tars_transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(tars_transaction_data, nullptr);

    try
    {
        struct bcos_sdk_c_transaction_data_v2* transaction_data_struct_v2 =
            (struct bcos_sdk_c_transaction_data_v2*)malloc(
                sizeof(struct bcos_sdk_c_transaction_data_v2));
        struct bcos_sdk_c_bytes* input_bytes = create_bytes_struct(
            tars_transaction_data->input.size(), tars_transaction_data->input.data());

        transaction_data_struct_v2->base_v1.base.input = input_bytes;
        transaction_data_struct_v2->base_v1.base.version = tars_transaction_data->version;
        transaction_data_struct_v2->base_v1.base.block_limit = tars_transaction_data->blockLimit;
        transaction_data_struct_v2->base_v1.base.chain_id =
            my_strdup(tars_transaction_data->chainID.data());
        transaction_data_struct_v2->base_v1.base.group_id =
            my_strdup(tars_transaction_data->groupID.data());
        transaction_data_struct_v2->base_v1.base.nonce =
            my_strdup(tars_transaction_data->nonce.data());
        transaction_data_struct_v2->base_v1.base.to = my_strdup(tars_transaction_data->to.data());
        transaction_data_struct_v2->base_v1.base.abi = my_strdup(tars_transaction_data->abi.data());
        transaction_data_struct_v2->base_v1.value = my_strdup(tars_transaction_data->value.data());
        transaction_data_struct_v2->base_v1.gas_price =
            my_strdup(tars_transaction_data->gasPrice.data());
        transaction_data_struct_v2->base_v1.gas_limit = tars_transaction_data->gasLimit;
        transaction_data_struct_v2->base_v1.max_fee_per_gas =
            my_strdup(tars_transaction_data->maxFeePerGas.data());
        transaction_data_struct_v2->base_v1.max_priority_fee_per_gas =
            my_strdup(tars_transaction_data->maxPriorityFeePerGas.data());

        struct bcos_sdk_c_bytes* extension_bytes = create_bytes_struct(
            tars_transaction_data->extension.size(), tars_transaction_data->extension.data());
        transaction_data_struct_v2->extension = extension_bytes;

        return transaction_data_struct_v2;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("convert_tars_transaction_data_to_struct_v1")
                          << LOG_DESC("exception")
                          << LOG_KV("transaction data", tars_transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}

bcostars::TransactionDataUniquePtr convert_transaction_data_to_tars_v2(
    struct bcos_sdk_c_transaction_data_v2* transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data->base_v1.base.group_id, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data->base_v1.base.chain_id, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data->base_v1.base.input, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data->base_v1.base.input->buffer, nullptr);

    try
    {
        auto tars_transaction_data = std::make_unique<bcostars::TransactionData>();
        tars_transaction_data->input.reserve(transaction_data->base_v1.base.input->length);
        for (size_t i = 0; i < transaction_data->base_v1.base.input->length; ++i)
        {
            tars_transaction_data->input.push_back(
                static_cast<tars::Char>(transaction_data->base_v1.base.input->buffer[i]));
        }
        tars_transaction_data->version = (tars::Int32)transaction_data->base_v1.base.version;
        tars_transaction_data->blockLimit =
            static_cast<tars::Int64>(transaction_data->base_v1.base.block_limit);
        tars_transaction_data->chainID = transaction_data->base_v1.base.chain_id;
        tars_transaction_data->groupID = transaction_data->base_v1.base.group_id;
        tars_transaction_data->nonce =
            transaction_data->base_v1.base.nonce ? transaction_data->base_v1.base.nonce : "";
        tars_transaction_data->to =
            transaction_data->base_v1.base.to ? transaction_data->base_v1.base.to : "";
        tars_transaction_data->abi =
            transaction_data->base_v1.base.abi ? transaction_data->base_v1.base.abi : "";
        tars_transaction_data->value =
            transaction_data->base_v1.value ? transaction_data->base_v1.value : "";
        tars_transaction_data->gasPrice =
            transaction_data->base_v1.gas_price ? transaction_data->base_v1.gas_price : "";
        tars_transaction_data->gasLimit =
            static_cast<tars::Int64>(transaction_data->base_v1.gas_limit);
        tars_transaction_data->maxFeePerGas = transaction_data->base_v1.max_fee_per_gas ?
                                                  transaction_data->base_v1.max_fee_per_gas :
                                                  "";
        tars_transaction_data->maxPriorityFeePerGas =
            transaction_data->base_v1.max_priority_fee_per_gas ?
                transaction_data->base_v1.max_priority_fee_per_gas :
                "";

        if (transaction_data->extension && transaction_data->extension->buffer)
        {
            tars_transaction_data->extension.reserve(transaction_data->extension->length);
            for (size_t i = 0; i < transaction_data->extension->length; ++i)
            {
                tars_transaction_data->extension.push_back(
                    static_cast<tars::Char>(transaction_data->extension->buffer[i]));
            }
        }
        return tars_transaction_data;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("convert_transaction_data_to_tars_v1")
                          << LOG_DESC("exception") << LOG_KV("transaction data", transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}

// tx struct
bcostars::TransactionUniquePtr convert_transaction_to_tars_v2(
    struct bcos_sdk_c_transaction_v2* transaction)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction->data_hash, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction->data_hash->buffer, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction->signature, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction->signature->buffer, nullptr);

    try
    {
        auto tars_transaction = std::make_unique<bcostars::Transaction>();
        auto TransactionDataUniquePtr =
            convert_transaction_data_to_tars_v2(transaction->transaction_data);
        tars_transaction->data = *TransactionDataUniquePtr;
        tars_transaction->dataHash.reserve(transaction->data_hash->length);
        for (size_t i = 0; i < transaction->data_hash->length; ++i)
        {
            tars_transaction->dataHash.push_back(
                static_cast<tars::Char>(transaction->data_hash->buffer[i]));
        }
        tars_transaction->signature.reserve(transaction->signature->length);
        for (size_t i = 0; i < transaction->signature->length; ++i)
        {
            tars_transaction->signature.push_back(
                static_cast<tars::Char>(transaction->signature->buffer[i]));
        }
        if (transaction->sender && transaction->sender->buffer)
        {
            tars_transaction->sender.reserve(transaction->sender->length);
            for (size_t i = 0; i < transaction->sender->length; ++i)
            {
                tars_transaction->sender.push_back(
                    static_cast<tars::Char>(transaction->sender->buffer[i]));
            }
        }
        tars_transaction->importTime = static_cast<tars::Int32>(transaction->import_time);
        tars_transaction->attribute = transaction->attribute;
        tars_transaction->extraData = transaction->extra_data ? transaction->extra_data : "";

        return tars_transaction;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("convert_transaction_to_tars_v1") << LOG_DESC("exception")
                          << LOG_KV("transaction", transaction) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}

struct bcos_sdk_c_transaction_v2* convert_tars_transaction_to_struct_v2(
    bcostars::TransactionUniquePtr tars_transaction)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(tars_transaction, nullptr);

    try
    {
        struct bcos_sdk_c_transaction_v2* transaction_struct_v2 =
            (struct bcos_sdk_c_transaction_v2*)malloc(sizeof(struct bcos_sdk_c_transaction_v2));
        struct bcos_sdk_c_bytes* data_hash_bytes = create_bytes_struct(
            tars_transaction->dataHash.size(), tars_transaction->dataHash.data());
        struct bcos_sdk_c_bytes* signature_bytes = create_bytes_struct(
            tars_transaction->signature.size(), tars_transaction->signature.data());
        struct bcos_sdk_c_bytes* sender_bytes =
            create_bytes_struct(tars_transaction->sender.size(), tars_transaction->sender.data());
        auto transactionDataUniquePtr =
            std::make_unique<bcostars::TransactionData>(tars_transaction->data);

        transaction_struct_v2->transaction_data =
            convert_tars_transaction_data_to_struct_v2(std::move(transactionDataUniquePtr));
        transaction_struct_v2->data_hash = data_hash_bytes;
        transaction_struct_v2->signature = signature_bytes;
        transaction_struct_v2->sender = sender_bytes;
        transaction_struct_v2->import_time = tars_transaction->importTime;
        transaction_struct_v2->attribute = tars_transaction->attribute;
        transaction_struct_v2->extra_data = my_strdup(tars_transaction->extraData.data());
        return transaction_struct_v2;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("convert_tars_transaction_to_struct_v1")
                          << LOG_DESC("exception") << LOG_KV("transaction", tars_transaction)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}

struct bcos_sdk_c_transaction_data_v2* bcos_sdk_create_transaction_data_struct_v2(
    transaction_version version, const char* group_id, const char* chain_id, const char* to,
    const char* nonce, const unsigned char* bytes_input, uint32_t bytes_input_length,
    const char* abi, int64_t block_limit, const char* value, const char* gas_price,
    int64_t gas_limit, const char* max_fee_per_gas, const char* max_priority_fee_per_gas,
    const unsigned char* extension, uint32_t extension_length)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(bytes_input, nullptr);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (bytes_input_length > 0), "bytes input length must > 0", nullptr);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must > 0", nullptr);

    try
    {
        struct bcos_sdk_c_transaction_data_v2* transaction_data_struct_v2 =
            (struct bcos_sdk_c_transaction_data_v2*)malloc(
                sizeof(struct bcos_sdk_c_transaction_data_v2));
        TransactionBuilder builder;
        std::string nonceStr = builder.generateRandomStr();

        transaction_data_struct_v2->base_v1.base.version = version;
        transaction_data_struct_v2->base_v1.base.block_limit = block_limit;
        transaction_data_struct_v2->base_v1.base.group_id = my_strdup(group_id);
        transaction_data_struct_v2->base_v1.base.chain_id = my_strdup(chain_id);
        transaction_data_struct_v2->base_v1.base.to = to ? my_strdup(to) : my_strdup("");
        transaction_data_struct_v2->base_v1.base.abi = abi ? my_strdup(abi) : my_strdup("");
        transaction_data_struct_v2->base_v1.base.nonce =
            nonce ? my_strdup(nonce) : my_strdup(nonceStr.data());
        transaction_data_struct_v2->base_v1.base.input =
            create_bytes_struct(bytes_input_length, reinterpret_cast<const char*>(bytes_input));
        transaction_data_struct_v2->base_v1.value = my_strdup(value);
        transaction_data_struct_v2->base_v1.gas_price = my_strdup(gas_price);
        transaction_data_struct_v2->base_v1.gas_limit = gas_limit;
        transaction_data_struct_v2->base_v1.max_fee_per_gas = my_strdup(max_fee_per_gas);
        transaction_data_struct_v2->base_v1.max_priority_fee_per_gas =
            my_strdup(max_priority_fee_per_gas);
        if (extension && extension_length > 0)
        {
            transaction_data_struct_v2->extension =
                create_bytes_struct(extension_length, reinterpret_cast<const char*>(extension));
        }
        return transaction_data_struct_v2;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_transaction_data_struct_with_bytes_v1")
                          << LOG_DESC("exception") << LOG_KV("group_id", group_id)
                          << LOG_KV("chain_id", chain_id) << LOG_KV("to", std::string(to ? to : ""))
                          << LOG_KV("bytes_input", bytes_input)
                          << LOG_KV("bytes_input_length", bytes_input_length)
                          << LOG_KV("abi", std::string(abi ? abi : ""))
                          << LOG_KV("block_limit", block_limit) << LOG_KV("value", value)
                          << LOG_KV("gas_price", gas_price) << LOG_KV("gas_limit", gas_limit)
                          << LOG_KV("max_fee_per_gas", max_fee_per_gas)
                          << LOG_KV("max_priority_fee_per_gas", max_priority_fee_per_gas)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}

void bcos_sdk_destroy_transaction_data_struct_v2(
    struct bcos_sdk_c_transaction_data_v2* transaction_data)
{
    if (transaction_data == nullptr)
    {
        return;
    }

    if (transaction_data->base_v1.base.chain_id)
    {
        bcos_sdk_c_free(transaction_data->base_v1.base.chain_id);
    }

    if (transaction_data->base_v1.base.group_id)
    {
        bcos_sdk_c_free(transaction_data->base_v1.base.group_id);
    }

    if (transaction_data->base_v1.base.nonce)
    {
        bcos_sdk_c_free(transaction_data->base_v1.base.nonce);
    }

    if (transaction_data->base_v1.base.to)
    {
        bcos_sdk_c_free(transaction_data->base_v1.base.to);
    }

    if (transaction_data->base_v1.base.abi)
    {
        bcos_sdk_c_free((void*)transaction_data->base_v1.base.abi);
    }

    if (transaction_data->base_v1.base.input)
    {
        if (transaction_data->base_v1.base.input->buffer)
        {
            bcos_sdk_c_free(transaction_data->base_v1.base.input->buffer);
        }
        bcos_sdk_c_free((void*)transaction_data->base_v1.base.input);
    }

    if (transaction_data->base_v1.value)
    {
        bcos_sdk_c_free(transaction_data->base_v1.value);
    }

    if (transaction_data->base_v1.gas_price)
    {
        bcos_sdk_c_free(transaction_data->base_v1.gas_price);
    }

    if (transaction_data->base_v1.max_fee_per_gas)
    {
        bcos_sdk_c_free(transaction_data->base_v1.max_fee_per_gas);
    }

    if (transaction_data->base_v1.max_priority_fee_per_gas)
    {
        bcos_sdk_c_free(transaction_data->base_v1.max_priority_fee_per_gas);
    }

    if (transaction_data->extension)
    {
        if (transaction_data->extension->buffer)
        {
            bcos_sdk_c_free(transaction_data->extension->buffer);
        }
        bcos_sdk_c_free(transaction_data->extension);
    }

    bcos_sdk_c_free(transaction_data);
}

const char* bcos_sdk_calc_transaction_data_struct_hash_v2(
    int crypto_type, struct bcos_sdk_c_transaction_data_v2* transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, nullptr);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        nullptr);
    try
    {
        auto tars_transaction_data_v2 = convert_transaction_data_to_tars_v2(transaction_data);
        TransactionBuilderV1 builder;
        auto transactionDataHash = builder.calculateTransactionDataHash(
            crypto_type == BCOS_C_SDK_ECDSA_TYPE ? CryptoType::Secp256K1 : CryptoType::SM2,
            *tars_transaction_data_v2);
        return strdup(bcos::toHexStringWithPrefix(transactionDataHash).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_calc_transaction_data_struct_hash_v1")
                          << LOG_DESC("exception") << LOG_KV("crypto_type", crypto_type)
                          << LOG_KV("transaction_data", transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}

// transaction
struct bcos_sdk_c_transaction_v2* bcos_sdk_create_transaction_struct_v2(
    struct bcos_sdk_c_transaction_data_v2* transaction_data, const unsigned char* signature,
    uint32_t sign_length, const unsigned char* tx_data_hash, uint32_t hash_length,
    int32_t attribute, const char* extra_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(signature, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(tx_data_hash, nullptr);

    try
    {
        struct bcos_sdk_c_transaction_v2* transaction_struct_v2 =
            (struct bcos_sdk_c_transaction_v2*)malloc(sizeof(struct bcos_sdk_c_transaction_v2));
        transaction_struct_v2->transaction_data = transaction_data_copy_v2(transaction_data);
        transaction_struct_v2->sender = nullptr;
        transaction_struct_v2->import_time = 0;
        transaction_struct_v2->attribute = attribute;
        transaction_struct_v2->extra_data = my_strdup(extra_data);
        // signature
        struct bcos_sdk_c_bytes* signature_bytes =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        signature_bytes->length = sign_length;
        signature_bytes->buffer = (uint8_t*)malloc(sign_length);
        memcpy(signature_bytes->buffer, signature, sign_length);
        transaction_struct_v2->signature = signature_bytes;
        // data_hash
        struct bcos_sdk_c_bytes* data_hash_bytes =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        data_hash_bytes->length = hash_length;
        data_hash_bytes->buffer = (uint8_t*)malloc(hash_length);
        memcpy(data_hash_bytes->buffer, tx_data_hash, hash_length);
        transaction_struct_v2->data_hash = data_hash_bytes;

        return transaction_struct_v2;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_transaction_struct_v1")
                          << LOG_DESC("exception") << LOG_KV("attribute", attribute)
                          << LOG_KV("extra_data", extra_data) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}

void bcos_sdk_destroy_transaction_struct_v2(struct bcos_sdk_c_transaction_v2* transaction)
{
    if (transaction == nullptr)
    {
        return;
    }

    if (transaction->transaction_data)
    {
        bcos_sdk_destroy_transaction_data_struct_v2(transaction->transaction_data);
    }

    if (transaction->data_hash)
    {
        if (transaction->data_hash->buffer)
        {
            bcos_sdk_c_free(transaction->data_hash->buffer);
        }
        bcos_sdk_c_free(transaction->data_hash);
    }

    if (transaction->signature)
    {
        if (transaction->signature->buffer)
        {
            bcos_sdk_c_free(transaction->signature->buffer);
        }
        bcos_sdk_c_free(transaction->signature);
    }

    if (transaction->sender)
    {
        if (transaction->sender->buffer)
        {
            bcos_sdk_c_free(transaction->sender->buffer);
        }
        bcos_sdk_c_free(transaction->sender);
    }

    if (transaction->extra_data)
    {
        bcos_sdk_c_free(transaction->extra_data);
    }

    bcos_sdk_c_free(transaction);
}

const char* bcos_sdk_create_encoded_transaction_v2(
    struct bcos_sdk_c_transaction_data_v2* transaction_data, const char* signature,
    const char* transaction_data_hash, int32_t attribute, const char* extra_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(signature, nullptr);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data_hash, nullptr);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (signature[0] != '\0'), "signature can not be empty string", nullptr);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((transaction_data_hash[0] != '\0'),
        "transaction_data_hash can not be empty string", nullptr);

    try
    {
        TransactionBuilder builder;
        auto tars_tx_data_v1 = convert_transaction_data_to_tars_v2(transaction_data);
        auto signedBytes = builder.createSignedTransaction(*tars_tx_data_v1,
            *fromHexString(signature), bcos::crypto::HashType(transaction_data_hash), attribute,
            extra_data ? std::string(extra_data) : std::string());
        return strdup(bcos::toHexStringWithPrefix(*signedBytes).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_encoded_transaction_v1")
                          << LOG_DESC("exception") << LOG_KV("attribute", attribute)
                          << LOG_KV("extra_data", extra_data) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return nullptr;
}

const char* bcos_sdk_encode_transaction_struct_to_hex_v2(
    struct bcos_sdk_c_transaction_v2* transaction)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction, nullptr);

    try
    {
        auto tars_transaction_v2 = convert_transaction_to_tars_v2(transaction);
        TransactionBuilder builder;
        auto encodedTransaction = builder.encodeTransaction(*tars_transaction_v2);
        auto hex_tx_str = toHexStringWithPrefix(*encodedTransaction);
        return strdup(hex_tx_str.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_encode_transaction_struct_to_hex_v1")
                          << LOG_DESC("exception") << LOG_KV("transaction", transaction)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}

const char* bcos_sdk_encode_transaction_struct_to_json_v2(
    struct bcos_sdk_c_transaction_v2* transaction)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction, nullptr);

    try
    {
        auto tars_transaction_v2 = convert_transaction_to_tars_v2(transaction);
        auto json_str = TarsTransactionWriteToJsonString(tars_transaction_v2);
        return strdup(json_str.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_encode_transaction_struct_to_json_v1")
                          << LOG_DESC("exception") << LOG_KV("transaction", transaction)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}

struct bcos_sdk_c_transaction_v2* bcos_sdk_decode_transaction_struct_from_hex_v2(
    const char* transaction_hex_str)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_hex_str, nullptr);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (transaction_hex_str[0] != '\0'), "transaction_hex_str can not be empty string", NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((strlen(transaction_hex_str) % 2 != 1),
        "the length of transaction_hex_str must be an even number", nullptr);

    try
    {
        TransactionBuilder builder;
        auto tx_bytes = fromHexString(transaction_hex_str);
        auto tars_tx = builder.decodeTransaction(*tx_bytes);
        return convert_tars_transaction_to_struct_v2(std::move(tars_tx));
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_decode_transaction_struct")
                          << LOG_DESC("exception")
                          << LOG_KV("transaction_hex_str", transaction_hex_str)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}

struct bcos_sdk_c_transaction_v2* bcos_sdk_decode_transaction_struct_from_json_v2(
    const char* transaction_json_str)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_json_str, nullptr);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (transaction_json_str[0] != '\0'), "transaction_json_str can not be empty string", NULL);

    try
    {
        TransactionBuilder builder;
        auto transaction = builder.createTransactionWithJson(std::string(transaction_json_str));

        return convert_tars_transaction_to_struct_v2(std::move(transaction));
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_decode_transaction_struct_from_json_v1")
                          << LOG_DESC("exception")
                          << LOG_KV("transaction_json_str", transaction_json_str)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return nullptr;
}