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
 * @file testTransactionStruct.cpp
 * @author: xingqiangbai
 * @date 2023-07-06
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

struct TestTxStructFixture
{
};

BOOST_FIXTURE_TEST_SUITE(TransactionStructTest, TestTxStructFixture)

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

struct bcos_sdk_c_transaction_data* createTxDataStructWithHexInput()
{
    auto group_id = "";
    auto chain_id = "";
    auto to = "";
    auto abi = "";
    int64_t block_limit = 1;
    auto txDataWithHex = bcos_sdk_create_transaction_data_struct_with_hex_input(
        group_id, chain_id, to, input, abi, block_limit);

    return txDataWithHex;
}

struct bcos_sdk_c_transaction_data* createTxDataStructWithByteInput()
{
    auto group_id = "";
    auto chain_id = "";
    auto to = "";
    auto abi = "";
    int64_t block_limit = 1;
    auto bytesInput = fromHexString(input);

    auto txDataWithBytes = bcos_sdk_create_transaction_data_struct_with_bytes(
        group_id, chain_id, to, bytesInput->data(), bytesInput->size(), abi, block_limit);

    return txDataWithBytes;
}

void compareTxStruct(struct bcos_sdk_c_transaction* txStruct,
    struct bcos_sdk_c_transaction_data* txDataStruct, const char* signature, const char* extraData,
    int64_t attribute)
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

BOOST_AUTO_TEST_CASE(testCreateTxDataStructWithHexInput)
{
    // create transaction_data_struct failed (block_limit = 0)
    auto group_id = "";
    auto chain_id = "";
    auto to = "";
    auto abi = "";
    int64_t block_limit = 0;
    auto txData = bcos_sdk_create_transaction_data_struct_with_hex_input(
        group_id, chain_id, to, input, abi, block_limit);
    auto success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("block limit must > 0"));

    // create transaction_data_struct failed (input = "")
    block_limit = 1;
    txData = bcos_sdk_create_transaction_data_struct_with_hex_input(
        group_id, chain_id, to, "", abi, block_limit);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("input can not be empty string"));

    // create transaction_data_struct success
    txData = bcos_sdk_create_transaction_data_struct_with_hex_input(
        group_id, chain_id, to, input, abi, block_limit);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txData != nullptr);
    BOOST_TEST(txData->group_id == group_id);
    BOOST_TEST(txData->chain_id == chain_id);
    BOOST_TEST(txData->to == to);
    BOOST_TEST(txData->abi == abi);
    BOOST_TEST(txData->block_limit == block_limit);
    auto bytesInput = fromHexString(input);
    std::string fromInputHexString(bytesInput->begin(), bytesInput->end());
    std::string txDataInputString(
        reinterpret_cast<char*>(txData->input->buffer), txData->input->length);
    BOOST_TEST(txDataInputString == fromInputHexString);

    bcos_sdk_destroy_transaction_data_struct(txData);
}

BOOST_AUTO_TEST_CASE(testCreateTxDataStructWithByteInput)
{
    auto bytesInput = fromHexString(input);

    // create transaction_data_struct failed (block_limit = 0)
    auto group_id = "";
    auto chain_id = "";
    auto to = "";
    auto abi = "";
    uint32_t bytes_input_length = 10;
    int64_t block_limit = 0;
    auto txData = bcos_sdk_create_transaction_data_struct_with_bytes(
        group_id, chain_id, to, bytesInput->data(), bytes_input_length, abi, block_limit);
    auto success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("block limit must > 0"));

    // create transaction_data_struct failed (bytes_input_length = 0)
    bytes_input_length = 0;
    block_limit = 1;
    txData = bcos_sdk_create_transaction_data_struct_with_bytes(
        group_id, chain_id, to, bytesInput->data(), bytes_input_length, abi, block_limit);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("bytes input length must > 0"));

    // create transaction_data_struct failed (group_id = NULL)
    group_id = nullptr;
    txData = bcos_sdk_create_transaction_data_struct_with_bytes(
        group_id, chain_id, to, bytesInput->data(), bytesInput->size(), abi, block_limit);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create transaction_data_struct failed (group_id has Chinese characters)
    group_id = "区块链群组1";
    txData = bcos_sdk_create_transaction_data_struct_with_bytes(
        group_id, chain_id, to, bytesInput->data(), bytesInput->size(), abi, block_limit);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txData != nullptr);
    BOOST_TEST(txData->group_id == group_id);
    BOOST_TEST(txData->chain_id == chain_id);
    BOOST_TEST(txData->to == to);
    BOOST_TEST(txData->abi == abi);
    BOOST_TEST(txData->block_limit == block_limit);
    std::string fromInputHexString1(bytesInput->begin(), bytesInput->end());
    std::string txDataInputString1(
        reinterpret_cast<char*>(txData->input->buffer), txData->input->length);
    BOOST_TEST(txDataInputString1 == fromInputHexString1);

    // create transaction_data_struct failed (chain_id = NULL)
    chain_id = nullptr;
    txData = bcos_sdk_create_transaction_data_struct_with_bytes(
        group_id, chain_id, to, bytesInput->data(), bytesInput->size(), abi, block_limit);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create transaction_data_struct failed (chain_id has Chinese characters)
    chain_id = "区块链1";
    txData = bcos_sdk_create_transaction_data_struct_with_bytes(
        group_id, chain_id, to, bytesInput->data(), bytesInput->size(), abi, block_limit);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txData != nullptr);
    BOOST_TEST(txData->group_id == group_id);
    BOOST_TEST(txData->chain_id == chain_id);
    BOOST_TEST(txData->to == to);
    BOOST_TEST(txData->abi == abi);
    BOOST_TEST(txData->block_limit == block_limit);
    std::string fromInputHexString2(bytesInput->begin(), bytesInput->end());
    std::string txDataInputString2(
        reinterpret_cast<char*>(txData->input->buffer), txData->input->length);
    BOOST_TEST(txDataInputString2 == fromInputHexString2);

    // create transaction_data_struct failed (abi has Chinese characters)
    chain_id = "";
    txData = bcos_sdk_create_transaction_data_struct_with_bytes(group_id, chain_id, to,
        bytesInput->data(), bytesInput->size(), abi_with_chinese, block_limit);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txData != nullptr);
    BOOST_TEST(txData->group_id == group_id);
    BOOST_TEST(txData->chain_id == chain_id);
    BOOST_TEST(txData->to == to);
    BOOST_TEST(txData->abi == abi_with_chinese);
    BOOST_TEST(txData->block_limit == block_limit);
    std::string fromInputHexString3(bytesInput->begin(), bytesInput->end());
    std::string txDataInputString3(
        reinterpret_cast<char*>(txData->input->buffer), txData->input->length);
    BOOST_TEST(txDataInputString3 == fromInputHexString3);

    // create transaction_data_struct success
    group_id = "";
    txData = bcos_sdk_create_transaction_data_struct_with_bytes(
        group_id, chain_id, to, bytesInput->data(), bytesInput->size(), abi, block_limit);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txData != nullptr);
    BOOST_TEST(txData->group_id == group_id);
    BOOST_TEST(txData->chain_id == chain_id);
    BOOST_TEST(txData->to == to);
    BOOST_TEST(txData->abi == abi);
    BOOST_TEST(txData->block_limit == block_limit);
    std::string fromInputHexString(bytesInput->begin(), bytesInput->end());
    std::string txDataInputString(
        reinterpret_cast<char*>(txData->input->buffer), txData->input->length);
    BOOST_TEST(txDataInputString == fromInputHexString);

    bcos_sdk_destroy_transaction_data_struct(txData);
}

BOOST_AUTO_TEST_CASE(testEncodeDecodeTxDataStruct)
{
    auto txDataWithHex = createTxDataStructWithHexInput();
    auto txDataWithBytes = createTxDataStructWithByteInput();

    // 1. test tx data encode decode hex
    // encode to hex failed (transaction_data == NULL)
    auto encodedTxDataHex = bcos_sdk_encode_transaction_data_struct_to_hex(nullptr);
    auto hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(encodedTxDataHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // encode to hex success
    encodedTxDataHex = bcos_sdk_encode_transaction_data_struct_to_hex(txDataWithHex);
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == true);
    BOOST_TEST(encodedTxDataHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    encodedTxDataHex = bcos_sdk_encode_transaction_data_struct_to_hex(txDataWithBytes);
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == true);
    BOOST_TEST(encodedTxDataHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // decode hex failed (transaction_data_hex_str == NULL)
    auto decodedTxDataHex = bcos_sdk_decode_transaction_data_struct_from_hex(nullptr);
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxDataHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // decode hex failed (transaction_data_hex_str == "")
    decodedTxDataHex = bcos_sdk_decode_transaction_data_struct_from_hex("");
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxDataHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_data_hex_str can not be empty string"));

    // decode hex failed (the length of transaction_data_hex_str is not even number)
    decodedTxDataHex = bcos_sdk_decode_transaction_data_struct_from_hex("0xabc");
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxDataHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("the length of transaction_data_hex_str must be an even number"));

    // decode hex success
    decodedTxDataHex = bcos_sdk_decode_transaction_data_struct_from_hex(encodedTxDataHex);
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
    std::string txDataInputString(
        reinterpret_cast<char*>(txDataWithBytes->input->buffer), txDataWithBytes->input->length);
    std::string decodedHexTxDataInputString(
        reinterpret_cast<char*>(decodedTxDataHex->input->buffer), decodedTxDataHex->input->length);
    BOOST_TEST(txDataInputString == decodedHexTxDataInputString);

    // 2. test tx data encode decode json
    // encode to json failed (transaction_data == NULL)
    auto encodedTxDataJson = bcos_sdk_encode_transaction_data_struct_to_json(nullptr);
    auto jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(encodedTxDataJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // encode to json success
    encodedTxDataJson = bcos_sdk_encode_transaction_data_struct_to_json(txDataWithHex);
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == true);
    BOOST_TEST(encodedTxDataJson != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    encodedTxDataJson = bcos_sdk_encode_transaction_data_struct_to_json(txDataWithBytes);
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == true);
    BOOST_TEST(encodedTxDataJson != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // decode json failed (transaction_data_json_str == NULL)
    auto decodedTxDataJson = bcos_sdk_decode_transaction_data_struct_from_json(nullptr);
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(decodedTxDataJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // decode json failed (transaction_data_json_str == "")
    decodedTxDataJson = bcos_sdk_decode_transaction_data_struct_from_json("");
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(decodedTxDataJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_data_json_str can not be empty string"));

    // decode json success
    decodedTxDataJson = bcos_sdk_decode_transaction_data_struct_from_json(encodedTxDataJson);
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
    std::string decodedJsonTxDataInputJson(
        reinterpret_cast<char*>(decodedTxDataJson->input->buffer),
        decodedTxDataJson->input->length);
    BOOST_TEST(txDataInputString == decodedJsonTxDataInputJson);

    bcos_sdk_destroy_transaction_data_struct(txDataWithHex);
    bcos_sdk_destroy_transaction_data_struct(txDataWithBytes);
    bcos_sdk_destroy_transaction_data_struct(decodedTxDataHex);
    bcos_sdk_destroy_transaction_data_struct(decodedTxDataJson);
}

BOOST_AUTO_TEST_CASE(testCalculateTxDataHash)
{
    auto txDataWithHex = createTxDataStructWithHexInput();
    auto txDataWithByte = createTxDataStructWithByteInput();
    auto encodedtxDataWithHex = bcos_sdk_encode_transaction_data_struct_to_hex(txDataWithHex);
    auto encodedtxDataWithByte = bcos_sdk_encode_transaction_data_struct_to_hex(txDataWithByte);

    // 1. test cal tx data hash with struct
    // calculate tx data hash failed (cryptoType not in 0,1)
    auto cryptoType = 2;
    auto txDataHash = bcos_sdk_calc_transaction_data_struct_hash(cryptoType, txDataWithHex);
    auto success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txDataHash == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto "
                           "type) or BCOS_C_SDK_SM_TYPE(sm crypto type)"));

    // calculate tx data hash failed (cryptoType is negative number)
    cryptoType = -11;
    txDataHash = bcos_sdk_calc_transaction_data_struct_hash(cryptoType, txDataWithHex);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txDataHash == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto "
                           "type) or BCOS_C_SDK_SM_TYPE(sm crypto type)"));

    // calculate tx data hash failed (transaction_data == NULL)
    cryptoType = 1;
    txDataHash = bcos_sdk_calc_transaction_data_struct_hash(cryptoType, nullptr);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txDataHash == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // calculate tx data hash success (txDataWithHex)
    cryptoType = 1;
    txDataHash = bcos_sdk_calc_transaction_data_struct_hash(cryptoType, txDataWithHex);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txDataHash != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // calculate tx data hash success (txDataWithByte)
    txDataHash = bcos_sdk_calc_transaction_data_struct_hash(cryptoType, txDataWithByte);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txDataHash != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // 2. test cal tx data hash with tx_data_hex
    // calculate tx data hash with hex failed (cryptoType not in 0,1)
    cryptoType = 2;
    auto txDataHashWithHex =
        bcos_sdk_calc_transaction_data_struct_hash_with_hex(cryptoType, encodedtxDataWithHex);
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
        bcos_sdk_calc_transaction_data_struct_hash_with_hex(cryptoType, encodedtxDataWithHex);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txDataHashWithHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto "
                           "type) or BCOS_C_SDK_SM_TYPE(sm crypto type)"));

    // calculate tx data hash with hex failed (transaction_data_hex == NULL)
    cryptoType = 1;
    txDataHashWithHex = bcos_sdk_calc_transaction_data_struct_hash_with_hex(cryptoType, nullptr);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txDataHashWithHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // calculate tx data hash with hex failed (transaction_data_hex == "")
    txDataHashWithHex = bcos_sdk_calc_transaction_data_struct_hash_with_hex(cryptoType, "");
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txDataHashWithHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_data_hex can not be empty string"));

    // calculate tx data hash with hex success (encodedtxDataWithHex)
    cryptoType = 1;
    txDataHashWithHex =
        bcos_sdk_calc_transaction_data_struct_hash_with_hex(cryptoType, encodedtxDataWithHex);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txDataHashWithHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // calculate tx data hash with hex success (encodedtxDataWithByte)
    txDataHashWithHex =
        bcos_sdk_calc_transaction_data_struct_hash_with_hex(cryptoType, encodedtxDataWithByte);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(txDataHashWithHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // 3. compare hash
    BOOST_TEST(txDataHash == txDataHashWithHex);

    bcos_sdk_destroy_transaction_data_struct(txDataWithHex);
    bcos_sdk_destroy_transaction_data_struct(txDataWithByte);
}

BOOST_AUTO_TEST_CASE(testCreateTxStruct)
{
    auto txDataStruct = createTxDataStructWithHexInput();
    auto transactionDataHash = bcos_sdk_calc_transaction_data_struct_hash(1, txDataStruct);
    void* key_pair = bcos_sdk_create_keypair(1);
    auto signature = bcos_sdk_sign_transaction_data_hash(key_pair, transactionDataHash);
    auto extraData = "";
    int64_t attribute = 0;

    // create transaction_struct failed (transaction_data_struct = NULL)
    auto txStruct = bcos_sdk_create_transaction_struct(
        nullptr, signature, transactionDataHash, attribute, extraData);
    auto success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create transaction_struct failed (transaction_data_hash = NULL)
    txStruct =
        bcos_sdk_create_transaction_struct(txDataStruct, signature, nullptr, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create transaction_struct failed (signature = NULL)
    txStruct = bcos_sdk_create_transaction_struct(
        txDataStruct, nullptr, transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create transaction_struct failed (transaction_data_hash = "")
    txStruct =
        bcos_sdk_create_transaction_struct(txDataStruct, signature, "", attribute, extraData);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_data_hash can not be empty string"));

    // create transaction_struct failed (signature = "")
    txStruct = bcos_sdk_create_transaction_struct(
        txDataStruct, "", transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(txStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("signature can not be empty string"));

    // create transaction_struct success (extraData = NULL)
    txStruct = bcos_sdk_create_transaction_struct(
        txDataStruct, signature, transactionDataHash, attribute, nullptr);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);
    compareTxStruct(txStruct, txDataStruct, signature, "", attribute);

    // create transaction_struct success
    txStruct = bcos_sdk_create_transaction_struct(
        txDataStruct, signature, transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);
    compareTxStruct(txStruct, txDataStruct, signature, extraData, attribute);

    bcos_sdk_destroy_transaction_data_struct(txDataStruct);
    bcos_sdk_destroy_transaction_struct(txStruct);
}

BOOST_AUTO_TEST_CASE(testCreateEncodedTx)
{
    auto txDataStruct = createTxDataStructWithHexInput();
    auto transactionDataHash = bcos_sdk_calc_transaction_data_struct_hash(1, txDataStruct);
    void* key_pair = bcos_sdk_create_keypair(1);
    auto signature = bcos_sdk_sign_transaction_data_hash(key_pair, transactionDataHash);
    auto extraData = "";
    int64_t attribute = 0;
    auto txStruct = bcos_sdk_create_transaction_struct(
        txDataStruct, signature, transactionDataHash, attribute, extraData);

    // 1. test bcos_sdk_create_encoded_transaction
    // create encoded tx failed (transaction_data_struct = NULL)
    auto encodedTx = bcos_sdk_create_encoded_transaction(
        nullptr, signature, transactionDataHash, attribute, extraData);
    auto success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(encodedTx == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create encoded tx failed (transaction_data_hash = NULL)
    encodedTx =
        bcos_sdk_create_encoded_transaction(txDataStruct, signature, nullptr, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(encodedTx == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create encoded tx failed (signature = NULL)
    encodedTx = bcos_sdk_create_encoded_transaction(
        txDataStruct, nullptr, transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(encodedTx == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create encoded tx failed (transaction_data_hash = "")
    encodedTx =
        bcos_sdk_create_encoded_transaction(txDataStruct, signature, "", attribute, extraData);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(encodedTx == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_data_hash can not be empty string"));

    // create encoded tx failed (signature = "")
    encodedTx = bcos_sdk_create_encoded_transaction(
        txDataStruct, "", transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(encodedTx == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("signature can not be empty string"));

    // create encoded tx success
    encodedTx = bcos_sdk_create_encoded_transaction(
        txDataStruct, signature, transactionDataHash, attribute, extraData);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(encodedTx != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // 2. test bcos_sdk_encode_transaction_struct_to_hex_
    // create encoded tx with tx_struct failed (transaction_struct == "")
    auto encodedTxWithTxStruct = bcos_sdk_encode_transaction_struct_to_hex(nullptr);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == false);
    BOOST_TEST(encodedTxWithTxStruct == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // create encoded tx with tx_struct success
    encodedTxWithTxStruct = bcos_sdk_encode_transaction_struct_to_hex(txStruct);
    success = bcos_sdk_is_last_opr_success();

    BOOST_TEST(success == true);
    BOOST_TEST(encodedTxWithTxStruct != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // 3. compare encodedTx encodedTxWithTxStruct
    BOOST_TEST(encodedTx == encodedTxWithTxStruct);

    bcos_sdk_destroy_transaction_data_struct(txDataStruct);
    bcos_sdk_destroy_transaction_struct(txStruct);
}

BOOST_AUTO_TEST_CASE(testEncodeDecodeTxStruct)
{
    auto txDataStruct = createTxDataStructWithHexInput();
    auto transactionDataHash = bcos_sdk_calc_transaction_data_struct_hash(1, txDataStruct);
    void* key_pair = bcos_sdk_create_keypair(1);
    auto signature = bcos_sdk_sign_transaction_data_hash(key_pair, transactionDataHash);
    auto extraData = "";
    int64_t attribute = 0;

    auto txStruct = bcos_sdk_create_transaction_struct(
        txDataStruct, signature, transactionDataHash, attribute, extraData);

    // 1. test tx encode decode hex
    // encode tx to hex failed (transaction == NULL)
    auto encodedTxHex = bcos_sdk_encode_transaction_struct_to_hex(nullptr);
    auto hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(encodedTxHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // encode tx to hex success
    encodedTxHex = bcos_sdk_encode_transaction_struct_to_hex(txStruct);
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == true);
    BOOST_TEST(encodedTxHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // decode hex failed (transaction_hex_str == NULL)
    auto decodedTxHex = bcos_sdk_decode_transaction_struct_from_hex(nullptr);
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // decode hex failed (transaction_hex_str == "")
    decodedTxHex = bcos_sdk_decode_transaction_struct_from_hex("");
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_hex_str can not be empty string"));

    // decode hex failed (the length of transaction_hex_str is not even number)
    decodedTxHex = bcos_sdk_decode_transaction_struct_from_hex("0xabc");
    hexSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(hexSuccess == false);
    BOOST_TEST(decodedTxHex == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("the length of transaction_hex_str must be an even number"));

    // decode hex success
    decodedTxHex = bcos_sdk_decode_transaction_struct_from_hex(encodedTxHex);
    hexSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(hexSuccess == true);
    BOOST_TEST(decodedTxHex != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    compareTxStruct(decodedTxHex, txDataStruct, signature, extraData, attribute);

    // 2. test tx encode decode json
    // encode tx to json failed (transaction == NULL)
    auto encodedTxJson = bcos_sdk_encode_transaction_struct_to_json(nullptr);
    auto jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(encodedTxJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // encode tx to json success
    encodedTxJson = bcos_sdk_encode_transaction_struct_to_json(txStruct);
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == true);
    BOOST_TEST(encodedTxJson != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    // decode json failed (transaction_json_str == NULL)
    auto decodedTxJson = bcos_sdk_decode_transaction_struct_from_json(nullptr);
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(decodedTxJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);

    // decode json failed (transaction_json_str == "")
    decodedTxJson = bcos_sdk_decode_transaction_struct_from_json("");
    jsonSuccess = bcos_sdk_is_last_opr_success();

    BOOST_TEST(jsonSuccess == false);
    BOOST_TEST(decodedTxJson == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() ==
               std::string("transaction_json_str can not be empty string"));

    // decode json success
    decodedTxJson = bcos_sdk_decode_transaction_struct_from_json(encodedTxJson);
    jsonSuccess = bcos_sdk_is_last_opr_success();
    BOOST_TEST(jsonSuccess == true);
    BOOST_TEST(decodedTxJson != nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == 0);

    compareTxStruct(decodedTxJson, txDataStruct, signature, extraData, attribute);

    bcos_sdk_destroy_transaction_data_struct(txDataStruct);
    bcos_sdk_destroy_transaction_struct(txStruct);
    bcos_sdk_destroy_transaction_struct(decodedTxHex);
    bcos_sdk_destroy_transaction_struct(decodedTxJson);
}

BOOST_AUTO_TEST_SUITE_END()