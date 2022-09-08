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
 * @file bcos_sdk_c_util_abi.h
 * @author: octopus
 * @date 2022-09-08
 */

#ifndef __INCLUDE_BCOS_SDK_C_UTIL_ABI__
#define __INCLUDE_BCOS_SDK_C_UTIL_ABI__

#include "bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief: encode constructor params
 *
 * @param abi: abi of the solidity contract
 * @param bin: bin of the solidity contract
 * @param params: params to be encoded, json string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: encoded abi params in hex string format, return NULL on failure
 */
const char* bcos_sdk_abi_encode_constructor(
    const char* abi, const char* bin, const char* params, int crypto_type);

/**
 * @brief: encode method params
 *
 * @param abi: abi of the solidity contract
 * @param method_name: method name
 * @param params: param to be encoded, json string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: encoded abi params in hex string format, return NULL on failure
 */
const char* bcos_sdk_abi_encode_method(
    const char* abi, const char* method_name, const char* params, int crypto_type);

/**
 * @brief: encode method params by method id
 *
 * @param abi: abi of the solidity contract
 * @param method_id: method id
 * @param params: param to be encoded, json string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: encoded abi params in hex string format, return NULL on failure
 */
const char* bcos_sdk_abi_encode_method_by_method_id(
    const char* abi, const char* method_id, const char* params, int crypto_type);

/**
 * @brief: encode method params by method signature, method signature should be in format:
 * method_name(type1 param1, type2 params2 ...)
 *
 * @param abi: abi of the solidity contract
 * @param method_sig: method signature, construct the method abi from method signature
 * @param params: param to be encoded, json string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: encoded abi params in hex string format, return NULL on failure
 */
const char* bcos_sdk_abi_encode_method_by_method_sig(
    const char* method_sig, const char* params, int crypto_type);

/**
 * @brief: decode method input params
 *
 * @param abi: abi of the solidity contract
 * @param method_name: method name
 * @param data: data to be decode, hex string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: decode abi params in json string format, return NULL on failure
 */
const char* bcos_sdk_abi_decode_method_input(
    const char* abi, const char* method_name, const char* data, int crypto_type);

/**
 * @brief: decode method input params by method id
 *
 * @param abi: abi of the solidity contract
 * @param method_id: method id
 * @param data: data to be decode, hex string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: decode abi params in json string format, return NULL on failure
 */
const char* bcos_sdk_abi_decode_method_input_by_method_id(
    const char* abi, const char* method_id, const char* data, int crypto_type);

/**
 * @brief: decode method input params by method signature, method signature should be in format:
 * method_name(type1 param1, type2 params2 ...)
 *
 * @param method_sig: method signature
 * @param data: data to be decode, hex string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: decode abi params in json string format, return NULL on failure
 */
const char* bcos_sdk_abi_decode_method_input_by_method_sig(
    const char* method_sig, const char* data, int crypto_type);

/**
 * @brief: decode method output params
 *
 * @param abi: abi of the solidity contract
 * @param method_name: method name
 * @param data: data to be decode, hex string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: decode abi params in json string format, return NULL on failure
 */
const char* bcos_sdk_abi_decode_method_output(
    const char* abi, const char* method_name, const char* data, int crypto_type);

/**
 * @brief: decode method output params by method id
 *
 * @param abi: abi of the solidity contract
 * @param method_id: method id
 * @param data: data to be decode, hex string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: decode abi params in json string format, return NULL on failure
 */
const char* bcos_sdk_abi_decode_method_output_by_method_id(
    const char* abi, const char* method_id, const char* data, int crypto_type);

/**
 * @brief: decode event params
 *
 * @param abi: abi of the solidity contract
 * @param event_name: event name
 * @param data: data to be decode, hex string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: decode event params in json string format, return NULL on failure
 */
const char* bcos_sdk_abi_decode_event(
    const char* abi, const char* event_name, const char* data, int crypto_type);

/**
 * @brief: decode event params by topic
 *
 * @param abi: abi of the solidity contract
 * @param topic: event topic
 * @param data: data to be decode, hex string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: decode event params in json string format, return NULL on failure
 */
const char* bcos_sdk_abi_decode_event_by_topic(
    const char* abi, const char* topic, const char* data, int crypto_type);

#ifdef __cplusplus
}
#endif
#endif
