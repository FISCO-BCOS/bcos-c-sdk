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
 * @file bcos_sdk_c_uti_receipt.cpp
 * @author: kyonGuo
 * @date 2022/12/7
 */

#include "bcos_sdk_c_uti_receipt.h"
#include "bcos_sdk_c_error.h"
#include <bcos-cpp-sdk/Sdk.h>
#include <bcos-cpp-sdk/utilities/crypto/Common.h>
#include <bcos-cpp-sdk/utilities/receipt/ReceiptBuilder.h>
#include <cstring>
#include <exception>
#include <memory>
#include <system_error>

using namespace bcos;
using namespace bcos::cppsdk;
using namespace bcos::cppsdk::utilities;

void* bcos_sdk_create_receipt_data(
    const char* gas_used, const char* contract_address, const char* out_put, int64_t block_number)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(gas_used, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(contract_address, NULL);
    // BCOS_SDK_C_PARAMS_VERIFICATION(to, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(out_put, NULL);
    // BCOS_SDK_C_PARAMS_VERIFICATION(abi, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION((block_number > 0), "block number must >= 0", NULL);

    try
    {
        auto bytesData = fromHexString(out_put);
        auto builder = std::make_shared<ReceiptBuilder>();
        auto receiptData = builder->createReceiptData(
            std::string(gas_used), std::string(contract_address), *bytesData, block_number);
        return receiptData.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_receipt_data") << LOG_DESC("exception")
                          << LOG_KV("gas_used", gas_used)
                          << LOG_KV("contract_address", contract_address)
                          << LOG_KV("out_put", out_put) << LOG_KV("block_number", block_number)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

void* bcos_sdk_create_receipt_data_with_json(const char* json)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(json, NULL);

    try
    {
        auto builder = std::make_shared<ReceiptBuilder>();
        auto receiptData = builder->createReceiptDataWithJson(std::string(json));
        return receiptData.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_create_receipt_data") << LOG_DESC("exception")
                          << LOG_KV("json", json)
                          << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

void bcos_sdk_destroy_receipt_data(void* receipt_data)
{
    if (receipt_data)
    {
        delete (bcostars::TransactionReceiptData*)receipt_data;
        receipt_data = NULL;
    }
}

const char* bcos_sdk_encode_receipt_data(void* receipt_data)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(receipt_data, NULL);

    try
    {
        auto builder = std::make_shared<ReceiptBuilder>();
        auto receiptData = builder->encodeReceipt(*(bcostars::TransactionReceiptData*)receipt_data);
        return strdup(toHexString(*receiptData)->c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_encode_transaction_data") << LOG_DESC("exception")
                          << LOG_KV("receipt_data", receipt_data) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }

    return NULL;
}

const char* bcos_sdk_decode_receipt_data(const char* receipt_hex_str)
{
    BCOS_SDK_C_PARAMS_VERIFICATION(receipt_hex_str, NULL);
    try
    {
        auto builder = std::make_unique<ReceiptBuilder>();
        auto rpt_bytes = fromHexString(receipt_hex_str);
        auto json = builder->decodeReceiptDataToJsonObj(*rpt_bytes);
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

const char* bcos_sdk_calc_receipt_data_hash(int crypto_type, void* _receiptData)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(_receiptData, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        auto builder = std::make_shared<ReceiptBuilder>();
        auto transactionDataHash = builder->calculateReceiptDataHash(
            crypto_type == BCOS_C_SDK_ECDSA_TYPE ? CryptoType::Secp256K1 : CryptoType::SM2,
            *(bcostars::TransactionReceiptData*)_receiptData);
        return strdup(bcos::toHexStringWithPrefix(transactionDataHash).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        BCOS_LOG(WARNING) << LOG_BADGE("bcos_sdk_calc_transaction_data_hash")
                          << LOG_DESC("exception") << LOG_KV("crypto_type", crypto_type)
                          << LOG_KV("_receiptData", _receiptData) << LOG_KV("error", errorMsg);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
    }
    return NULL;
}