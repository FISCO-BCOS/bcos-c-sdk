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
 * @file testTransactionStructV2.cpp
 * @author: lucasli
 * @date 2023-12-13
 */

#include "bcos-c-sdk/bcos_sdk_c.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_rpc.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_abi.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_tx.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_tx_struct.h"
#include <bcos-cpp-sdk/utilities/crypto/Common.h>
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>

using namespace bcos;

struct TestTxStructV2Fixture
{
};

BOOST_FIXTURE_TEST_SUITE(TransactionStructV2Test, TestTxStructV2Fixture)

const auto input =
    "0x3590b49f00000000000000000000000000000000000000000000000000000000000000200000000000000000"
    "00000000000000000000000000000000000000000000001748656c6c6f20464953434f2d42434f5320332e3021"
    "2121000000000000000000";

const char* abi_with_chinese =
    "[{\"inputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"constructor\"},{\"inputs\":[],"
    "\"name\":\"get\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"李伟光\",\"type\":"
    "\"string\"}]"
    ",\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":"
    "\"string\",\"name\":\"n\",\"type\":\"string\"}],\"name\":\"set\",\"outputs\":[],"
    "\"stateMutability\":\"nonpayable\",\"type\":\"function\"}]";

struct bcos_sdk_c_transaction_data_v2* createTxDataStructWithHexInputV2()
{
    auto group_id = "";
    auto chain_id = "";
    auto to = "";
    auto abi = "";
    int64_t block_limit = 1;
    auto value = "1";
    auto gas_price = "0";
    int64_t gas_limit = 0;
    auto max_fee_per_gas = "1";
    auto max_priority_fee_per_gas = "1";
    auto txDataWithHex = bcos_sdk_create_transaction_data_struct_with_hex_input_v2(group_id,
        chain_id, to, input, abi, block_limit, value, gas_price, gas_limit, max_fee_per_gas,
        max_priority_fee_per_gas);

    return txDataWithHex;
}

struct bcos_sdk_c_transaction_data_v2* createTxDataStructWithByteInputV2()
{
    auto group_id = "";
    auto chain_id = "";
    auto to = "";
    auto abi = "";
    int64_t block_limit = 1;
    auto value = "1";
    auto gas_price = "0";
    int64_t gas_limit = 0;
    auto max_fee_per_gas = "1";
    auto max_priority_fee_per_gas = "1";
    auto bytesInput = fromHexString(input);

    auto txDataWithBytes = bcos_sdk_create_transaction_data_struct_with_bytes_v2(group_id, chain_id,
        to, bytesInput->data(), bytesInput->size(), abi, block_limit, value, gas_price, gas_limit,
        max_fee_per_gas, max_priority_fee_per_gas);

    return txDataWithBytes;
}

void compareTxStruct(struct bcos_sdk_c_transaction_v2* txStruct,
    struct bcos_sdk_c_transaction_data_v2* txDataStruct, const char* signature,
    const char* extraData, int64_t attribute)
{
    // transaction_data
    BOOST_TEST(txStruct != nullptr);
    BOOST_TEST(txStruct->transaction_data->version == txDataStruct->version);
    BOOST_TEST(txStruct->transaction_data->block_limit == txDataStruct->block_limit);
    BOOST_TEST(txStruct->transaction_data->chain_id == txDataStruct->chain_id);
    BOOST_TEST(txStruct->transaction_data->version == txDataStruct->version);
    BOOST_TEST(txStruct->transaction_data->group_id == txDataStruct->group_id);
    BOOST_TEST(txStruct->transaction_data->nonce == txDataStruct->nonce);
    BOOST_TEST(txStruct->transaction_data->to == txDataStruct->to);
    BOOST_TEST(txStruct->transaction_data->abi == txDataStruct->abi);
    BOOST_TEST(txStruct->transaction_data->value == txDataStruct->value);
    BOOST_TEST(txStruct->transaction_data->gas_price == txDataStruct->gas_price);
    BOOST_TEST(txStruct->transaction_data->gas_limit == txDataStruct->gas_limit);
    BOOST_TEST(txStruct->transaction_data->max_fee_per_gas == txDataStruct->max_fee_per_gas);
    BOOST_TEST(txStruct->transaction_data->max_priority_fee_per_gas ==
               txDataStruct->max_priority_fee_per_gas);
    std::string txDataInputString(
        reinterpret_cast<char*>(txDataStruct->input->buffer), txDataStruct->input->length);
    std::string txInputString(reinterpret_cast<char*>(txStruct->transaction_data->input->buffer),
        txStruct->transaction_data->input->length);
    BOOST_TEST(txDataInputString == txInputString);

    // signature
    std::string txStructSignatureString(
        reinterpret_cast<char*>(txStruct->signature->buffer), txStruct->signature->length);
    auto bytesSignature = fromHexString(signature);
    std::string fromSignatureHexString(bytesSignature->begin(), bytesSignature->end());
    BOOST_TEST(fromSignatureHexString == txStructSignatureString);

    // attribute
    BOOST_TEST(txStruct->attribute == attribute);

    // extra_data
    BOOST_TEST(txStruct->extra_data == extraData);
}

BOOST_AUTO_TEST_CASE(testCreateTxDataStructWithHexInputV2)
{
    auto group_id = "";
    auto chain_id = "";
    auto to = "";
    auto abi = "";
    int64_t block_limit = 0;
    auto value = "1";
    auto gas_price = "0";
    int64_t gas_limit = 0;
    auto max_fee_per_gas = "1";
    auto max_priority_fee_per_gas = "1";

    // create transaction_data_struct_v2 failed (block_limit = 0)
    auto txData = bcos_sdk_create_transaction_data_struct_with_hex_input_v2(group_id, chain_id, to,
        input, abi, block_limit, value, gas_price, gas_limit, max_fee_per_gas,
        max_priority_fee_per_gas);
    auto success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("block limit must > 0"));

    // create transaction_data_struct_v2 failed (input = "")
    block_limit = 1;
    txData = bcos_sdk_create_transaction_data_struct_with_hex_input_v2(group_id, chain_id, to, "",
        abi, block_limit, value, gas_price, gas_limit, max_fee_per_gas, max_priority_fee_per_gas);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("input can not be empty string"));

    // create transaction_data_struct_v2 failed (gas_limit <0)
    txData = bcos_sdk_create_transaction_data_struct_with_hex_input_v2(group_id, chain_id, to,
        input, abi, block_limit, value, gas_price, -1, max_fee_per_gas, max_priority_fee_per_gas);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("gas limit must >= 0"));

    // create transaction_data_struct_v2 success
    txData = bcos_sdk_create_transaction_data_struct_with_hex_input_v2(group_id, chain_id, to,
        input, abi, block_limit, value, gas_price, gas_limit, max_fee_per_gas,
        max_priority_fee_per_gas);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);
    BOOST_TEST(txData != nullptr);
    BOOST_TEST(txData->group_id == group_id);
    BOOST_TEST(txData->chain_id == chain_id);
    BOOST_TEST(txData->to == to);
    BOOST_TEST(txData->abi == abi);
    BOOST_TEST(txData->block_limit == block_limit);
    BOOST_TEST(txData->value == value);
    BOOST_TEST(txData->gas_price == gas_price);
    BOOST_TEST(txData->gas_limit == gas_limit);
    BOOST_TEST(txData->max_fee_per_gas == max_fee_per_gas);
    BOOST_TEST(txData->max_priority_fee_per_gas == max_priority_fee_per_gas);
    auto bytesInput = fromHexString(input);
    std::string fromInputHexString(bytesInput->begin(), bytesInput->end());
    std::string txDataInputString(
        reinterpret_cast<char*>(txData->input->buffer), txData->input->length);
    BOOST_TEST(txDataInputString == fromInputHexString);

    bcos_sdk_destroy_transaction_data_struct_v2(txData);
}

BOOST_AUTO_TEST_CASE(testCreateTxDataStructWithByteInputV2)
{
    auto bytesInput = fromHexString(input);
    auto group_id = "";
    auto chain_id = "";
    auto to = "";
    auto abi = "";
    uint32_t bytes_input_length = 10;
    int64_t block_limit = 0;
    auto value = "1";
    auto gas_price = "0";
    int64_t gas_limit = 0;
    auto max_fee_per_gas = "1";
    auto max_priority_fee_per_gas = "1";

    // create transaction_data_struct_v2 failed (block_limit = 0)
    auto txData = bcos_sdk_create_transaction_data_struct_with_bytes_v2(group_id, chain_id, to,
        bytesInput->data(), bytes_input_length, abi, block_limit, value, gas_price, gas_limit,
        max_fee_per_gas, max_priority_fee_per_gas);
    auto success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("block limit must > 0"));

    // create transaction_data_struct_v2 failed (bytes_input_length = 0)
    bytes_input_length = 0;
    block_limit = 1;
    txData = bcos_sdk_create_transaction_data_struct_with_bytes_v2(group_id, chain_id, to,
        bytesInput->data(), bytes_input_length, abi, block_limit, value, gas_price, gas_limit,
        max_fee_per_gas, max_priority_fee_per_gas);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("bytes input length must > 0"));

    // create transaction_data_struct_v2 failed (group_id = NULL)
    group_id = nullptr;
    txData = bcos_sdk_create_transaction_data_struct_with_bytes_v2(group_id, chain_id, to,
        bytesInput->data(), bytesInput->size(), abi, block_limit, value, gas_price, gas_limit,
        max_fee_per_gas, max_priority_fee_per_gas);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create transaction_data_struct_v2 success (group_id has Chinese characters)
    group_id = "区块链群组1";
    txData = bcos_sdk_create_transaction_data_struct_with_bytes_v2(group_id, chain_id, to,
        bytesInput->data(), bytesInput->size(), abi, block_limit, value, gas_price, gas_limit,
        max_fee_per_gas, max_priority_fee_per_gas);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txData != nullptr);
    BOOST_TEST(txData->group_id == group_id);
    BOOST_TEST(txData->chain_id == chain_id);
    BOOST_TEST(txData->to == to);
    BOOST_TEST(txData->abi == abi);
    BOOST_TEST(txData->block_limit == block_limit);
    BOOST_TEST(txData->value == value);
    BOOST_TEST(txData->gas_price == gas_price);
    BOOST_TEST(txData->gas_limit == gas_limit);
    BOOST_TEST(txData->max_fee_per_gas == max_fee_per_gas);
    BOOST_TEST(txData->max_priority_fee_per_gas == max_priority_fee_per_gas);
    std::string fromInputHexString1(bytesInput->begin(), bytesInput->end());
    std::string txDataInputString1(
        reinterpret_cast<char*>(txData->input->buffer), txData->input->length);
    BOOST_TEST(txDataInputString1 == fromInputHexString1);

    // create transaction_data_struct_v2 failed (chain_id = NULL)
    chain_id = nullptr;
    txData = bcos_sdk_create_transaction_data_struct_with_bytes_v2(group_id, chain_id, to,
        bytesInput->data(), bytesInput->size(), abi, block_limit, value, gas_price, gas_limit,
        max_fee_per_gas, max_priority_fee_per_gas);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create transaction_data_struct_v2 success (chain_id has Chinese characters)
    chain_id = "区块链1";
    txData = bcos_sdk_create_transaction_data_struct_with_bytes_v2(group_id, chain_id, to,
        bytesInput->data(), bytesInput->size(), abi, block_limit, value, gas_price, gas_limit,
        max_fee_per_gas, max_priority_fee_per_gas);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txData != nullptr);
    BOOST_TEST(txData->group_id == group_id);
    BOOST_TEST(txData->chain_id == chain_id);
    BOOST_TEST(txData->to == to);
    BOOST_TEST(txData->abi == abi);
    BOOST_TEST(txData->block_limit == block_limit);
    BOOST_TEST(txData->value == value);
    BOOST_TEST(txData->gas_price == gas_price);
    BOOST_TEST(txData->gas_limit == gas_limit);
    BOOST_TEST(txData->max_fee_per_gas == max_fee_per_gas);
    BOOST_TEST(txData->max_priority_fee_per_gas == max_priority_fee_per_gas);
    std::string fromInputHexString2(bytesInput->begin(), bytesInput->end());
    std::string txDataInputString2(
        reinterpret_cast<char*>(txData->input->buffer), txData->input->length);
    BOOST_TEST(txDataInputString2 == fromInputHexString2);

    // create transaction_data_struct_v2 success (abi has Chinese characters)
    chain_id = "";
    txData = bcos_sdk_create_transaction_data_struct_with_bytes_v2(group_id, chain_id, to,
        bytesInput->data(), bytesInput->size(), abi_with_chinese, block_limit, value, gas_price,
        gas_limit, max_fee_per_gas, max_priority_fee_per_gas);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txData != nullptr);
    BOOST_TEST(txData->group_id == group_id);
    BOOST_TEST(txData->chain_id == chain_id);
    BOOST_TEST(txData->to == to);
    BOOST_TEST(txData->abi == abi_with_chinese);
    BOOST_TEST(txData->block_limit == block_limit);
    BOOST_TEST(txData->value == value);
    BOOST_TEST(txData->gas_price == gas_price);
    BOOST_TEST(txData->gas_limit == gas_limit);
    BOOST_TEST(txData->max_fee_per_gas == max_fee_per_gas);
    BOOST_TEST(txData->max_priority_fee_per_gas == max_priority_fee_per_gas);
    std::string fromInputHexString3(bytesInput->begin(), bytesInput->end());
    std::string txDataInputString3(
        reinterpret_cast<char*>(txData->input->buffer), txData->input->length);
    BOOST_TEST(txDataInputString3 == fromInputHexString3);

    // create transaction_data_struct_v2 failed (gas_limit <0)
    txData = bcos_sdk_create_transaction_data_struct_with_bytes_v2(group_id, chain_id, to,
        bytesInput->data(), bytesInput->size(), abi, block_limit, value, gas_price, -1,
        max_fee_per_gas, max_priority_fee_per_gas);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("gas limit must >= 0"));

    // create transaction_data_struct_v2 success
    group_id = "";
    txData = bcos_sdk_create_transaction_data_struct_with_bytes_v2(group_id, chain_id, to,
        bytesInput->data(), bytesInput->size(), abi, block_limit, value, gas_price, gas_limit,
        max_fee_per_gas, max_priority_fee_per_gas);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txData != nullptr);
    BOOST_TEST(txData->group_id == group_id);
    BOOST_TEST(txData->chain_id == chain_id);
    BOOST_TEST(txData->to == to);
    BOOST_TEST(txData->abi == abi);
    BOOST_TEST(txData->block_limit == block_limit);
    BOOST_TEST(txData->value == value);
    BOOST_TEST(txData->gas_price == gas_price);
    BOOST_TEST(txData->gas_limit == gas_limit);
    BOOST_TEST(txData->max_fee_per_gas == max_fee_per_gas);
    BOOST_TEST(txData->max_priority_fee_per_gas == max_priority_fee_per_gas);
    std::string fromInputHexString(bytesInput->begin(), bytesInput->end());
    std::string txDataInputString(
        reinterpret_cast<char*>(txData->input->buffer), txData->input->length);
    BOOST_TEST(txDataInputString == fromInputHexString);

    bcos_sdk_destroy_transaction_data_struct_v2(txData);
}

BOOST_AUTO_TEST_CASE(testEncodeDecodeTxDataStructV2)
{
    auto txDataWithHex = createTxDataStructWithHexInputV2();
    auto txDataWithBytes = createTxDataStructWithByteInputV2();

    // 1. test tx data encode decode hex
    // encode to hex failed (transaction_data == NULL)
    auto encodedTxDataHex = bcos_sdk_encode_transaction_data_struct_to_hex_v2(nullptr);
    auto hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(encodedTxDataHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // encode to hex success
    encodedTxDataHex = bcos_sdk_encode_transaction_data_struct_to_hex_v2(txDataWithHex);
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == true);
    BOOST_TEST(encodedTxDataHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    encodedTxDataHex = bcos_sdk_encode_transaction_data_struct_to_hex_v2(txDataWithBytes);
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == true);
    BOOST_TEST(encodedTxDataHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // decode hex failed (transaction_data_hex_str == NULL)
    auto decodedTxDataHex = bcos_sdk_decode_transaction_data_struct_from_hex_v2(nullptr);
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxDataHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // decode hex failed (transaction_data_hex_str == "")
    decodedTxDataHex = bcos_sdk_decode_transaction_data_struct_from_hex_v2("");
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxDataHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_data_hex_str can not be empty string"));

    // decode hex failed (the length of transaction_data_hex_str is not even number)
    decodedTxDataHex = bcos_sdk_decode_transaction_data_struct_from_hex_v2("0xabc");
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxDataHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("the length of transaction_data_hex_str must be an even number"));

    // decode hex success
    decodedTxDataHex = bcos_sdk_decode_transaction_data_struct_from_hex_v2(encodedTxDataHex);
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == true);
    BOOST_TEST(decodedTxDataHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);
    BOOST_TEST(decodedTxDataHex->version == txDataWithBytes->version);
    BOOST_TEST(decodedTxDataHex->group_id == txDataWithBytes->group_id);
    BOOST_TEST(decodedTxDataHex->chain_id == txDataWithBytes->chain_id);
    BOOST_TEST(decodedTxDataHex->to == txDataWithBytes->to);
    BOOST_TEST(decodedTxDataHex->abi == txDataWithBytes->abi);
    BOOST_TEST(decodedTxDataHex->block_limit == txDataWithBytes->block_limit);
    BOOST_TEST(decodedTxDataHex->nonce == txDataWithBytes->nonce);
    BOOST_TEST(decodedTxDataHex->value == txDataWithBytes->value);
    BOOST_TEST(decodedTxDataHex->gas_price == txDataWithBytes->gas_price);
    BOOST_TEST(decodedTxDataHex->gas_limit == txDataWithBytes->gas_limit);
    BOOST_TEST(decodedTxDataHex->max_fee_per_gas == txDataWithBytes->max_fee_per_gas);
    BOOST_TEST(
        decodedTxDataHex->max_priority_fee_per_gas == txDataWithBytes->max_priority_fee_per_gas);
    std::string txDataInputString(
        reinterpret_cast<char*>(txDataWithBytes->input->buffer), txDataWithBytes->input->length);
    std::string decodedHexTxDataInputString(
        reinterpret_cast<char*>(decodedTxDataHex->input->buffer), decodedTxDataHex->input->length);
    BOOST_TEST(txDataInputString == decodedHexTxDataInputString);

    // 2. test tx data encode decode json
    // encode to json failed (transaction_data == NULL)
    auto encodedTxDataJson = bcos_sdk_encode_transaction_data_struct_to_json_v2(nullptr);
    auto jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(encodedTxDataJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // encode to json success
    encodedTxDataJson = bcos_sdk_encode_transaction_data_struct_to_json_v2(txDataWithHex);
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == true);
    BOOST_TEST(encodedTxDataJson != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    encodedTxDataJson = bcos_sdk_encode_transaction_data_struct_to_json_v2(txDataWithBytes);
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == true);
    BOOST_TEST(encodedTxDataJson != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // decode json failed (transaction_data_json_str == NULL)
    auto decodedTxDataJson = bcos_sdk_decode_transaction_data_struct_from_json_v2(nullptr);
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(decodedTxDataJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // decode json failed (transaction_data_json_str == "")
    decodedTxDataJson = bcos_sdk_decode_transaction_data_struct_from_json_v2("");
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(decodedTxDataJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_data_json_str can not be empty string"));

    // decode json success
    decodedTxDataJson = bcos_sdk_decode_transaction_data_struct_from_json_v2(encodedTxDataJson);
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == true);
    BOOST_TEST(decodedTxDataJson != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);
    BOOST_TEST(decodedTxDataJson->version == txDataWithBytes->version);
    BOOST_TEST(decodedTxDataJson->group_id == txDataWithBytes->group_id);
    BOOST_TEST(decodedTxDataJson->chain_id == txDataWithBytes->chain_id);
    BOOST_TEST(decodedTxDataJson->to == txDataWithBytes->to);
    BOOST_TEST(decodedTxDataJson->abi == txDataWithBytes->abi);
    BOOST_TEST(decodedTxDataJson->block_limit == txDataWithBytes->block_limit);
    BOOST_TEST(decodedTxDataJson->nonce == txDataWithBytes->nonce);
    BOOST_TEST(decodedTxDataHex->value == txDataWithBytes->value);
    BOOST_TEST(decodedTxDataHex->gas_price == txDataWithBytes->gas_price);
    BOOST_TEST(decodedTxDataHex->gas_limit == txDataWithBytes->gas_limit);
    BOOST_TEST(decodedTxDataHex->max_fee_per_gas == txDataWithBytes->max_fee_per_gas);
    BOOST_TEST(
        decodedTxDataHex->max_priority_fee_per_gas == txDataWithBytes->max_priority_fee_per_gas);
    std::string decodedJsonTxDataInputJson(
        reinterpret_cast<char*>(decodedTxDataJson->input->buffer),
        decodedTxDataJson->input->length);
    BOOST_TEST(txDataInputString == decodedJsonTxDataInputJson);

    bcos_sdk_destroy_transaction_data_struct_v2(txDataWithHex);
    bcos_sdk_destroy_transaction_data_struct_v2(txDataWithBytes);
    bcos_sdk_destroy_transaction_data_struct_v2(decodedTxDataHex);
    bcos_sdk_destroy_transaction_data_struct_v2(decodedTxDataJson);
}

BOOST_AUTO_TEST_CASE(testCalculateTxDataHashV2)
{
    auto txDataWithHex = createTxDataStructWithHexInputV2();
    auto txDataWithByte = createTxDataStructWithByteInputV2();
    auto encodedtxDataWithHex = bcos_sdk_encode_transaction_data_struct_to_hex_v2(txDataWithHex);
    auto encodedtxDataWithByte = bcos_sdk_encode_transaction_data_struct_to_hex_v2(txDataWithByte);

    // 1. test cal tx data hash with struct
    // calculate tx data hash failed (cryptoType not in 0,1)
    auto cryptoType = 2;
    auto txDataHash = bcos_sdk_calc_transaction_data_struct_hash_v2(cryptoType, txDataWithHex);
    auto success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txDataHash == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto "
                           "type) or BCOS_C_SDK_SM_TYPE(sm crypto type)"));

    // calculate tx data hash failed (cryptoType is negative number)
    cryptoType = -11;
    txDataHash = bcos_sdk_calc_transaction_data_struct_hash_v2(cryptoType, txDataWithHex);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txDataHash == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto "
                           "type) or BCOS_C_SDK_SM_TYPE(sm crypto type)"));

    // calculate tx data hash failed (transaction_data == NULL)
    cryptoType = 1;
    txDataHash = bcos_sdk_calc_transaction_data_struct_hash_v2(cryptoType, nullptr);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txDataHash == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // calculate tx data hash success (txDataWithHex)
    cryptoType = 1;
    txDataHash = bcos_sdk_calc_transaction_data_struct_hash_v2(cryptoType, txDataWithHex);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);
    BOOST_TEST(txDataHash != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // calculate tx data hash success (txDataWithByte)
    txDataHash = bcos_sdk_calc_transaction_data_struct_hash_v2(cryptoType, txDataWithByte);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);
    BOOST_TEST(txDataHash != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // 2. test cal tx data hash with tx_data_hex
    // calculate tx data hash with hex failed (cryptoType not in 0,1)
    cryptoType = 2;
    auto txDataHashWithHex =
        bcos_sdk_calc_transaction_data_struct_hash_with_hex_v2(cryptoType, encodedtxDataWithHex);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txDataHashWithHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto "
                           "type) or BCOS_C_SDK_SM_TYPE(sm crypto type)"));

    // calculate tx data hash failed (cryptoType is negative number)
    cryptoType = -11;
    txDataHashWithHex =
        bcos_sdk_calc_transaction_data_struct_hash_with_hex_v2(cryptoType, encodedtxDataWithHex);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txDataHashWithHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto "
                           "type) or BCOS_C_SDK_SM_TYPE(sm crypto type)"));

    // calculate tx data hash with hex failed (transaction_data_hex == NULL)
    cryptoType = 1;
    txDataHashWithHex = bcos_sdk_calc_transaction_data_struct_hash_with_hex_v2(cryptoType, nullptr);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txDataHashWithHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // calculate tx data hash with hex failed (transaction_data_hex == "")
    txDataHashWithHex = bcos_sdk_calc_transaction_data_struct_hash_with_hex_v2(cryptoType, "");
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txDataHashWithHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_data_hex can not be empty string"));

    // calculate tx data hash with hex success (encodedtxDataWithHex)
    cryptoType = 1;
    txDataHashWithHex =
        bcos_sdk_calc_transaction_data_struct_hash_with_hex_v2(cryptoType, encodedtxDataWithHex);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);
    BOOST_TEST(txDataHashWithHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // calculate tx data hash with hex success (encodedtxDataWithByte)
    txDataHashWithHex =
        bcos_sdk_calc_transaction_data_struct_hash_with_hex_v2(cryptoType, encodedtxDataWithByte);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);
    BOOST_TEST(txDataHashWithHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // 3. compare hash
    BOOST_TEST(txDataHash == txDataHashWithHex);

    bcos_sdk_destroy_transaction_data_struct_v2(txDataWithHex);
    bcos_sdk_destroy_transaction_data_struct_v2(txDataWithByte);
}

BOOST_AUTO_TEST_CASE(testCreateTxStructV2)
{
    auto txDataStruct = createTxDataStructWithHexInputV2();
    auto transactionDataHash = bcos_sdk_calc_transaction_data_struct_hash_v2(1, txDataStruct);
    void* key_pair = bcos_sdk_create_keypair(1);
    auto signature = bcos_sdk_sign_transaction_data_hash(key_pair, transactionDataHash);
    auto extraData = "";
    int64_t attribute = 0;

    // create transaction_struct failed (transaction_data_struct_v2 = NULL)
    auto txStruct = bcos_sdk_create_transaction_struct_v2(
        nullptr, signature, transactionDataHash, attribute, extraData);
    auto success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create transaction_struct failed (transaction_data_hash = NULL)
    txStruct = bcos_sdk_create_transaction_struct_v2(
        txDataStruct, signature, nullptr, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create transaction_struct failed (signature = NULL)
    txStruct = bcos_sdk_create_transaction_struct_v2(
        txDataStruct, nullptr, transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create transaction_struct failed (transaction_data_hash = "")
    txStruct =
        bcos_sdk_create_transaction_struct_v2(txDataStruct, signature, "", attribute, extraData);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_data_hash can not be empty string"));

    // create transaction_struct failed (signature = "")
    txStruct = bcos_sdk_create_transaction_struct_v2(
        txDataStruct, "", transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(txStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("signature can not be empty string"));

    // create transaction_struct success (extraData = NULL)
    txStruct = bcos_sdk_create_transaction_struct_v2(
        txDataStruct, signature, transactionDataHash, attribute, nullptr);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);
    compareTxStruct(txStruct, txDataStruct, signature, "", attribute);

    // create transaction_struct success
    txStruct = bcos_sdk_create_transaction_struct_v2(
        txDataStruct, signature, transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);

    compareTxStruct(txStruct, txDataStruct, signature, extraData, attribute);

    bcos_sdk_destroy_transaction_data_struct_v2(txDataStruct);
    bcos_sdk_destroy_transaction_struct_v2(txStruct);
}

BOOST_AUTO_TEST_CASE(testCreateEncodedTxV2)
{
    auto txDataStruct = createTxDataStructWithHexInputV2();
    auto transactionDataHash = bcos_sdk_calc_transaction_data_struct_hash_v2(1, txDataStruct);
    void* key_pair = bcos_sdk_create_keypair(1);
    auto signature = bcos_sdk_sign_transaction_data_hash(key_pair, transactionDataHash);
    auto extraData = "";
    int64_t attribute = 0;
    auto txStruct = bcos_sdk_create_transaction_struct_v2(
        txDataStruct, signature, transactionDataHash, attribute, extraData);

    // 1. test bcos_sdk_create_encoded_transaction_v2
    // create encoded tx failed (transaction_data_struct_v2 = NULL)
    auto encodedTx = bcos_sdk_create_encoded_transaction_v2(
        nullptr, signature, transactionDataHash, attribute, extraData);
    auto success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(encodedTx == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create encoded tx failed (transaction_data_hash = NULL)
    encodedTx = bcos_sdk_create_encoded_transaction_v2(
        txDataStruct, signature, nullptr, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(encodedTx == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create encoded tx failed (signature = NULL)
    encodedTx = bcos_sdk_create_encoded_transaction_v2(
        txDataStruct, nullptr, transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(encodedTx == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create encoded tx failed (transaction_data_hash = "")
    encodedTx =
        bcos_sdk_create_encoded_transaction_v2(txDataStruct, signature, "", attribute, extraData);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(encodedTx == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_data_hash can not be empty string"));

    // create encoded tx failed (signature = "")
    encodedTx = bcos_sdk_create_encoded_transaction_v2(
        txDataStruct, "", transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(encodedTx == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("signature can not be empty string"));

    // create encoded tx success
    encodedTx = bcos_sdk_create_encoded_transaction_v2(
        txDataStruct, signature, transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);
    BOOST_TEST(encodedTx != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // 2. test bcos_sdk_encode_transaction_struct_to_hex__v2
    // create encoded tx with tx_struct failed (transaction_struct == "")
    auto encodedTxWithTxStruct = bcos_sdk_encode_transaction_struct_to_hex_v2(nullptr);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == false);
    BOOST_TEST(encodedTxWithTxStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create encoded tx with tx_struct success
    encodedTxWithTxStruct = bcos_sdk_encode_transaction_struct_to_hex_v2(txStruct);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);
    BOOST_TEST(encodedTxWithTxStruct != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // 3. compare encodedTx encodedTxWithTxStruct
    BOOST_TEST(encodedTx == encodedTxWithTxStruct);

    bcos_sdk_destroy_transaction_data_struct_v2(txDataStruct);
    bcos_sdk_destroy_transaction_struct_v2(txStruct);
}

BOOST_AUTO_TEST_CASE(testEncodeDecodeTxStructV2)
{
    auto txDataStruct = createTxDataStructWithHexInputV2();
    auto transactionDataHash = bcos_sdk_calc_transaction_data_struct_hash_v2(1, txDataStruct);
    void* key_pair = bcos_sdk_create_keypair(1);
    auto signature = bcos_sdk_sign_transaction_data_hash(key_pair, transactionDataHash);
    auto extraData = "";
    int64_t attribute = 0;

    auto txStruct = bcos_sdk_create_transaction_struct_v2(
        txDataStruct, signature, transactionDataHash, attribute, extraData);

    // 1. test tx encode decode hex
    // encode tx to hex failed (transaction == NULL)
    auto encodedTxHex = bcos_sdk_encode_transaction_struct_to_hex_v2(nullptr);
    auto hexSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(encodedTxHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // encode tx to hex success
    encodedTxHex = bcos_sdk_encode_transaction_struct_to_hex_v2(txStruct);
    hexSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(hexSuccess == true);
    BOOST_TEST(encodedTxHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // decode hex failed (transaction_hex_str == NULL)
    auto decodedTxHex = bcos_sdk_decode_transaction_struct_from_hex_v2(nullptr);
    hexSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // decode hex failed (transaction_hex_str == "")
    decodedTxHex = bcos_sdk_decode_transaction_struct_from_hex_v2("");
    hexSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_hex_str can not be empty string"));

    // decode hex failed (the length of transaction_hex_str is not even number)
    decodedTxHex = bcos_sdk_decode_transaction_struct_from_hex_v2("0xabc");
    hexSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("the length of transaction_hex_str must be an even number"));

    // decode hex success
    decodedTxHex = bcos_sdk_decode_transaction_struct_from_hex_v2(encodedTxHex);
    hexSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(hexSuccess == true);
    BOOST_TEST(decodedTxHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    compareTxStruct(decodedTxHex, txDataStruct, signature, extraData, attribute);

    // 2. test tx encode decode json
    // encode tx to json failed (transaction == NULL)
    auto encodedTxJson = bcos_sdk_encode_transaction_struct_to_json_v2(nullptr);
    auto jsonSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(encodedTxJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // encode tx to json success
    encodedTxJson = bcos_sdk_encode_transaction_struct_to_json_v2(txStruct);
    jsonSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(jsonSuccess == true);
    BOOST_TEST(encodedTxJson != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // decode json failed (transaction_json_str == NULL)
    auto decodedTxJson = bcos_sdk_decode_transaction_struct_from_json_v2(nullptr);
    jsonSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(decodedTxJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // decode json failed (transaction_json_str == "")
    decodedTxJson = bcos_sdk_decode_transaction_struct_from_json_v2("");
    jsonSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(decodedTxJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_json_str can not be empty string"));

    // decode json success
    decodedTxJson = bcos_sdk_decode_transaction_struct_from_json_v2(encodedTxJson);
    jsonSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(jsonSuccess == true);
    BOOST_TEST(decodedTxJson != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    compareTxStruct(decodedTxJson, txDataStruct, signature, extraData, attribute);

    bcos_sdk_destroy_transaction_data_struct_v2(txDataStruct);
    bcos_sdk_destroy_transaction_struct_v2(txStruct);
    bcos_sdk_destroy_transaction_struct_v2(decodedTxHex);
    bcos_sdk_destroy_transaction_struct_v2(decodedTxJson);
}

BOOST_AUTO_TEST_SUITE_END()