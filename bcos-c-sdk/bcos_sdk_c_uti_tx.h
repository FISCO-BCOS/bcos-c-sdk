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
 * @file bcos_sdk_c_util_tx.h
 * @author: octopus
 * @date 2022-01-17
 */

#ifndef __INCLUDE_BCOS_SDK_C_UTIL_TX__
#define __INCLUDE_BCOS_SDK_C_UTIL_TX__

#include "bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif

const char* bcos_sdk_create_signed_tx(void* key_pair, const char* to, const char* data,
    const char* chain_id, const char* group_id, int64_t block_limit);

#ifdef __cplusplus
}
#endif
#endif
