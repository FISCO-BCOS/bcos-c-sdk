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

void* bcos_sdk_create_keypair(int crypto_type);

void* bcos_sdk_load_keypair(const char* pem_path);

void bcos_sdk_destroy_keypair(void* key_pair);

const char* bcos_sdk_get_keypair_address(void* key_pair);

const char* bcos_sdk_get_keypair_public_key(void* key_pair);

const char* bcos_sdk_get_keypair_private_key(void* key_pair);

// --------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
