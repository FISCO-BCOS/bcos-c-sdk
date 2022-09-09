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
 * @file bcos_sdk_c_util_abi.cpp
 * @author: octopus
 * @date 2022-01-17
 */

#include "bcos_sdk_c_uti_abi.h"
#include "bcos_sdk_c_common.h"
#include "bcos_sdk_c_error.h"
#include <bcos-cpp-sdk/utilities/abi/ContractABICodec.h>
#include <bcos-utilities/DataConvertUtility.h>
#include <cstring>
#include <exception>
#include <memory>
#include <system_error>

using namespace bcos;
using namespace bcos::crypto;
using namespace bcos::cppsdk;
using namespace bcos::cppsdk::abi;

/**
 * @brief: encode constructor params
 *
 * @param abi: abi of the solidity contract
 * @param bin: bin of the solidity contract
 * @param params: params to be encoded, it should be in json string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: encoded abi params in hex string format, return NULL on failure
 */
const char* bcos_sdk_abi_encode_constructor(
    const char* abi, const char* bin, const char* params, int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(abi, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(bin, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(params, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);
    try
    {
        bcos::crypto::Hash::Ptr hashImpl = nullptr;
        if (crypto_type == BCOS_C_SDK_ECDSA_TYPE)
        {
            hashImpl = std::make_shared<Keccak256>();
        }
        else
        {
            hashImpl = std::make_shared<SM3>();
        }
        auto solcImpl = std::make_shared<ContractABITypeCodecSolImpl>();
        ContractABICodec codec(hashImpl, solcImpl);

        auto r = codec.encodeConstructor(abi, bin, params);
        return strdup(toHexStringWithPrefix(r).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        return NULL;
    }
}

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
    const char* abi, const char* method_name, const char* params, int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(abi, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(method_name, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(params, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        bcos::crypto::Hash::Ptr hashImpl = nullptr;
        if (crypto_type == BCOS_C_SDK_ECDSA_TYPE)
        {
            hashImpl = std::make_shared<Keccak256>();
        }
        else
        {
            hashImpl = std::make_shared<SM3>();
        }
        auto solcImpl = std::make_shared<ContractABITypeCodecSolImpl>();
        ContractABICodec codec(hashImpl, solcImpl);

        auto r = codec.encodeMethod(abi, method_name, params);
        return strdup(toHexStringWithPrefix(r).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        return NULL;
    }
}

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
    const char* abi, const char* method_id, const char* params, int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(abi, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(method_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(params, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        bcos::crypto::Hash::Ptr hashImpl = nullptr;
        if (crypto_type == BCOS_C_SDK_ECDSA_TYPE)
        {
            hashImpl = std::make_shared<Keccak256>();
        }
        else
        {
            hashImpl = std::make_shared<SM3>();
        }
        auto solcImpl = std::make_shared<ContractABITypeCodecSolImpl>();
        ContractABICodec codec(hashImpl, solcImpl);

        auto r = codec.encodeMethodByMethodID(abi, method_id, params);
        return strdup(toHexStringWithPrefix(r).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        return NULL;
    }
}

/**
 * @brief: encode method params by method signature, method signature should be in format:
 * method_name(type1 param1, type2 params2 ...)
 *
 * @param abi: abi of the solidity contract
 * @param method_sig: method signature
 * @param params: param to be encoded, json string format
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @return const char*: encoded abi params in hex string format, return NULL on failure
 */
const char* bcos_sdk_abi_encode_method_by_method_sig(
    const char* method_sig, const char* params, int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(method_sig, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(params, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        bcos::crypto::Hash::Ptr hashImpl = nullptr;
        if (crypto_type == BCOS_C_SDK_ECDSA_TYPE)
        {
            hashImpl = std::make_shared<Keccak256>();
        }
        else
        {
            hashImpl = std::make_shared<SM3>();
        }
        auto solcImpl = std::make_shared<ContractABITypeCodecSolImpl>();
        ContractABICodec codec(hashImpl, solcImpl);

        auto r = codec.encodeMethodBySignature(method_sig, params);
        return strdup(toHexStringWithPrefix(r).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        return NULL;
    }
}

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
    const char* abi, const char* method_name, const char* data, int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(abi, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(method_name, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(data, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        bcos::crypto::Hash::Ptr hashImpl = nullptr;
        if (crypto_type == BCOS_C_SDK_ECDSA_TYPE)
        {
            hashImpl = std::make_shared<Keccak256>();
        }
        else
        {
            hashImpl = std::make_shared<SM3>();
        }
        auto solcImpl = std::make_shared<ContractABITypeCodecSolImpl>();
        ContractABICodec codec(hashImpl, solcImpl);

        auto r = codec.decodeMethodInput(abi, method_name, *fromHexString(data));
        return strdup(toHexStringWithPrefix(r).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        return NULL;
    }
}

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
    const char* abi, const char* method_id, const char* data, int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(abi, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(method_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(data, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        bcos::crypto::Hash::Ptr hashImpl = nullptr;
        if (crypto_type == BCOS_C_SDK_ECDSA_TYPE)
        {
            hashImpl = std::make_shared<Keccak256>();
        }
        else
        {
            hashImpl = std::make_shared<SM3>();
        }
        auto solcImpl = std::make_shared<ContractABITypeCodecSolImpl>();
        ContractABICodec codec(hashImpl, solcImpl);

        auto r = codec.decodeMethodInputByMethodID(abi, method_id, *fromHexString(data));
        return strdup(toHexStringWithPrefix(r).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        return NULL;
    }
}

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
    const char* method_sig, const char* data, int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(method_sig, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(data, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        bcos::crypto::Hash::Ptr hashImpl = nullptr;
        if (crypto_type == BCOS_C_SDK_ECDSA_TYPE)
        {
            hashImpl = std::make_shared<Keccak256>();
        }
        else
        {
            hashImpl = std::make_shared<SM3>();
        }
        auto solcImpl = std::make_shared<ContractABITypeCodecSolImpl>();
        ContractABICodec codec(hashImpl, solcImpl);

        auto r = codec.decodeMethodInputByMethodSig(method_sig, *fromHexString(data));
        return strdup(toHexStringWithPrefix(r).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        return NULL;
    }
}

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
    const char* abi, const char* method_name, const char* data, int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(abi, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(method_name, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(data, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        bcos::crypto::Hash::Ptr hashImpl = nullptr;
        if (crypto_type == BCOS_C_SDK_ECDSA_TYPE)
        {
            hashImpl = std::make_shared<Keccak256>();
        }
        else
        {
            hashImpl = std::make_shared<SM3>();
        }
        auto solcImpl = std::make_shared<ContractABITypeCodecSolImpl>();
        ContractABICodec codec(hashImpl, solcImpl);

        auto r = codec.decodeMethodOutput(abi, method_name, *fromHexString(data));
        return strdup(toHexStringWithPrefix(r).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        return NULL;
    }
}

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
    const char* abi, const char* method_id, const char* data, int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(abi, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(method_id, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(data, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        bcos::crypto::Hash::Ptr hashImpl = nullptr;
        if (crypto_type == BCOS_C_SDK_ECDSA_TYPE)
        {
            hashImpl = std::make_shared<Keccak256>();
        }
        else
        {
            hashImpl = std::make_shared<SM3>();
        }
        auto solcImpl = std::make_shared<ContractABITypeCodecSolImpl>();
        ContractABICodec codec(hashImpl, solcImpl);

        auto r = codec.decodeMethodOutputByMethodID(abi, method_id, *fromHexString(data));
        return strdup(toHexStringWithPrefix(r).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        return NULL;
    }
}

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
    const char* abi, const char* event_name, const char* data, int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(abi, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(event_name, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(data, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        bcos::crypto::Hash::Ptr hashImpl = nullptr;
        if (crypto_type == BCOS_C_SDK_ECDSA_TYPE)
        {
            hashImpl = std::make_shared<Keccak256>();
        }
        else
        {
            hashImpl = std::make_shared<SM3>();
        }
        auto solcImpl = std::make_shared<ContractABITypeCodecSolImpl>();
        ContractABICodec codec(hashImpl, solcImpl);

        auto r = codec.decodeEvent(abi, event_name, *fromHexString(data));
        return strdup(toHexStringWithPrefix(r).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        return NULL;
    }
}

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
    const char* abi, const char* topic, const char* data, int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(abi, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(topic, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(data, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);

    try
    {
        bcos::crypto::Hash::Ptr hashImpl = nullptr;
        if (crypto_type == BCOS_C_SDK_ECDSA_TYPE)
        {
            hashImpl = std::make_shared<Keccak256>();
        }
        else
        {
            hashImpl = std::make_shared<SM3>();
        }
        auto solcImpl = std::make_shared<ContractABITypeCodecSolImpl>();
        ContractABICodec codec(hashImpl, solcImpl);

        auto r = codec.decodeEventByTopic(abi, topic, *fromHexString(data));
        return strdup(toHexStringWithPrefix(r).c_str());
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        return NULL;
    }
}
