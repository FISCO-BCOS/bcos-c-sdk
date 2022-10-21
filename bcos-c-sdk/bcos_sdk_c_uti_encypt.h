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
 * @file bcos_sdk_c_util_encrypt.h
 * @author: octopus
 * @date 2022-01-17
 */

#ifndef __INCLUDE_BCOS_SDK_C_UTIL_ENCRYPTO__
#define __INCLUDE_BCOS_SDK_C_UTIL_ENCRYPTO__

#ifdef __cplusplus
extern "C" {
#endif
// --------------------------------------------------------------------

/**
 * @brief create encryption object
 *
 * @param crypto_type
 * @return void*
 */
void* bcos_sdk_create_encrypt_obj(int crypto_type);

/**
 * @brief encrypt
 *
 * @param p
 * @param encrypt_data
 * @param encrypt_data_size
 * @param decrypt_data
 * @param decrypt_data_size
 */
void bcos_sdk_encrypt(void* p, void* encrypt_data, int encrypt_data_size, void** decrypt_data,
    int* decrypt_data_size);

/**
 * @brief decrypt
 *
 * @param p
 * @param decrypt_data
 * @param decrypt_data_size
 * @param encrypt_data
 * @param encrypt_data_size
 */
void bcos_sdk_decrypt(void* p, void* decrypt_data, int decrypt_data_size, void** encrypt_data,
    int* encrypt_data_size);

// --------------------------------------------------------------------


#ifdef __cplusplus
}
#endif
#endif
