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
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>

struct TestTxStructFixture
{
};

BOOST_FIXTURE_TEST_SUITE(TransactionStructTest, TestTxStructFixture)

BOOST_AUTO_TEST_CASE(test_create_transaction_data_struct_with_hex_input)
{
    auto group_id = "";
    auto chain_id = "";
    auto to = "";
    auto input = "";
    auto abi = "";
    int64_t block_limit = 0;
    auto txData = bcos_sdk_create_transaction_data_struct_with_hex_input(
        group_id, chain_id, to, input, abi, block_limit);
    auto success = bcos_sdk_is_last_opr_success();
    // if (!success)
    // {
    //     std::cerr << bcos_sdk_get_last_error_msg() << std::endl;
    // }
    BOOST_TEST(success == false);
    BOOST_TEST(txData == nullptr);
    BOOST_TEST(bcos_sdk_get_last_error() == -1);
    BOOST_TEST(bcos_sdk_get_last_error_msg() == std::string("block limit must >= 0"));

    block_limit = 1;
    txData = bcos_sdk_create_transaction_data_struct_with_hex_input(
        group_id, chain_id, to, input, abi, block_limit);
    success = bcos_sdk_is_last_opr_success();
    if (!success)
    {
        // BOOST_TEST_MESSAGE(bcos_sdk_get_last_error_msg());
        std::cerr << bcos_sdk_get_last_error_msg() << std::endl;
    }
    BOOST_TEST(success == true);
    BOOST_TEST(txData != nullptr);
    BOOST_TEST(txData->group_id == group_id);
    BOOST_TEST(txData->chain_id == chain_id);
    BOOST_TEST(txData->to == to);
    BOOST_TEST(txData->abi == abi);
    BOOST_TEST(txData->block_limit == block_limit);

    // auto json = bcos_sdk_encode_transaction_data_struct(txData);
    // BOOST_TEST(json == std::string("{\"groupId\":\"\",\"chainId\":\"\",\"to\":\"\",\"input\":\"\","
    //                                "\"abi\":\"\",\"blockLimit\":1}"));

    bcos_sdk_destroy_transaction_data_struct(txData);
}

BOOST_AUTO_TEST_SUITE_END()