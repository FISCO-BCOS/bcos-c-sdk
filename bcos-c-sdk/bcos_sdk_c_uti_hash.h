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
 * @file bcos_sdk_c_util_hash.h
 * @author: octopus
 * @date 2022-01-17
 */

#ifndef __INCLUDE_BCOS_SDK_C_UTIL_HASH__
#define __INCLUDE_BCOS_SDK_C_UTIL_HASH__

#ifdef __cplusplus
extern "C" {
#endif

// --------------------------------------------------------------------

/**
 * @brief
 *
 * @param crypto_type
 * @return void*
 */
void* bcos_sdk_create_hash_obj(int hash_type);

/**
 * @brief
 *
 * @param p
 * @param data
 * @param size
 * @return void*
 */
void* bcos_sdk_hash(void* p, void* data, int size);

// --------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
