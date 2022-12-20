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
 * @file bcos_sdk_c_util_signature.h
 * @author: lucasli
 * @date 2022-12-19
 */

#ifndef __INCLUDE_BCOS_SDK_C_UTIL_SIGNATURE__
#define __INCLUDE_BCOS_SDK_C_UTIL_SIGNATURE__

#include "bcos_sdk_c_common.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief : transaction sign
 *
 * @param keypair: key pair object pointer
 * @param hash: hash message to be signed
 * @param hsm_lib_path: the path of hsm library
 *
 * @return bcos_sdk_c_signature_result*: signature data struct pointer, return empty struct on
 * failure according to the function called bcos_sdk_get_last_error(if sign failed, return -1)
 */
struct bcos_sdk_c_signature_result bcos_sdk_sign(
    void* key_pair, const char* hash, const char* hsm_lib_path);

/**
 * @brief : verify the signature data of transaction
 *
 * @param crypto_type: crypto type
 * @param publickey: public key for verify
 * @param pubkey_len: length of public key
 * @param hash: hash message
 * @param signature_data: the signature data
 * @param size: size of signature data
 * @param hsm_lib_path: the path of hsm library
 *
 * @return bool: return fasle if verify failed
 */
bool bcos_sdk_verify(int crypto_type, void* public_key, size_t pubkey_len, const char* hash,
    void* signature_data, size_t size, const char* hsm_lib_path);

#ifdef __cplusplus
}
#endif
#endif
