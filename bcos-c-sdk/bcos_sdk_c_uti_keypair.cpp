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
 * @file bcos_sdk_c_util_keypair.cpp
 * @author: octopus
 * @date 2022-01-17
 */

#include "bcos_sdk_c_uti_keypair.h"
#include "bcos_sdk_c_common.h"
#include "bcos_sdk_c_error.h"
#include <bcos-cpp-sdk/utilities/crypto/KeyPairBuilder.h>
#include <bcos-crypto/hash/Keccak256.h>
#include <bcos-crypto/hash/SM3.h>
#include <bcos-crypto/interfaces/crypto/CryptoSuite.h>
#include <bcos-crypto/signature/key/KeyImpl.h>
#include <bcos-utilities/Common.h>
#include <cstddef>
#include <exception>
#include <memory>

using namespace bcos;
using namespace bcos::cppsdk;
using namespace bcos::cppsdk::utilities;
/**
 * @brief : create key pair used for transaction sign
 *
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 *
 * @return void*: key pair object pointer, return NULL on failure
 */
void* bcos_sdk_create_keypair(int crypto_type)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);
    try
    {
        auto keyPairBuilder = std::make_shared<KeyPairBuilder>();
        auto keyPair = keyPairBuilder->genKeyPair(
            crypto_type == BCOS_C_SDK_ECDSA_TYPE ? CryptoType::Secp256K1 : CryptoType::SM2);
        return keyPair.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_create_keypair") << LOG_KV("errorMsg", errorMsg);
        return NULL;
    }
}

/**
 * @brief : create hsm key pair used for transaction sign
 *
 * @param hsm_lib_path: the path of hsm library
 *
 * @return void*: key pair object pointer, return NULL on failure
 */
void* bcos_sdk_create_hsm_keypair(const char* hsm_lib_path)
{
    bcos_sdk_clear_last_error();
    try
    {
        auto keyPairBuilder = std::make_shared<KeyPairBuilder>();
        std::string hsmLibPath(hsm_lib_path);
        auto keyPair = keyPairBuilder->genKeyPair(CryptoType::HsmSM2, hsmLibPath);
        return keyPair.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());
        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_create_hsm_keypair") << LOG_KV("errorMsg", errorMsg);
        return NULL;
    }
}

/**
 * @brief : create key pair used for transaction sign
 *
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @param private_key: private key in bytes format
 * @param length     : private key bytes length
 *
 * @return void*: key pair object pointer, return NULL on failure
 */
void* bcos_sdk_create_keypair_by_private_key(int crypto_type, void* private_key, unsigned len)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(private_key, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (len > 0), "invalid private key length, it must greater than zero", NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be BCOS_C_SDK_ECDSA_TYPE(ecdsa crypto type) or "
        "BCOS_C_SDK_SM_TYPE(sm crypto type)",
        NULL);
    try
    {
        auto priBytes =
            std::make_shared<bcos::bytes>((bcos::byte*)private_key, (bcos::byte*)private_key + len);
        /*
        ECDSA_TYPE  1,
        SM_TYPE  2
        */
        auto keyPairBuilder = std::make_shared<KeyPairBuilder>();
        auto keyPair = keyPairBuilder->genKeyPair(
            crypto_type == BCOS_C_SDK_ECDSA_TYPE ? CryptoType::Secp256K1 : CryptoType::SM2,
            bytesConstRef((byte*)private_key, len));
        return keyPair.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());

        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_create_keypair_by_private_key")
                        << LOG_KV("crypto_type", crypto_type) << LOG_KV("errorMsg", errorMsg);
        return NULL;
    }
}

/**
 * @brief : create hsm key pair used for transaction sign
 *
 * @param private_key: private key in bytes format
 * @param length     : private key bytes length
 * @param hsm_lib_path: the path of hsm library
 *
 * @return void*: key pair object pointer, return NULL on failure
 */
void* bcos_sdk_create_hsm_keypair_by_private_key(
    void* private_key, unsigned length, const char* hsm_lib_path)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(private_key, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (length > 0), "invalid private key length, it must greater than zero", NULL);
    try
    {
        auto priBytes = std::make_shared<bcos::bytes>(
            (bcos::byte*)private_key, (bcos::byte*)private_key + length);
        auto keyPairBuilder = std::make_shared<KeyPairBuilder>();
        auto keyPair = keyPairBuilder->genKeyPair(
            CryptoType::HsmSM2, bytesConstRef((byte*)private_key, length), hsm_lib_path);
        return keyPair.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());

        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_create_hsm_keypair_by_private_key")
                        << LOG_KV("errorMsg", errorMsg);
        return NULL;
    }
}

/**
 * @brief : create key pair used for transaction sign
 *
 * @param crypto_type: crypto type, ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE
 * @param private_key: private key in hex string format
 *
 * @return void*: key pair object pointer, return NULL on failure
 */
void* bcos_sdk_create_keypair_by_hex_private_key(int crypto_type, const char* private_key)
{
    bcos_sdk_clear_last_error();

    BCOS_SDK_C_PARAMS_VERIFICATION(private_key, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (crypto_type == BCOS_C_SDK_ECDSA_TYPE || crypto_type == BCOS_C_SDK_SM_TYPE),
        "invalid crypto type, it must be 0(ecdsa crypto type) or 1(sm crypto type)", NULL)
    try
    {
        auto priBytes = fromHexString(std::string(private_key));
        /*
        ECDSA_TYPE  1,
        SM_TYPE  2
        */
        auto keyPairBuilder = std::make_shared<KeyPairBuilder>();
        auto keyPair = keyPairBuilder->genKeyPair(
            crypto_type == BCOS_C_SDK_ECDSA_TYPE ? CryptoType::Secp256K1 : CryptoType::SM2,
            bytesConstRef((byte*)priBytes->data(), priBytes->size()));
        return keyPair.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());

        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_create_keypair_by_hex_private_key")
                        << LOG_KV("crypto_type", crypto_type) << LOG_KV("errorMsg", errorMsg);
        return NULL;
    }
}

/**
 * @brief : create hsm key pair used for transaction sign
 *
 * @param private_key: private key in hex string format
 * @param hsm_lib_path: the path of hsm library
 *
 * @return void*: key pair object pointer, return NULL on failure
 */
void* bcos_sdk_create_hsm_keypair_by_hex_private_key(
    const char* private_key, const char* hsm_lib_path)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(private_key, NULL);
    try
    {
        auto priBytes = fromHexString(std::string(private_key));
        auto keyPairBuilder = std::make_shared<KeyPairBuilder>();
        auto keyPair = keyPairBuilder->genKeyPair(CryptoType::HsmSM2,
            bytesConstRef((byte*)priBytes->data(), priBytes->size()), hsm_lib_path);
        return keyPair.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());

        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_create_hsm_keypair_by_hex_private_key")
                        << LOG_KV("errorMsg", errorMsg);
        return NULL;
    }
}

/**
 * @brief : use hsm key pair for transaction sign according to the keyindex and password
 *
 * @param key_index: key index inside the HSM
 * @param password: the password for the permission to use HSM
 * @param hsm_lib_path: the path of hsm library
 *
 * @return void*: key pair object pointer, return NULL on failure
 */
void* bcos_sdk_use_hsm_keypair_by_keyindex_and_password(
    unsigned key_index, const char* password, const char* hsm_lib_path)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(password, NULL);
    BCOS_SDK_C_PARAMS_VERIFY_CONDITION(
        (key_index > 0), "invalid key index of HSM, it must greater than zero", NULL);
    try
    {
        auto keyPairBuilder = std::make_shared<KeyPairBuilder>();
        auto keyPair = keyPairBuilder->useHsmKeyPair(key_index, password, hsm_lib_path);
        return keyPair.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());

        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_use_hsm_keypair_by_keyindex_and_password")
                        << LOG_KV("errorMsg", errorMsg);
        return NULL;
    }
}

/**
 * @brief : destroy the keypair object
 *
 * @param key_pair: key pair object pointer
 */
void bcos_sdk_destroy_keypair(void* key_pair)
{
    bcos_sdk_clear_last_error();
    if (key_pair)
    {
        delete (bcos::crypto::KeyPairInterface*)key_pair;
        key_pair = NULL;
    }
}

/**
 * @brief : get the crypto type of the keypair
 *
 * @param key_pair: key pair object pointer
 *
 * @return int : ECDSA: BCOS_C_SDK_ECDSA_TYPE, SM: BCOS_C_SDK_SM_TYPE, return -1 on failure
 */
int bcos_sdk_get_keypair_type(void* key_pair)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, -1);

    auto keyPair = (bcos::crypto::KeyPairInterface*)key_pair;
    return (int)keyPair->keyPairType();
}

/**
 * @brief : get the address of the keypair private key
 *
 * @param key_pair : key pair object pointer
 *
 * @return const char* : keypair address
 */
const char* bcos_sdk_get_keypair_address(void* key_pair)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, NULL);

    auto keyPair = (bcos::crypto::KeyPairInterface*)key_pair;
    auto cryptoType = (int)keyPair->keyPairType();
    if (cryptoType == BCOS_C_SDK_ECDSA_TYPE)
    {
        auto cryptoSuite = std::make_unique<bcos::crypto::CryptoSuite>(
            std::make_shared<bcos::crypto::Keccak256>(), nullptr, nullptr);
        return strdup(cryptoSuite->calculateAddress(keyPair->publicKey()).hexPrefixed().c_str());
    }
    else
    {
        auto cryptoSuite = std::make_unique<bcos::crypto::CryptoSuite>(
            std::make_shared<bcos::crypto::SM3>(), nullptr, nullptr);
        return strdup(cryptoSuite->calculateAddress(keyPair->publicKey()).hexPrefixed().c_str());
    }
}

/**
 * @brief : get the publish key of the keypair
 *
 * @param key_pair : key pair object pointer
 *
 * @return const char* : hex string format publish key, return NULL on failure
 */
const char* bcos_sdk_get_keypair_public_key(void* key_pair)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, NULL);

    auto priKey = ((bcos::crypto::KeyPairInterface*)key_pair)->publicKey();
    return strdup(priKey->hex().c_str());
}

/**
 * @brief : get the private key of the keypair
 *
 * @param key_pair : key pair object pointer
 *
 * @return const char* : hex string format private key, return NULL on failure
 */
const char* bcos_sdk_get_keypair_private_key(void* key_pair)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, NULL);

    auto priKey = ((bcos::crypto::KeyPairInterface*)key_pair)->secretKey();
    return strdup(priKey->hex().c_str());
}