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
 * @file testTransaction.cpp
 * @author: lucasli
 * @date 2023-09-12
 */

#include "bcos-c-sdk/bcos_sdk_c.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_abi.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_tx.h"
#include <bcos-cpp-sdk/utilities/crypto/Common.h>
#include <bcos-tars-protocol/tars/Transaction.h>

#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>

using namespace bcos;

struct TestTxFixture
{
};

BOOST_FIXTURE_TEST_SUITE(TransactionTest, TestTxFixture)
const char* g_hw_abi =
    "[{\"inputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"constructor\"},{\"inputs\":[],"
    "\"name\":\"get\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}]"
    ",\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":"
    "\"string\",\"name\":\"n\",\"type\":\"string\"}],\"name\":\"set\",\"outputs\":[],"
    "\"stateMutability\":\"nonpayable\",\"type\":\"function\"}]";

BOOST_AUTO_TEST_CASE(testTxData)
{
    auto sm_crypto = 0;
    auto group_id = "group0";
    auto chain_id = "chain0";
    int64_t block_limit = 11;
    const char* extra_data = "ExtraData";
    void* key_pair = bcos_sdk_create_keypair(sm_crypto);

    const char* set_data =
        bcos_sdk_abi_encode_method(g_hw_abi, "set", "[\"Hello FISCO-BCOS 3.0!!!\"]", sm_crypto);

    void* transaction_data = bcos_sdk_create_transaction_data(
        group_id, chain_id, nullptr, set_data, g_hw_abi, block_limit);

    auto success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);

    const char* transaction_data_hash =
        bcos_sdk_calc_transaction_data_hash(sm_crypto, transaction_data);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);

    const char* signed_hash = bcos_sdk_sign_transaction_data_hash(key_pair, transaction_data_hash);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);

    const char* signed_tx = bcos_sdk_create_signed_transaction_with_signed_data_ver_extra_data(
        transaction_data, signed_hash, transaction_data_hash, 0, extra_data);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);

    void* txRaw = bcos_sdk_decode_transaction(signed_tx);
    success = bcos_sdk_is_last_opr_success();
    BOOST_TEST(success == true);
    auto tx = (bcostars::Transaction*)txRaw;
    BOOST_TEST(tx->extraData == extra_data);
    BOOST_TEST(tx->data.version == 0);
    BOOST_TEST(tx->data.blockLimit == block_limit);
    BOOST_TEST(tx->data.groupID == group_id);
    BOOST_TEST(tx->data.chainID == chain_id);
    BOOST_TEST(tx->data.abi == g_hw_abi);


    bcos_sdk_destroy_transaction_data(transaction_data);
    bcos_sdk_destroy_transaction(txRaw);
    bcos_sdk_c_free((void*)transaction_data_hash);
    bcos_sdk_c_free((void*)signed_hash);
    bcos_sdk_c_free((void*)signed_tx);
}

BOOST_AUTO_TEST_SUITE_END()