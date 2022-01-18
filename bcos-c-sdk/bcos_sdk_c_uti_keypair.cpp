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
#include <bcos-cpp-sdk/utilities/crypto/Common.h>
#include <bcos-cpp-sdk/utilities/crypto/CryptoSuite.h>
#include <bcos-cpp-sdk/utilities/crypto/KeyPair.h>
#include <bcos-cpp-sdk/utilities/crypto/KeyPairBuilder.h>
#include <exception>

using namespace bcos;
using namespace bcos::cppsdk;
using namespace bcos::cppsdk::utilities;

void* bcos_sdk_create_keypair(int crypto_type)
{
    bcos_sdk_clear_last_error();
    try
    {
        auto keyPairBuilder = std::make_shared<KeyPairBuilder>();
        auto keyPair = keyPairBuilder->genKeyPair(crypto_type == BCOS_C_SDK_ECDSA_TYPE ?
                                                      CryptoSuiteType::ECDSA_TYPE :
                                                      CryptoSuiteType::SM_TYPE);
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

void* bcos_sdk_load_keypair(const char* pem)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(pem, NULL);
    try
    {
        /*
        ECDSA_TYPE  1,
        SM_TYPE  2
        */
        auto keyPairBuilder = std::make_shared<KeyPairBuilder>();
        auto keyPair = keyPairBuilder->loadKeyPair(std::string(pem));
        return keyPair.release();
    }
    catch (const std::exception& e)
    {
        std::string errorMsg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, errorMsg.c_str());

        BCOS_LOG(ERROR) << LOG_BADGE("bcos_sdk_load_keypair") << LOG_KV("pem file", pem)
                        << LOG_KV("errorMsg", errorMsg);
        return NULL;
    }
}

void bcos_sdk_destroy_keypair(void* key_pair)
{
    bcos_sdk_clear_last_error();
    if (key_pair)
    {
        delete (KeyPair*)key_pair;
        key_pair = NULL;
    }
}

const char* bcos_sdk_get_keypair_address(void* key_pair)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, NULL);

    auto cryptoSuite = std::make_shared<CryptoSuite>(*((KeyPair*)key_pair));
    auto address = cryptoSuite->address();
    return strdup(address.hexPrefixed().c_str());
}

const char* bcos_sdk_get_keypair_public_key(void* key_pair)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, NULL);

    auto priKey = ((KeyPair*)key_pair)->hexPublicKey();
    return strdup(priKey.c_str());
}

const char* bcos_sdk_get_keypair_private_key(void* key_pair)
{
    bcos_sdk_clear_last_error();
    BCOS_SDK_C_PARAMS_VERIFICATION(key_pair, NULL);

    auto priKey = ((KeyPair*)key_pair)->hexPrivateKey();
    return strdup(priKey.c_str());
}