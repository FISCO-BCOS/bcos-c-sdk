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
 * @file bcos_sdk_c_uti_tx_struct_v1.cpp
 * @author: kyonGuo
 * @date 2024/3/7
 */

#include "bcos_sdk_c_uti_tx_struct_v1.h"
#include "bcos_sdk_c_error.h"
#include <bcos-c-sdk/bcos_sdk_c_common.h>
#include <bcos-cpp-sdk/Sdk.h>
#include <bcos-cpp-sdk/utilities/crypto/Common.h>
#include <bcos-cpp-sdk/utilities/tx/TransactionBuilder.h>
#include <bcos-cpp-sdk/utilities/tx/TransactionUtils.h>
#include <cstring>
#include <exception>
#include <memory>
#include <system_error>

using namespace bcos;
using namespace bcos::cppsdk;
using namespace bcos::cppsdk::utilities;

struct bcos_sdk_c_transaction_data_v1* transaction_data_copy_v1(
    const bcos_sdk_c_transaction_data_v1* tx_data_src)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(tx_data_src, NULL);

    try
    {
        struct bcos_sdk_c_transaction_data_v1* transaction_data_struct_v1 =
            (struct bcos_sdk_c_transaction_data_v1*)malloc(
                sizeof(struct bcos_sdk_c_transaction_data_v1));
        transaction_data_struct_v1->base.version = tx_data_src->base.version;
        transaction_data_struct_v1->base.block_limit = tx_data_src->base.block_limit;
        transaction_data_struct_v1->base.chain_id = my_strdup(tx_data_src->base.chain_id);
        transaction_data_struct_v1->base.group_id = my_strdup(tx_data_src->base.group_id);
        transaction_data_struct_v1->base.nonce = my_strdup(tx_data_src->base.nonce);
        transaction_data_struct_v1->base.to = my_strdup(tx_data_src->base.to);
        transaction_data_struct_v1->base.abi = my_strdup(tx_data_src->base.abi);
        transaction_data_struct_v1->base.input = bytes_struct_copy(tx_data_src->base.input);
        transaction_data_struct_v1->value = my_strdup(tx_data_src->value);
        transaction_data_struct_v1->gas_price = my_strdup(tx_data_src->gas_price);
        transaction_data_struct_v1->gas_limit = tx_data_src->gas_limit;
        transaction_data_struct_v1->max_fee_per_gas = my_strdup(tx_data_src->max_fee_per_gas);
        transaction_data_struct_v1->max_priority_fee_per_gas =
            my_strdup(tx_data_src->max_priority_fee_per_gas);

        return transaction_data_struct_v1;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("transaction_data_copy_v1") << LOG_DESC("exception")
                          << LOG_KV("tx_data_src", tx_data_src) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

struct bcos_sdk_c_transaction_data_v1* convert_tars_transaction_data_to_struct_v1(
    bcostars::TransactionDataUniquePtr tars_transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(tars_transaction_data, NULL);

    try
    {
        struct bcos_sdk_c_transaction_data_v1* transaction_data_struct_v1 =
            (struct bcos_sdk_c_transaction_data_v1*)malloc(
                sizeof(struct bcos_sdk_c_transaction_data_v1));
        struct bcos_sdk_c_bytes* input_bytes = create_bytes_struct(
            tars_transaction_data->input.size(), tars_transaction_data->input.data());

        transaction_data_struct_v1->base.input = input_bytes;
        transaction_data_struct_v1->base.version = tars_transaction_data->version;
        transaction_data_struct_v1->base.block_limit = tars_transaction_data->blockLimit;
        transaction_data_struct_v1->base.chain_id =
            my_strdup(tars_transaction_data->chainID.data());
        transaction_data_struct_v1->base.group_id =
            my_strdup(tars_transaction_data->groupID.data());
        transaction_data_struct_v1->base.nonce = my_strdup(tars_transaction_data->nonce.data());
        transaction_data_struct_v1->base.to = my_strdup(tars_transaction_data->to.data());
        transaction_data_struct_v1->base.abi = my_strdup(tars_transaction_data->abi.data());
        transaction_data_struct_v1->value = my_strdup(tars_transaction_data->value.data());
        transaction_data_struct_v1->gas_price = my_strdup(tars_transaction_data->gasPrice.data());
        transaction_data_struct_v1->gas_limit = tars_transaction_data->gasLimit;
        transaction_data_struct_v1->max_fee_per_gas =
            my_strdup(tars_transaction_data->maxFeePerGas.data());
        transaction_data_struct_v1->max_priority_fee_per_gas =
            my_strdup(tars_transaction_data->maxPriorityFeePerGas.data());

        return transaction_data_struct_v1;
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

    return NULL;
}

bcostars::TransactionDataUniquePtr convert_transaction_data_to_tars_v1(
    struct bcos_sdk_c_transaction_data_v1* transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data->base.group_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data->base.chain_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data->base.input, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data->value, NULL);

    try
    {
        auto tars_transaction_data_v1 = std::make_unique<bcostars::TransactionData>();
        tars_transaction_data_v1->input.reserve(transaction_data->base.input->length);
        for (size_t i = 0; i < transaction_data->base.input->length; ++i)
        {
            tars_transaction_data_v1->input.push_back(
                static_cast<tars::Char>(transaction_data->base.input->buffer[i]));
        }
        tars_transaction_data_v1->version = (tars::Int32)transaction_data->base.version;
        tars_transaction_data_v1->blockLimit = (tars::Int64)transaction_data->base.block_limit;
        tars_transaction_data_v1->chainID = std::string(transaction_data->base.chain_id);
        tars_transaction_data_v1->groupID = std::string(transaction_data->base.group_id);
        tars_transaction_data_v1->nonce = std::string(transaction_data->base.nonce);
        tars_transaction_data_v1->to = std::string(transaction_data->base.to);
        tars_transaction_data_v1->abi = std::string(transaction_data->base.abi);
        tars_transaction_data_v1->value = std::string(transaction_data->value);
        tars_transaction_data_v1->gasPrice = std::string(transaction_data->gas_price);
        tars_transaction_data_v1->gasLimit = (tars::Int64)transaction_data->gas_limit;
        tars_transaction_data_v1->maxFeePerGas = std::string(transaction_data->max_fee_per_gas);
        tars_transaction_data_v1->maxPriorityFeePerGas =
            std::string(transaction_data->max_priority_fee_per_gas);

        return tars_transaction_data_v1;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("convert_transaction_data_to_tars_v1")
                          << LOG_DESC("exception") << LOG_KV("transaction data", transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

bcostars::TransactionUniquePtr convert_transaction_to_tars_v1(
    struct bcos_sdk_c_transaction_v1* transaction)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction->data_hash, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction->signature, NULL);

    try
    {
        auto tars_transaction = std::make_unique<bcostars::Transaction>();
        auto TransactionDataUniquePtr =
            convert_transaction_data_to_tars_v1(transaction->transaction_data);
        tars_transaction->data = *TransactionDataUniquePtr;
        for (size_t i = 0; i < transaction->data_hash->length; ++i)
        {
            tars_transaction->dataHash.push_back(transaction->data_hash->buffer[i]);
        }
        for (size_t i = 0; i < transaction->signature->length; ++i)
        {
            tars_transaction->signature.push_back(transaction->signature->buffer[i]);
        }
        if (transaction->sender)
        {
            for (size_t i = 0; i < transaction->sender->length; ++i)
            {
                tars_transaction->sender.push_back(transaction->sender->buffer[i]);
            }
        }
        tars_transaction->importTime = (tars::Int32)transaction->import_time;
        tars_transaction->attribute = (tars::Int64)transaction->attribute;
        tars_transaction->extraData = std::string(transaction->extra_data);

        return tars_transaction;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("convert_transaction_to_tars_v1") << LOG_DESC("exception")
                          << LOG_KV("transaction", transaction) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

struct bcos_sdk_c_transaction_v1* convert_tars_transaction_to_struct_v1(
    bcostars::TransactionUniquePtr tars_transaction)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(tars_transaction, NULL);

    try
    {
        struct bcos_sdk_c_transaction_v1* transaction_struct_v1 =
            (struct bcos_sdk_c_transaction_v1*)malloc(sizeof(struct bcos_sdk_c_transaction_v1));
        struct bcos_sdk_c_bytes* data_hash_bytes = create_bytes_struct(
            tars_transaction->dataHash.size(), tars_transaction->dataHash.data());
        struct bcos_sdk_c_bytes* signature_bytes = create_bytes_struct(
            tars_transaction->signature.size(), tars_transaction->signature.data());
        struct bcos_sdk_c_bytes* sender_bytes =
            create_bytes_struct(tars_transaction->sender.size(), tars_transaction->sender.data());
        auto transactionDataUniquePtr =
            std::make_unique<bcostars::TransactionData>(tars_transaction->data);

        transaction_struct_v1->transaction_data =
            convert_tars_transaction_data_to_struct_v1(std::move(transactionDataUniquePtr));
        transaction_struct_v1->data_hash = data_hash_bytes;
        transaction_struct_v1->signature = signature_bytes;
        transaction_struct_v1->sender = sender_bytes;
        transaction_struct_v1->import_time = tars_transaction->importTime;
        transaction_struct_v1->attribute = tars_transaction->attribute;
        transaction_struct_v1->extra_data = my_strdup(tars_transaction->extraData.data());
        return transaction_struct_v1;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("convert_tars_transaction_to_struct_v1")
                          << LOG_DESC("exception") << LOG_KV("transaction", tars_transaction)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

struct bcos_sdk_c_transaction_data_v1* bcos_sdk_create_transaction_data_struct_with_hex_input_v1(
    const char* group_id, const char* chain_id, const char* to, const char* input, const char* abi,
    int64_t block_limit, const char* value, const char* gas_price, int64_t gas_limit,
    const char* max_fee_per_gas, const char* max_priority_fee_per_gas)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(input, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((input[0] != '\0'), "input can not be empty string", NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must > 0", NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(value, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(gas_price, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((gas_limit >= 0), "gas limit must >= 0", NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(max_fee_per_gas, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(max_priority_fee_per_gas, NULL);

    try
    {
        struct bcos_sdk_c_transaction_data_v1* transaction_data_struct_v1 =
            (struct bcos_sdk_c_transaction_data_v1*)malloc(
                sizeof(struct bcos_sdk_c_transaction_data_v1));
        auto bytesInput = fromHexString(input);
        TransactionBuilder builder;
        std::string nonceStr = builder.generateRandomStr();

        transaction_data_struct_v1->base.version = 1;
        transaction_data_struct_v1->base.block_limit = block_limit;
        transaction_data_struct_v1->base.group_id = my_strdup(group_id);
        transaction_data_struct_v1->base.chain_id = my_strdup(chain_id);
        transaction_data_struct_v1->base.to = to ? my_strdup(to) : my_strdup("");
        transaction_data_struct_v1->base.abi = abi ? my_strdup(abi) : my_strdup("");
        transaction_data_struct_v1->base.nonce = my_strdup(nonceStr.data());
        transaction_data_struct_v1->base.input =
            create_bytes_struct(bytesInput->size(), reinterpret_cast<char*>(bytesInput->data()));
        transaction_data_struct_v1->value = my_strdup(value);
        transaction_data_struct_v1->gas_price = my_strdup(gas_price);
        transaction_data_struct_v1->gas_limit = gas_limit;
        transaction_data_struct_v1->max_fee_per_gas = my_strdup(max_fee_per_gas);
        transaction_data_struct_v1->max_priority_fee_per_gas = my_strdup(max_priority_fee_per_gas);

        return transaction_data_struct_v1;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_transaction_data_struct_with_hex_input_v1")
                          << LOG_DESC("exception") << LOG_KV("group_id", group_id)
                          << LOG_KV("chain_id", chain_id) << LOG_KV("to", std::string(to ? to : ""))
                          << LOG_KV("input", input) << LOG_KV("abi", std::string(abi ? abi : ""))
                          << LOG_KV("block_limit", block_limit) << LOG_KV("value", value)
                          << LOG_KV("gas_price", gas_price) << LOG_KV("gas_limit", gas_limit)
                          << LOG_KV("max_fee_per_gas", max_fee_per_gas)
                          << LOG_KV("max_priority_fee_per_gas", max_priority_fee_per_gas)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

struct bcos_sdk_c_transaction_data_v1* bcos_sdk_create_transaction_data_struct_with_bytes_v1(
    const char* group_id, const char* chain_id, const char* to, const unsigned char* bytes_input,
    uint32_t bytes_input_length, const char* abi, int64_t block_limit, const char* value,
    const char* gas_price, int64_t gas_limit, const char* max_fee_per_gas,
    const char* max_priority_fee_per_gas)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(group_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(chain_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(bytes_input, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (bytes_input_length > 0), "bytes input length must > 0", NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_limit > 0), "block limit must > 0", NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(value, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(gas_price, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((gas_limit >= 0), "gas limit must >= 0", NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(max_fee_per_gas, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(max_priority_fee_per_gas, NULL);

    try
    {
        struct bcos_sdk_c_transaction_data_v1* transaction_data_struct_v1 =
            (struct bcos_sdk_c_transaction_data_v1*)malloc(
                sizeof(struct bcos_sdk_c_transaction_data_v1));
        TransactionBuilder builder;
        std::string nonceStr = builder.generateRandomStr();

        transaction_data_struct_v1->base.version = 1;
        transaction_data_struct_v1->base.block_limit = block_limit;
        transaction_data_struct_v1->base.group_id = my_strdup(group_id);
        transaction_data_struct_v1->base.chain_id = my_strdup(chain_id);
        transaction_data_struct_v1->base.to = to ? my_strdup(to) : my_strdup("");
        transaction_data_struct_v1->base.abi = abi ? my_strdup(abi) : my_strdup("");
        transaction_data_struct_v1->base.nonce = my_strdup(nonceStr.data());
        transaction_data_struct_v1->base.input = create_bytes_struct(
            bytes_input_length, const_cast<char*>(reinterpret_cast<const char*>(bytes_input)));
        transaction_data_struct_v1->value = my_strdup(value);
        transaction_data_struct_v1->gas_price = my_strdup(gas_price);
        transaction_data_struct_v1->gas_limit = gas_limit;
        transaction_data_struct_v1->max_fee_per_gas = my_strdup(max_fee_per_gas);
        transaction_data_struct_v1->max_priority_fee_per_gas = my_strdup(max_priority_fee_per_gas);

        return transaction_data_struct_v1;
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

    return NULL;
}

void bcos_sdk_destroy_transaction_data_struct_v1(
    struct bcos_sdk_c_transaction_data_v1* transaction_data)
{
    if (transaction_data == NULL)
    {
        return;
    }

    if (transaction_data->base.chain_id)
    {
        bcos_sdk_c_free(transaction_data->base.chain_id);
    }

    if (transaction_data->base.group_id)
    {
        bcos_sdk_c_free(transaction_data->base.group_id);
    }

    if (transaction_data->base.nonce)
    {
        bcos_sdk_c_free(transaction_data->base.nonce);
    }

    if (transaction_data->base.to)
    {
        bcos_sdk_c_free(transaction_data->base.to);
    }

    if (transaction_data->base.abi)
    {
        bcos_sdk_c_free((void*)transaction_data->base.abi);
    }

    if (transaction_data->base.input)
    {
        if (transaction_data->base.input->buffer)
        {
            bcos_sdk_c_free(transaction_data->base.input->buffer);
        }
        bcos_sdk_c_free((void*)transaction_data->base.input);
    }

    if (transaction_data->value)
    {
        bcos_sdk_c_free(transaction_data->value);
    }

    if (transaction_data->gas_price)
    {
        bcos_sdk_c_free(transaction_data->gas_price);
    }

    if (transaction_data->max_fee_per_gas)
    {
        bcos_sdk_c_free(transaction_data->max_fee_per_gas);
    }

    if (transaction_data->max_priority_fee_per_gas)
    {
        bcos_sdk_c_free(transaction_data->max_priority_fee_per_gas);
    }

    bcos_sdk_c_free(transaction_data);
}

const char* bcos_sdk_encode_transaction_data_struct_to_hex_v1(
    struct bcos_sdk_c_transaction_data_v1* transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);

    try
    {
        auto tars_transaction_data = convert_transaction_data_to_tars_v1(transaction_data);
        TransactionBuilder builder;
        auto encodedTransactionData = builder.encodeTransactionData(*tars_transaction_data);
        auto hex_tx_data_str = bcos::toHexString(*encodedTransactionData);
        return strdup(hex_tx_data_str->c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_encode_transaction_data_struct_to_hex_v1")
                          << LOG_DESC("exception") << LOG_KV("transaction_data", transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

const char* bcos_sdk_encode_transaction_data_struct_to_json_v1(
    struct bcos_sdk_c_transaction_data_v1* transaction_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);

    try
    {
        auto tars_transaction_data_v1 = convert_transaction_data_to_tars_v1(transaction_data);
        auto json_str = TarsTransactionDataWriteToJsonString(*tars_transaction_data_v1);
        return strdup(json_str.c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_encode_transaction_data_struct_to_json_v1")
                          << LOG_DESC("exception") << LOG_KV("transaction_data", transaction_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

struct bcos_sdk_c_transaction_data_v1* bcos_sdk_decode_transaction_data_struct_from_hex_v1(
    const char* transaction_data_hex_str)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data_hex_str, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((transaction_data_hex_str[0] != '\0'),
        "transaction_data_hex_str can not be empty string", NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((strlen(transaction_data_hex_str) % 2 != 1),
        "the length of transaction_data_hex_str must be an even number", NULL);

    try
    {
        TransactionBuilder builder;
        auto tx_data_bytes = fromHexString(transaction_data_hex_str);
        auto tars_tx_data_v1 = builder.decodeTransactionData(*tx_data_bytes);
        return convert_tars_transaction_data_to_struct_v1(std::move(tars_tx_data_v1));
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_decode_transaction_data_struct_v1")
                          << LOG_DESC("exception") << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

struct bcos_sdk_c_transaction_data_v1* bcos_sdk_decode_transaction_data_struct_from_json_v1(
    const char* transaction_data_json_str)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data_json_str, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((transaction_data_json_str[0] != '\0'),
        "transaction_data_json_str can not be empty string", NULL);

    try
    {
        TransactionBuilder builder;
        auto transactionData =
            builder.createTransactionDataWithJson(std::string(transaction_data_json_str));

        return convert_tars_transaction_data_to_struct_v1(std::move(transactionData));
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_decode_transaction_data_struct_from_json_v1")
                          << LOG_DESC("exception")
                          << LOG_KV("transaction_data_json_str", transaction_data_json_str)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

const char* bcos_sdk_calc_transaction_data_struct_hash_v1(
    int crypto_type, struct bcos_sdk_c_transaction_data_v1* transaction_data)
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
        auto tars_transaction_data_v1 = convert_transaction_data_to_tars_v1(transaction_data);
        TransactionBuilder builder;
        auto transactionDataHash = builder.calculateTransactionDataHash(
            crypto_type == BCOS_C_SDK_ECDSA_TYPE ? CryptoType::Secp256K1 : CryptoType::SM2,
            *tars_transaction_data_v1);
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

    return NULL;
}

const char* bcos_sdk_calc_transaction_data_struct_hash_with_hex_v1(
    int crypto_type, const char* transaction_data_hex)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data_hex, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (transaction_data_hex[0] != '\0'), "transaction_data_hex can not be empty string", NULL);

    try
    {
        struct bcos_sdk_c_transaction_data_v1* transaction_data_struct_v1 =
            bcos_sdk_decode_transaction_data_struct_from_hex_v1(transaction_data_hex);
        auto tars_transaction_data_v1 =
            convert_transaction_data_to_tars_v1(transaction_data_struct_v1);
        TransactionBuilder builder;
        auto transactionDataHash = builder.calculateTransactionDataHash(
            crypto_type == BCOS_C_SDK_ECDSA_TYPE ? CryptoType::Secp256K1 : CryptoType::SM2,
            *tars_transaction_data_v1);
        return strdup(bcos::toHexStringWithPrefix(transactionDataHash).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_calc_transaction_data_struct_hash_with_hex_v1")
                          << LOG_DESC("exception") << LOG_KV("crypto_type", crypto_type)
                          << LOG_KV("transaction_data_hex", transaction_data_hex)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

struct bcos_sdk_c_transaction_v1* bcos_sdk_create_transaction_struct_v1(
    struct bcos_sdk_c_transaction_data_v1* transaction_data, const char* signature,
    const char* transaction_data_hash, int32_t attribute, const char* extra_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(signature, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data_hash, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (signature[0] != '\0'), "signature can not be empty string", NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (transaction_data_hash[0] != '\0'), "transaction_data_hash can not be empty string", NULL);

    try
    {
        struct bcos_sdk_c_transaction_v1* transaction_struct_v1 =
            (struct bcos_sdk_c_transaction_v1*)malloc(sizeof(struct bcos_sdk_c_transaction_v1));
        transaction_struct_v1->transaction_data = transaction_data_copy_v1(transaction_data);
        transaction_struct_v1->sender = NULL;
        transaction_struct_v1->import_time = 0;
        transaction_struct_v1->attribute = attribute;
        transaction_struct_v1->extra_data = my_strdup(extra_data);
        // signature
        auto signatureWithoutHex = fromHexString(signature);
        struct bcos_sdk_c_bytes* signature_bytes =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        signature_bytes->length = signatureWithoutHex->size();
        signature_bytes->buffer = (uint8_t*)malloc(signatureWithoutHex->size());
        memcpy(signature_bytes->buffer, signatureWithoutHex->data(), signatureWithoutHex->size());
        transaction_struct_v1->signature = signature_bytes;
        // data_hash
        auto dataHashArray = bcos::crypto::HashType(transaction_data_hash);
        struct bcos_sdk_c_bytes* data_hash_bytes =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        data_hash_bytes->length = dataHashArray.size();
        data_hash_bytes->buffer = (uint8_t*)malloc(dataHashArray.size());
        memcpy(data_hash_bytes->buffer, dataHashArray.data(), dataHashArray.size());
        transaction_struct_v1->data_hash = data_hash_bytes;

        return transaction_struct_v1;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_transaction_struct_v1")
                          << LOG_DESC("exception") << LOG_KV("signature", signature)
                          << LOG_KV("transaction_data_hash", transaction_data_hash)
                          << LOG_KV("attribute", attribute) << LOG_KV("extra_data", extra_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

void bcos_sdk_destroy_transaction_struct_v1(struct bcos_sdk_c_transaction_v1* transaction)
{
    if (transaction == NULL)
    {
        return;
    }

    if (transaction && transaction->transaction_data)
    {
        bcos_sdk_destroy_transaction_data_struct_v1(transaction->transaction_data);
    }

    if (transaction && transaction->data_hash)
    {
        if (transaction->data_hash->buffer)
        {
            bcos_sdk_c_free(transaction->data_hash->buffer);
        }
        bcos_sdk_c_free(transaction->data_hash);
    }

    if (transaction && transaction->signature)
    {
        if (transaction->signature->buffer)
        {
            bcos_sdk_c_free(transaction->signature->buffer);
        }
        bcos_sdk_c_free(transaction->signature);
    }

    if (transaction && transaction->sender)
    {
        if (transaction->sender->buffer)
        {
            bcos_sdk_c_free(transaction->sender->buffer);
        }
        bcos_sdk_c_free(transaction->sender);
    }

    if (transaction && transaction->extra_data)
    {
        bcos_sdk_c_free(transaction->extra_data);
    }

    bcos_sdk_c_free(transaction);
}

const char* bcos_sdk_create_encoded_transaction_v1(
    struct bcos_sdk_c_transaction_data_v1* transaction_data, const char* signature,
    const char* transaction_data_hash, int32_t attribute, const char* extra_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(signature, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_data_hash, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (signature[0] != '\0'), "signature can not be empty string", NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (transaction_data_hash[0] != '\0'), "transaction_data_hash can not be empty string", NULL);

    try
    {
        TransactionBuilder builder;
        auto tars_tx_data_v1 = convert_transaction_data_to_tars_v1(transaction_data);
        auto signedBytes = builder.createSignedTransaction(*tars_tx_data_v1,
            *fromHexString(signature), bcos::crypto::HashType(transaction_data_hash), attribute,
            extra_data ? std::string(extra_data) : std::string());
        return strdup(bcos::toHexStringWithPrefix(*signedBytes).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_encoded_transaction_v1")
                          << LOG_DESC("exception") << LOG_KV("signature", signature)
                          << LOG_KV("transaction_data_hash", transaction_data_hash)
                          << LOG_KV("attribute", attribute) << LOG_KV("extra_data", extra_data)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return NULL;
}

const char* bcos_sdk_encode_transaction_struct_to_hex_v1(
    struct bcos_sdk_c_transaction_v1* transaction)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction, NULL);

    try
    {
        auto tars_transaction_v1 = convert_transaction_to_tars_v1(transaction);
        TransactionBuilder builder;
        auto encodedTransaction = builder.encodeTransaction(*tars_transaction_v1);
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

    return NULL;
}

const char* bcos_sdk_encode_transaction_struct_to_json_v1(
    struct bcos_sdk_c_transaction_v1* transaction)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction, NULL);

    try
    {
        auto tars_transaction_v1 = convert_transaction_to_tars_v1(transaction);
        auto json_str = TarsTransactionWriteToJsonString(tars_transaction_v1);
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

    return NULL;
}

struct bcos_sdk_c_transaction_v1* bcos_sdk_decode_transaction_struct_from_hex_v1(
    const char* transaction_hex_str)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_hex_str, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (transaction_hex_str[0] != '\0'), "transaction_hex_str can not be empty string", NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((strlen(transaction_hex_str) % 2 != 1),
        "the length of transaction_hex_str must be an even number", NULL);

    try
    {
        TransactionBuilder builder;
        auto tx_bytes = fromHexString(transaction_hex_str);
        auto tars_tx = builder.decodeTransaction(*tx_bytes);
        return convert_tars_transaction_to_struct_v1(std::move(tars_tx));
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

    return NULL;
}

struct bcos_sdk_c_transaction_v1* bcos_sdk_decode_transaction_struct_from_json_v1(
    const char* transaction_json_str)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(transaction_json_str, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (transaction_json_str[0] != '\0'), "transaction_json_str can not be empty string", NULL);

    try
    {
        TransactionBuilder builder;
        auto transaction = builder.createTransactionWithJson(std::string(transaction_json_str));

        return convert_tars_transaction_to_struct_v1(std::move(transaction));
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

    return NULL;
}