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
 * @file bcos_sdk_c_util_signature.cpp
 * @author: lucasli
 * @date 2022-12-19
 */

#include "bcos_sdk_c_uti_signature.h"
#include "bcos_sdk_c_error.h"
#include <bcos-cpp-sdk/utilities/crypto/Signature.h>
#include <bcos-crypto/hash/SM3.h>
#include <bcos-utilities/Common.h>
#include <cstddef>
#include <cstdio>
#include <exception>
#include <memory>

using namespace bcos;
using namespace bcos::cppsdk;
using namespace bcos::cppsdk::utilities;

/**
 * @brief : transaction sign
 *
 * @param keypair: key pair object pointer
 * @param hash: hash message to be signed
 * @param hsm_lib_path: the path of hsm library
 *
 * @return bcos_sdk_c_signature_result*: signature data struct pointer, return unassigned struct on
 * failure according to the function called bcos_sdk_get_last_error(if sign failed, return -1)
 */
struct bcos_sdk_c_signature_result bcos_sdk_sign(
    void* key_pair, const char* hash, const char* hsm_lib_path)
{
    // if sign failed, return unassigned struct
    // the function caller knows sign failed according to the function called
    // bcos_sdk_get_last_error(if sign failed, return -1)
    struct bcos_sdk_c_signature_result signatureResult;
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, signatureResult);
    BCOS_SDK_C_PARAMS_VERIFICATION(hash, signatureResult);

    try
    {
        auto keyPair = (bcos::crypto::KeyPairInterface*)key_pair;
        bcos::crypto::HashType hashType(hash);
        std::string hsmLibPath(hsm_lib_path);
        auto signature = std::make_shared<bcos::cppsdk::utilities::Signature>();

        auto signatureData = signature->sign(*keyPair, hashType, hsmLibPath);
        memcpy(signatureResult.r, signatureData->data(), 32);
        memcpy(signatureResult.s, signatureData->data() + 32, 32);
        memcpy(signatureResult.v, signatureData->data() + 64, signatureData->size() - 64);

        return signatureResult;
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_sign") << LOG_KV("errorMsg", errorMsg);
        return signatureResult;
    }
}

/**
 * @brief : verify the signature data of transaction
 *
 * @param crypto_type: crypto type
 * @param publickey: publish key for verify
 * @param hash: hash message
 * @param signature_data: the signature data
 * @param size: size of signature data
 * @param hsm_lib_path: the path of hsm library
 *
 * @return bool: return fasle if verify failed
 */
bool bcos_sdk_verify(int crypto_type, void* public_key, size_t pubkey_len, const char* hash,
    void* signature_data, size_t size, const char* hsm_lib_path)
{
    BCOS_SDK_C_PARAMS_VERIFICATION(public_key, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(hash, NULL);
    BCOS_SDK_C_PARAMS_VERIFICATION(signature_data, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (pubkey_len > 0), "invalid length of public key, it must greater than zero", NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (size > 0), "invalid size of signature data, it must greater than zero", NULL);

    try
    {
        auto pubBytes = std::make_shared<bcos::bytes>(
            (bcos::byte*)public_key, (bcos::byte*)public_key + pubkey_len);
        bcos::crypto::HashType hashType(hash);
        std::string hsmLibPath(hsm_lib_path);
        auto signature = std::make_shared<bcos::cppsdk::utilities::Signature>();
        return signature->verify((CryptoType)crypto_type, pubBytes, hashType,
            bytesConstRef((byte*)signature_data, size), hsmLibPath);
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_verify") << LOG_KV("errorMsg", errorMsg);
        return false;
    }
}