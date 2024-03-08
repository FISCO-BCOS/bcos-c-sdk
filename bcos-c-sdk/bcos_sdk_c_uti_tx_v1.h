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
 * @file bcos_sdk_c_uti_tx_v1.h
 * @author: kyonGuo
 * @date 2023/11/28
 */

#ifndef BCOS_C_SDK_BCOS_SDK_C_UTI_TX_V1_H
#define BCOS_C_SDK_BCOS_SDK_C_UTI_TX_V1_H

#include "bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief create transaction data with version 1 by default
 * @note version 1 transaction only supported in FISCO BCOS 3.6.0 and later
 *
 * @param group_id group id
 * @param chain_id chain id
 * @param to contract address, if it is a contract creation transaction, it can be empty
 * @param nonce random number to avoid duplicate transactions, if empty, then will generate one
 * @param input encoded contract method and params, bytes array
 * @param inputSize encoded contract method and params size
 * @param abi contract abi, only create contract need
 * @param block_limit block limit
 * @param value transfer value
 * @param gas_price gas price
 * @param gas_limit gas limit
 * @return void* transaction data pointer, you should release it after use
 */
void* bcos_sdk_create_transaction_v1_data(const char* group_id, const char* chain_id,
    const char* to, const char* nonce, const unsigned char* input, long inputSize, const char* abi,
    int64_t block_limit, const char* value, const char* gas_price, int64_t gas_limit);

/**
 * @brief create eip1559 transaction data with version 1 by default
 * @note version 1 transaction only supported in FISCO BCOS 3.6.0 and later
 *
 * @param group_id group id
 * @param chain_id chain id
 * @param to contract address, if it is a contract creation transaction, it can be empty
 * @param nonce random number to avoid duplicate transactions, if empty, then will generate one
 * @param input encoded contract method and params, bytes array
 * @param inputSize encoded contract method and params size
 * @param abi contract abi, only create contract need
 * @param block_limit block limit
 * @param value transfer value
 * @param gas_limit gas limit
 * @param max_fee_per_gas max fee per gas
 * @param max_priority_fee_per_gas max priority fee per gas
 * @return void* transaction data pointer, you should release it after use
 */
void* bcos_sdk_create_eip1559_transaction_data(const char* group_id, const char* chain_id,
    const char* to, const char* nonce, const unsigned char* input, long inputSize, const char* abi,
    int64_t block_limit, const char* value, int64_t gas_limit, const char* max_fee_per_gas,
    const char* max_priority_fee_per_gas);

/**
 * @brief create transaction data with full fields
 * @note version 1 transaction only supported in FISCO BCOS 3.6.0 and later
 *
 * @param crypto_type 0: keccak256, 1: sm3
 * @param version tx version, if version==1, then enable
 * (value,gasPrice,gasLimit,maxFeePerGas,maxPriorityFeePerGas) fields
 * @param group_id group id
 * @param chain_id chain id
 * @param to contract address, if it is a contract creation transaction, it can be empty
 * @param nonce nonce, random number to avoid duplicate transactions
 * @param input encoded contract method and params, bytes array
 * @param inputSize encoded contract method and params size
 * @param abi contract abi, only create contract need
 * @param block_limit block limit
 * @param value transfer value
 * @param gas_price gas price
 * @param gas_limit gas limit
 * @param max_fee_per_gas max fee per gas
 * @param max_priority_fee_per_gas max priority fee per gas
 * @return const char* transaction data hash hex string
 */
const char* bcos_sdk_calc_transaction_data_hash_with_full_fields(int crypto_type,
    transaction_version version, const char* group_id, const char* chain_id, const char* to,
    const char* nonce, const unsigned char* input, long inputSize, const char* abi,
    int64_t block_limit, const char* value, const char* gas_price, int64_t gas_limit,
    const char* max_fee_per_gas, const char* max_priority_fee_per_gas);

/**
 * @brief create transaction data with json string
 * @note version 1 transaction only supported in FISCO BCOS 3.6.0 and later
 *
 * @param crypto_type 0: keccak256, 1: sm3
 * @param json transaction data json string
 * @return const char* transaction data hash hex string
 * @throw exception if lack of some required fields, or some fields are invalid
 */
const char* bcos_sdk_calc_transaction_data_hash_with_json(int crypto_type, const char* json);

/**
 * @brief create encoded transaction data with external signature
 * @note version 1 transaction only supported in FISCO BCOS 3.6.0 and later
 *
 * @param signature signature bytes array, if ECDSA, it is r||s||v, if SM2, it is r||s||pk
 * @param signSize signature bytes array size
 * @param transaction_hash transactionData hash hex string
 * @param version tx version, only support 0 and 1 now, if version==1, then enable
 * (value,gasPrice,gasLimit,maxFeePerGas,maxPriorityFeePerGas) fields
 * @param group_id group id
 * @param chain_id chain id
 * @param to contract address, if it is a contract creation transaction, it can be empty
 * @param nonce nonce, random number to avoid duplicate transactions
 * @param input encoded contract method and params, bytes array
 * @param inputSize encoded contract method and params size
 * @param abi contract abi, only create contract need
 * @param block_limit block limit
 * @param value transfer value
 * @param gas_price gas price
 * @param gas_limit gas limit
 * @param max_fee_per_gas max fee per gas
 * @param max_priority_fee_per_gas max priority fee per gas
 * @param attribute transaction attribute
 * @param extra_data extra data in transaction
 * @return const char* encoded transaction hex string
 */
const char* bcos_sdk_create_signed_transaction_with_signature(const unsigned char* signature,
    long signSize, const char* transaction_hash, transaction_version version, const char* group_id,
    const char* chain_id, const char* to, const char* nonce, const unsigned char* input,
    long inputSize, const char* abi, int64_t block_limit, const char* value, const char* gas_price,
    int64_t gas_limit, const char* max_fee_per_gas, const char* max_priority_fee_per_gas,
    int32_t attribute, const char* extra_data);

/**
 * @brief create transaction with full fields, with version 1 by default
 * @note version 1 transaction only supported in FISCO BCOS 3.6.0 and later
 *
 * @param key_pair key pair pointer
 * @param group_id group id
 * @param chain_id chain id
 * @param to contract address, if it is a contract creation transaction, it can be empty
 * @param nonce random number to avoid duplicate transactions, if empty, then will generate one
 * @param input encoded contract method and params, bytes array
 * @param inputSize encoded contract method and params size
 * @param abi contract abi, only create contract need
 * @param block_limit block limit
 * @param value transfer value
 * @param gas_price gas price
 * @param gas_limit gas limit
 * @param attribute transaction attribute
 * @param extra_data extra data in transaction
 * @param tx_hash output transaction hash hex string
 * @param signed_tx output signed transaction hex string
 */
void bcos_sdk_create_signed_transaction_with_full_fields(void* key_pair, const char* group_id,
    const char* chain_id, const char* to, const char* nonce, const unsigned char* input,
    long inputSize, const char* abi, int64_t block_limit, const char* value, const char* gas_price,
    int64_t gas_limit, int32_t attribute, const char* extra_data, char** tx_hash, char** signed_tx);

/**
 * @brief create eip1559 transaction with full fields, with version 1 by default
 * @note version 1 transaction only supported in FISCO BCOS 3.6.0 and later
 *
 * @param key_pair key pair pointer
 * @param group_id group id
 * @param chain_id chain id
 * @param to contract address, if it is a contract creation transaction, it can be empty
 * @param nonce random number to avoid duplicate transactions, if empty, then will generate one
 * @param input encoded contract method and params, bytes array
 * @param inputSize encoded contract method and params size
 * @param abi contract abi, only create contract need
 * @param block_limit block limit
 * @param value transfer value
 * @param gas_limit gas limit
 * @param max_fee_per_gas max fee per gas
 * @param max_priority_fee_per_gas max priority fee per gas
 * @param attribute transaction attribute
 * @param extra_data extra data in transaction
 * @param tx_hash output transaction hash hex string
 * @param signed_tx output signed transaction hex string
 */
void bcos_sdk_create_signed_eip1559_transaction_with_full_fields(void* key_pair,
    const char* group_id, const char* chain_id, const char* to, const char* nonce,
    const unsigned char* input, long inputSize, const char* abi, int64_t block_limit,
    const char* value, int64_t gas_limit, const char* max_fee_per_gas,
    const char* max_priority_fee_per_gas, int32_t attribute, const char* extra_data, char** tx_hash,
    char** signed_tx);

#ifdef __cplusplus
}
#endif
#endif  // BCOS_C_SDK_BCOS_SDK_C_UTI_TX_V1_H
