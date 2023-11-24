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
 * @file bcos_sdk_c_util_tx_struct.h
 * @author: lucasli
 * @date 2023-05-08
 */

#ifndef __INCLUDE_BCOS_SDK_C_UTIL_TX_STRUCT__
#define __INCLUDE_BCOS_SDK_C_UTIL_TX_STRUCT__

#include "bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *
 * @param group_id
 * @param chain_id
 * @param to
 * @param input
 * @param abi
 * @param block_limit
 * @return bcos_sdk_c_transaction_data*: transaction data struct pointer, return unassigned struct
 * on failure according to the function called bcos_sdk_get_last_error(if create failed, return -1)
 */
struct bcos_sdk_c_transaction_data* bcos_sdk_create_transaction_data_struct_with_hex_input(
    const char* group_id, const char* chain_id, const char* to, const char* input, const char* abi,
    int64_t block_limit);

/**
 * @brief
 *
 * @param group_id
 * @param chain_id
 * @param to
 * @param bytes_input
 * @param bytes_input_length
 * @param abi
 * @param block_limit
 * @return bcos_sdk_c_transaction_data*: transaction data struct pointer, return unassigned struct
 * on failure according to the function called bcos_sdk_get_last_error(if create failed, return -1)
 */
struct bcos_sdk_c_transaction_data* bcos_sdk_create_transaction_data_struct_with_bytes(
    const char* group_id, const char* chain_id, const char* to, const unsigned char* bytes_input,
    uint32_t bytes_input_length, const char* abi, int64_t block_limit);

/**
 * @brief
 *
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 */
void bcos_sdk_destroy_transaction_data_struct(struct bcos_sdk_c_transaction_data* transaction_data);

/**
 * @brief encode transaction data into hex format
 *
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 * @return const char*
 */
const char* bcos_sdk_encode_transaction_data_struct(
    struct bcos_sdk_c_transaction_data* transaction_data);

/**
 * @brief convert transaction data into json format
 *
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 * @return const char*
 */
const char* bcos_sdk_encode_transaction_data_struct_to_json(
    struct bcos_sdk_c_transaction_data* transaction_data);

/**
 * @param transaction_data_hex_str
 * @return struct bcos_sdk_c_transaction_data*
 */
struct bcos_sdk_c_transaction_data* bcos_sdk_decode_transaction_data_struct(
    const char* transaction_data_hex_str);

/**
 * @param transaction_data_json_str
 *              version:number
 *              groupID:string
 *              chainID:string
 *              to:string
 *              data:hex string
 *              abi:string
 *              blockLimit:number
 *              nonce:string
 * @return struct bcos_sdk_c_transaction_data*
 */
struct bcos_sdk_c_transaction_data* bcos_sdk_decode_transaction_data_struct_with_json(
    const char* transaction_data_json_str);

/**
 * @brief
 *
 * @param crypto_type: int
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 * @return const char*
 */
const char* bcos_sdk_calc_transaction_data_struct_hash(
    int crypto_type, struct bcos_sdk_c_transaction_data* transaction_data);

/**
 * @brief
 *
 * @param crypto_type: int
 * @param transaction_data_hex: const char*
 * @return const char*
 */
const char* bcos_sdk_calc_transaction_data_struct_hash_with_hex(
    int crypto_type, const char* transaction_data_hex);

/**
 * @brief
 *
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 * @param signature
 * @param transaction_data_hash
 * @param attribute
 * @return struct bcos_sdk_c_transaction*
 */
struct bcos_sdk_c_transaction* bcos_sdk_create_transaction_struct(
    struct bcos_sdk_c_transaction_data* transaction_data, const char* signature,
    const char* transaction_data_hash, int32_t attribute, const char* extra_data);

/**
 * @brief
 *
 * @param transaction: struct bcos_sdk_c_transaction*
 */
void bcos_sdk_destroy_transaction_struct(struct bcos_sdk_c_transaction* transaction);

/**
 * @brief
 *
 * @param transaction_data: struct bcos_sdk_c_transaction_data*
 * @param signature
 * @param transaction_data_hash
 * @param attribute
 * @return const char*
 */
const char* bcos_sdk_create_encoded_transaction(
    struct bcos_sdk_c_transaction_data* transaction_data, const char* signature,
    const char* transaction_data_hash, int32_t attribute, const char* extra_data);

/**
 * @brief encode transaction into hex format
 *
 * @param transaction: struct bcos_sdk_c_transaction*
 * @return const char*
 */
const char* bcos_sdk_encode_transaction_struct(struct bcos_sdk_c_transaction* transaction);

/**
 * @brief convert transaction into json format
 *
 * @param transaction: struct bcos_sdk_c_transaction*
 * @return const char*
 */
const char* bcos_sdk_encode_transaction_struct_to_json(struct bcos_sdk_c_transaction* transaction);

/**
 * @brief
 *
 * @param transaction_hex_str
 * @return struct bcos_sdk_c_transaction*
 */
struct bcos_sdk_c_transaction* bcos_sdk_decode_transaction_struct(const char* transaction_hex_str);

/**
 * @brief
 *
 * @param transaction_json_str
 * @return struct bcos_sdk_c_transaction*
 */
struct bcos_sdk_c_transaction* bcos_sdk_decode_transaction_struct_with_json(
    const char* transaction_json_str);

struct bcos_sdk_c_transaction_data_v2* bcos_sdk_create_transaction_data_struct_with_hex_input_v2(
    const char* group_id, const char* chain_id, const char* to, const char* input, const char* abi,
    int64_t block_limit, const char* value, const char* gas_price, int64_t gas_limit, const char* max_fee_per_gas, const char* max_priority_fee_per_gas);

struct bcos_sdk_c_transaction_data_v2* bcos_sdk_create_transaction_data_struct_with_bytes_v2(
    const char* group_id, const char* chain_id, const char* to, const unsigned char* bytes_input,
    uint32_t bytes_input_length, const char* abi, int64_t block_limit, const char* value, const char* gas_price, int64_t gas_limit, const char* max_fee_per_gas, const char* max_priority_fee_per_gas);

void bcos_sdk_destroy_transaction_data_struct_v2(struct bcos_sdk_c_transaction_data_v2* transaction_data);

const char* bcos_sdk_encode_transaction_data_struct_v2(struct bcos_sdk_c_transaction_data_v2* transaction_data);

const char* bcos_sdk_encode_transaction_data_struct_to_json_v2(struct bcos_sdk_c_transaction_data_v2* transaction_data);

struct bcos_sdk_c_transaction_data_v2* bcos_sdk_decode_transaction_data_struct_v2(
    const char* transaction_data_hex_str);

struct bcos_sdk_c_transaction_data_v2* bcos_sdk_decode_transaction_data_struct_with_json_v2(
    const char* transaction_data_json_str);

const char* bcos_sdk_calc_transaction_data_struct_hash_v2(
    int crypto_type, struct bcos_sdk_c_transaction_data_v2* transaction_data);

const char* bcos_sdk_calc_transaction_data_struct_hash_with_hex_v2(
    int crypto_type, const char* transaction_data_hex);

struct bcos_sdk_c_transaction_v2* bcos_sdk_create_transaction_struct_v2(
    struct bcos_sdk_c_transaction_data_v2* transaction_data, const char* signature,
    const char* transaction_data_hash, int32_t attribute, const char* extra_data);

void bcos_sdk_destroy_transaction_struct_v2(struct bcos_sdk_c_transaction_v2* transaction);

const char* bcos_sdk_create_encoded_transaction_v2(
    struct bcos_sdk_c_transaction_data_v2* transaction_data, const char* signature,
    const char* transaction_data_hash, int32_t attribute, const char* extra_data);

const char* bcos_sdk_encode_transaction_struct_v2(struct bcos_sdk_c_transaction_v2* transaction);

const char* bcos_sdk_encode_transaction_struct_to_json_v2(struct bcos_sdk_c_transaction_v2* transaction);

struct bcos_sdk_c_transaction_v2* bcos_sdk_decode_transaction_struct_v2(
    const char* transaction_hex_str);

struct bcos_sdk_c_transaction_v2* bcos_sdk_decode_transaction_struct_with_json_v2(
    const char* transaction_json_str);

#ifdef __cplusplus
}
#endif
#endif
