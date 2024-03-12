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
 * @file bcos_sdk_c_uti_tx_struct_v2.h
 * @author: kyonGuo
 * @date 2024/3/7
 */

#ifndef BCOS_SDK_C_UTI_TX_STRUCT_V2_H
#define BCOS_SDK_C_UTI_TX_STRUCT_V2_H

#include "bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif
// transaction data

struct bcos_sdk_c_transaction_data_v2* bcos_sdk_create_transaction_data_struct_v2(
    transaction_version version, const char* group_id, const char* chain_id, const char* to,
    const char* nonce, const unsigned char* bytes_input, uint32_t bytes_input_length,
    const char* abi, int64_t block_limit, const char* value, const char* gas_price,
    int64_t gas_limit, const char* max_fee_per_gas, const char* max_priority_fee_per_gas,
    const unsigned char* extension, uint32_t extension_length);

void bcos_sdk_destroy_transaction_data_struct_v2(
    struct bcos_sdk_c_transaction_data_v2* transaction_data);

const char* bcos_sdk_calc_transaction_data_struct_hash_v2(
    int crypto_type, struct bcos_sdk_c_transaction_data_v2* transaction_data);

// transaction
struct bcos_sdk_c_transaction_v2* bcos_sdk_create_transaction_struct_v2(
    struct bcos_sdk_c_transaction_data_v2* transaction_data, const unsigned char* signature,
    uint32_t sign_length, const unsigned char* tx_data_hash, uint32_t hash_length,
    int32_t attribute, const char* extra_data);

void bcos_sdk_destroy_transaction_struct_v2(struct bcos_sdk_c_transaction_v2* transaction);

const char* bcos_sdk_create_encoded_transaction_v2(
    struct bcos_sdk_c_transaction_data_v2* transaction_data, const char* signature,
    const char* transaction_data_hash, int32_t attribute, const char* extra_data);

const char* bcos_sdk_encode_transaction_struct_to_hex_v2(
    struct bcos_sdk_c_transaction_v2* transaction);

const char* bcos_sdk_encode_transaction_struct_to_json_v2(
    struct bcos_sdk_c_transaction_v2* transaction);

struct bcos_sdk_c_transaction_v2* bcos_sdk_decode_transaction_struct_from_hex_v2(
    const char* transaction_hex_str);

struct bcos_sdk_c_transaction_v2* bcos_sdk_decode_transaction_struct_from_json_v2(
    const char* transaction_json_str);

#ifdef __cplusplus
}
#endif

#endif  // BCOS_SDK_C_UTI_TX_STRUCT_V2_H
