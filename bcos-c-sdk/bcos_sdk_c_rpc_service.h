/*
 *  Copyright (C) 2023 FISCO BCOS.
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
 * @file bcos_sdk_c_rpc_service.h
 * @author: octopus
 * @date 2023-02-26
 */

#ifndef __INCLUDE_BCOS_SDK_TARS_RPC__
#define __INCLUDE_BCOS_SDK_TARS_RPC__

#include "bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  these are tars rpc interfaces
 */

// ------------------------------tars rpc interface begin -------------------------
// sendTransaction
const char* bcos_rpc_service_send_transaction(void* sdk, void* key_pair, const char* group,
    const char* node, const char* to, void* data, int size, const char* abi, int attribute,
    const char* extra_data, bcos_sdk_c_struct_response_cb callback, void* context);

// ------------------------------tars rpc interface end -------------------------

#ifdef __cplusplus
}
#endif
#endif
