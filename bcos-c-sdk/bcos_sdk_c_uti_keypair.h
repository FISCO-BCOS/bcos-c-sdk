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
 * @file bcos_sdk_c_util_keypair.h
 * @author: octopus
 * @date 2022-01-17
 */

#ifndef __INCLUDE_BCOS_SDK_C_UTIL_KEYPAIR__
#define __INCLUDE_BCOS_SDK_C_UTIL_KEYPAIR__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief : create key pair used for transaction sign
 *
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 *
 * @return void*: key pair object pointer, return NULL on failure
 */
void* bcos_sdk_create_keypair(int crypto_type);

/**
 * @brief : create key pair used for transaction sign
 *
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @param private_key: private key in bytes format
 * @param length     : private key bytes length
 *
 * @return void*: key pair object pointer, return NULL on failure
 */
void* bcos_sdk_create_keypair_by_prikey(int crypto_type, void* private_key, unsigned length);

/**
 * @brief : create key pair used for transaction sign
 *
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @param private_key: private key in hex string format
 *
 * @return void*: key pair object pointer, return NULL on failure
 */
void* bcos_sdk_create_keypair_by_hex_prikey(int crypto_type, const char* private_key);

/**
 * @brief : destroy the keypair object
 *
 * @param key_pair: key pair object pointer
 */
void bcos_sdk_destroy_keypair(void* key_pair);

/**
 * @brief : get the crypto type of the keypair
 *
 * @param key_pair: key pair object pointer
 *
 * @return int : ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE, return -1 on failure
 */
int bcos_sdk_get_keypair_type(void* key_pair);

/**
 * @brief : get the address of the keypair private key
 *
 * @param key_pair : key pair object pointer
 *
 * @return const char* : keypair address
 */
const char* bcos_sdk_get_keypair_address(void* key_pair);

/**
 * @brief : get the publish key of the keypair
 *
 * @param key_pair : key pair object pointer
 *
 * @return const char* : hex string format publish key, return NULL on failure
 */
const char* bcos_sdk_get_keypair_public_key(void* key_pair);

/**
 * @brief : get the private key of the keypair
 *
 * @param key_pair : key pair object pointer
 *
 * @return const char* : hex string format private key, return NULL on failure
 */
const char* bcos_sdk_get_keypair_private_key(void* key_pair);

// --------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
