/**
 *  Copyright (C) 2022 FISCO BCOS.
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
 * @file org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj.cpp
 * @author: kyonGuo
 * @date 2023/11/29
 */

#include "org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_tx_v2.h"
#include "org_fisco_bcos_sdk_common.h"
#include <tuple>

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj
 * Method:    createTransactionData
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj_createTransactionData(
    JNIEnv* env, jclass, jstring j_group_id, jstring j_chain_id, jstring j_to, jstring j_input,
    jstring j_abi, jlong j_block_limit, jstring j_value, jstring j_gas_price, jlong j_gas_limit)
{
    CHECK_OBJECT_NOT_NULL(env, j_group_id, 0);
    CHECK_OBJECT_NOT_NULL(env, j_chain_id, 0);
    CHECK_OBJECT_NOT_NULL(env, j_input, 0);

    const char* group_id = GET_J_STRING_CONTENT(env, j_group_id);
    const char* chain_id = GET_J_STRING_CONTENT(env, j_chain_id);
    const char* to = GET_J_STRING_CONTENT_DEF(env, j_to, NULL);
    const char* input = GET_J_STRING_CONTENT(env, j_input);
    const char* abi = GET_J_STRING_CONTENT_DEF(env, j_abi, NULL);
    const char* value = GET_J_STRING_CONTENT_DEF(env, j_value, NULL);
    const char* gas_price = GET_J_STRING_CONTENT_DEF(env, j_gas_price, NULL);
    int64_t block_limit = (int64_t)j_block_limit;
    int64_t gas_limit = (int64_t)j_gas_limit;

    void* transaction_data = bcos_sdk_create_transaction_v2_data(
        group_id, chain_id, to, input, abi, block_limit, value, gas_price, gas_limit);

    env->ReleaseStringUTFChars(j_group_id, group_id);
    env->ReleaseStringUTFChars(j_chain_id, chain_id);
    env->ReleaseStringUTFChars(j_to, to);
    env->ReleaseStringUTFChars(j_input, input);
    env->ReleaseStringUTFChars(j_abi, abi);
    env->ReleaseStringUTFChars(j_value, value);
    env->ReleaseStringUTFChars(j_gas_price, gas_price);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    return reinterpret_cast<jlong>(transaction_data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj
 * Method:    createEIP1559TransactionData
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;JLjava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj_createEIP1559TransactionData(
    JNIEnv* env, jclass, jstring j_group_id, jstring j_chain_id, jstring j_to, jstring j_input,
    jstring j_abi, jlong j_block_limit, jstring j_value, jlong j_gas_limit,
    jstring j_max_fee_per_gas, jstring j_max_priority_fee_per_gas)
{
    CHECK_OBJECT_NOT_NULL(env, j_group_id, 0);
    CHECK_OBJECT_NOT_NULL(env, j_chain_id, 0);
    CHECK_OBJECT_NOT_NULL(env, j_input, 0);

    const char* group_id = GET_J_STRING_CONTENT(env, j_group_id);
    const char* chain_id = GET_J_STRING_CONTENT(env, j_chain_id);
    const char* to = GET_J_STRING_CONTENT_DEF(env, j_to, NULL);
    const char* input = GET_J_STRING_CONTENT(env, j_input);
    const char* abi = GET_J_STRING_CONTENT_DEF(env, j_abi, NULL);
    const char* value = GET_J_STRING_CONTENT_DEF(env, j_value, NULL);
    const char* max_fee_per_gas = GET_J_STRING_CONTENT_DEF(env, j_max_fee_per_gas, NULL);
    const char* max_priority_fee_per_gas =
        GET_J_STRING_CONTENT_DEF(env, j_max_priority_fee_per_gas, NULL);
    int64_t block_limit = (int64_t)j_block_limit;
    int64_t gas_limit = (int64_t)j_gas_limit;

    void* transaction_data = bcos_sdk_create_eip1559_transaction_data(group_id, chain_id, to, input,
        abi, block_limit, value, gas_limit, max_fee_per_gas, max_priority_fee_per_gas);

    env->ReleaseStringUTFChars(j_group_id, group_id);
    env->ReleaseStringUTFChars(j_chain_id, chain_id);
    env->ReleaseStringUTFChars(j_to, to);
    env->ReleaseStringUTFChars(j_input, input);
    env->ReleaseStringUTFChars(j_abi, abi);
    env->ReleaseStringUTFChars(j_value, value);
    env->ReleaseStringUTFChars(j_max_fee_per_gas, max_fee_per_gas);
    env->ReleaseStringUTFChars(j_max_priority_fee_per_gas, max_priority_fee_per_gas);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    return reinterpret_cast<jlong>(transaction_data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj
 * Method:    calcTransactionDataHashWithFullFields
 * Signature:
 * (ILorg/fisco/bcos/sdk/jni/utilities/tx/TransactionVersion;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj_calcTransactionDataHashWithFullFields(
    JNIEnv* env, jclass, jint j_crypto_type, jobject j_tx_version, jstring j_group_id,
    jstring j_chain_id, jstring j_to, jstring j_nonce, jstring j_input, jstring j_abi,
    jlong j_block_limit, jstring j_value, jstring j_gasPrice, jlong j_gas_limit,
    jstring j_max_fee_per_gas, jstring j_max_priority_fee_per_gas)
{
    CHECK_OBJECT_NOT_NULL(env, j_group_id, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_chain_id, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_input, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_nonce, NULL);

    const char* group_id = GET_J_STRING_CONTENT(env, j_group_id);
    const char* chain_id = GET_J_STRING_CONTENT(env, j_chain_id);
    const char* to = GET_J_STRING_CONTENT_DEF(env, j_to, NULL);
    const char* nonce = GET_J_STRING_CONTENT(env, j_nonce);
    const char* input = GET_J_STRING_CONTENT(env, j_input);
    const char* abi = GET_J_STRING_CONTENT_DEF(env, j_abi, NULL);
    const char* value = GET_J_STRING_CONTENT_DEF(env, j_value, NULL);
    const char* gas_price = GET_J_STRING_CONTENT_DEF(env, j_gasPrice, NULL);
    const char* max_fee_per_gas = GET_J_STRING_CONTENT_DEF(env, j_max_fee_per_gas, NULL);
    const char* max_priority_fee_per_gas =
        GET_J_STRING_CONTENT_DEF(env, j_max_priority_fee_per_gas, NULL);
    int crypto_type = (int)j_crypto_type;
    int64_t block_limit = (int64_t)j_block_limit;
    int64_t gas_limit = (int64_t)j_gas_limit;

    jclass tx_version = env->GetObjectClass(j_tx_version);
    jmethodID j_methodId = env->GetMethodID(tx_version, "getValue", "()I");
    int32_t ver = (int32_t)env->CallIntMethod(j_tx_version, j_methodId);

    const char* tx_hash = bcos_sdk_calc_transaction_data_hash_with_full_fields(crypto_type,
        (transaction_version)ver, group_id, chain_id, to, nonce, input, abi, block_limit, value,
        gas_price, gas_limit, max_fee_per_gas, max_priority_fee_per_gas);

    env->ReleaseStringUTFChars(j_group_id, group_id);
    env->ReleaseStringUTFChars(j_chain_id, chain_id);
    env->ReleaseStringUTFChars(j_to, to);
    env->ReleaseStringUTFChars(j_nonce, nonce);
    env->ReleaseStringUTFChars(j_input, input);
    env->ReleaseStringUTFChars(j_abi, abi);
    env->ReleaseStringUTFChars(j_value, value);
    env->ReleaseStringUTFChars(j_gasPrice, gas_price);
    env->ReleaseStringUTFChars(j_max_fee_per_gas, max_fee_per_gas);
    env->ReleaseStringUTFChars(j_max_priority_fee_per_gas, max_priority_fee_per_gas);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring j_transaction_data_hash = env->NewStringUTF(tx_hash);

    if (tx_hash)
    {
        free((void*)tx_hash);
        tx_hash = NULL;
    }

    return j_transaction_data_hash;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj
 * Method:    calcTransactionDataHashWithJson
 * Signature: (ILjava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj_calcTransactionDataHashWithJson(
    JNIEnv* env, jclass, jint j_crypto_type, jstring j_json)
{
    CHECK_OBJECT_NOT_NULL(env, j_json, NULL);
    const char* json = env->GetStringUTFChars(j_json, NULL);
    int crypto_type = (int)j_crypto_type;

    const char* tx_hash = bcos_sdk_calc_transaction_data_hash_with_json(crypto_type, json);

    env->ReleaseStringUTFChars(j_json, json);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring j_transaction_data_hash = env->NewStringUTF(tx_hash);

    if (tx_hash)
    {
        free((void*)tx_hash);
        tx_hash = NULL;
    }

    return j_transaction_data_hash;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj
 * Method:    createSignedTransactionWithSignature
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionVersion;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;ILjava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj_createSignedTransactionWithSignature(
    JNIEnv* env, jclass, jstring j_signature, jstring j_tx_hash, jobject j_tx_version,
    jstring j_group_id, jstring j_chain_id, jstring j_to, jstring j_nonce, jstring j_input,
    jstring j_abi, jlong j_block_limit, jstring j_value, jstring j_gas_price, jlong j_gas_limit,
    jstring j_max_fee_per_gas, jstring j_max_priority_fee_per_gas, jint j_attribute,
    jstring j_extra_data)
{
    CHECK_OBJECT_NOT_NULL(env, j_signature, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_tx_hash, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_group_id, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_chain_id, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_input, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_nonce, NULL);

    int64_t block_limit = (int64_t)j_block_limit;
    int64_t gas_limit = (int64_t)j_gas_limit;
    int32_t attribute = (int32_t)j_attribute;
    const char* signature = GET_J_STRING_CONTENT(env, j_signature);
    const char* tx_hash = GET_J_STRING_CONTENT(env, j_tx_hash);
    const char* group_id = GET_J_STRING_CONTENT(env, j_group_id);
    const char* chain_id = GET_J_STRING_CONTENT(env, j_chain_id);
    const char* to = GET_J_STRING_CONTENT_DEF(env, j_to, NULL);
    const char* nonce = GET_J_STRING_CONTENT(env, j_nonce);
    const char* input = GET_J_STRING_CONTENT(env, j_input);
    const char* abi = GET_J_STRING_CONTENT_DEF(env, j_abi, NULL);
    const char* value = GET_J_STRING_CONTENT_DEF(env, j_value, NULL);
    const char* gas_price = GET_J_STRING_CONTENT_DEF(env, j_gas_price, NULL);
    const char* max_fee_per_gas = GET_J_STRING_CONTENT_DEF(env, j_max_fee_per_gas, NULL);
    const char* max_priority_fee_per_gas =
        GET_J_STRING_CONTENT_DEF(env, j_max_priority_fee_per_gas, NULL);
    const char* extra_data = GET_J_STRING_CONTENT_DEF(env, j_extra_data, NULL);

    jclass tx_version = env->GetObjectClass(j_tx_version);
    jmethodID j_methodId = env->GetMethodID(tx_version, "getValue", "()I");
    int32_t ver = (int32_t)env->CallIntMethod(j_tx_version, j_methodId);

    const char* encodedTx = bcos_sdk_create_signed_transaction_with_signature(signature, tx_hash,
        (transaction_version)ver, group_id, chain_id, to, nonce, input, abi, block_limit, value,
        gas_price, gas_limit, max_fee_per_gas, max_priority_fee_per_gas, attribute, extra_data);

    env->ReleaseStringUTFChars(j_signature, signature);
    env->ReleaseStringUTFChars(j_tx_hash, tx_hash);
    env->ReleaseStringUTFChars(j_group_id, group_id);
    env->ReleaseStringUTFChars(j_chain_id, chain_id);
    env->ReleaseStringUTFChars(j_to, to);
    env->ReleaseStringUTFChars(j_nonce, nonce);
    env->ReleaseStringUTFChars(j_input, input);
    env->ReleaseStringUTFChars(j_abi, abi);
    env->ReleaseStringUTFChars(j_value, value);
    env->ReleaseStringUTFChars(j_gas_price, gas_price);
    env->ReleaseStringUTFChars(j_max_fee_per_gas, max_fee_per_gas);
    env->ReleaseStringUTFChars(j_max_priority_fee_per_gas, max_priority_fee_per_gas);
    env->ReleaseStringUTFChars(j_extra_data, extra_data);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring encoded_tx_hex = env->NewStringUTF(encodedTx);

    if (encodedTx)
    {
        free((void*)encodedTx);
        encodedTx = NULL;
    }

    return encoded_tx_hex;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj
 * Method:    createSignedTransactionWithFullFields
 * Signature:
 * (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;JILjava/lang/String;)Lorg/fisco/bcos/sdk/jni/utilities/tx/TxPair;
 */
JNIEXPORT jobject JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj_createSignedTransactionWithFullFields(
    JNIEnv* env, jclass, jlong j_key_pair, jstring j_group_id, jstring j_chain_id, jstring j_to,
    jstring j_input, jstring j_abi, jlong j_block_limit, jstring j_value, jstring j_gas_price,
    jlong j_gas_limit, jint j_attribute, jstring j_extra_data)
{
    CHECK_OBJECT_NOT_NULL(env, j_group_id, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_chain_id, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_input, NULL);

    void* keypair = reinterpret_cast<void*>(j_key_pair);
    int64_t block_limit = (int64_t)j_block_limit;
    int64_t gas_limit = (int64_t)j_gas_limit;
    int32_t attribute = (int32_t)j_attribute;
    const char* group_id = GET_J_STRING_CONTENT(env, j_group_id);
    const char* chain_id = GET_J_STRING_CONTENT(env, j_chain_id);
    const char* to = GET_J_STRING_CONTENT_DEF(env, j_to, NULL);
    const char* input = GET_J_STRING_CONTENT(env, j_input);
    const char* abi = GET_J_STRING_CONTENT_DEF(env, j_abi, NULL);
    const char* value = GET_J_STRING_CONTENT_DEF(env, j_value, NULL);
    const char* gas_price = GET_J_STRING_CONTENT_DEF(env, j_gas_price, NULL);
    const char* extra_data = GET_J_STRING_CONTENT_DEF(env, j_extra_data, NULL);

    char* tx_hash = NULL;
    char* signed_tx = NULL;
    bcos_sdk_create_signed_transaction_with_full_fields(keypair, group_id, chain_id, to, input, abi,
        block_limit, value, gas_price, gas_limit, attribute, extra_data, &tx_hash, &signed_tx);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring j_tx_hash = env->NewStringUTF(tx_hash);
    jstring j_signed_tx = env->NewStringUTF(signed_tx);

    jclass j_txpair = env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TxPair");
    if (j_txpair == NULL)
    {
        env->FatalError(
            "No such class, className: "
            "org/fisco/bcos/sdk/jni/utilities/tx/TxPair");
    }
    jmethodID txpair_mtd =
        env->GetMethodID(j_txpair, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (txpair_mtd == NULL)
    {
        env->FatalError("No such constructor in TxPair, constructor(String, String)");
    }

    jobject txpair = env->NewObject(j_txpair, txpair_mtd, j_tx_hash, j_signed_tx);

    env->ReleaseStringUTFChars(j_group_id, group_id);
    env->ReleaseStringUTFChars(j_chain_id, chain_id);
    env->ReleaseStringUTFChars(j_to, to);
    env->ReleaseStringUTFChars(j_input, input);
    env->ReleaseStringUTFChars(j_abi, abi);
    env->ReleaseStringUTFChars(j_value, value);
    env->ReleaseStringUTFChars(j_gas_price, gas_price);
    env->ReleaseStringUTFChars(j_extra_data, extra_data);

    env->DeleteLocalRef(j_tx_hash);
    env->DeleteLocalRef(j_signed_tx);

    if (tx_hash)
    {
        free((void*)tx_hash);
        tx_hash = NULL;
    }

    if (signed_tx)
    {
        free((void*)signed_tx);
        signed_tx = NULL;
    }

    return txpair;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj
 * Method:    createSignedEIP1559TransactionWithFullFields
 * Signature:
 * (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;JILjava/lang/String;)Lorg/fisco/bcos/sdk/jni/utilities/tx/TxPair;
 */
JNIEXPORT jobject JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderV2JniObj_createSignedEIP1559TransactionWithFullFields(
    JNIEnv* env, jclass, jlong j_key_pair, jstring j_group_id, jstring j_chain_id, jstring j_to,
    jstring j_input, jstring j_abi, jlong j_block_limit, jstring j_value, jstring j_max_fee_per_gas,
    jstring j_max_priority_fee_per_gas, jlong j_gas_limit, jint j_attribute, jstring j_extra_data)
{
    CHECK_OBJECT_NOT_NULL(env, j_group_id, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_chain_id, NULL);
    CHECK_OBJECT_NOT_NULL(env, j_input, NULL);

    void* keypair = reinterpret_cast<void*>(j_key_pair);
    int64_t block_limit = (int64_t)j_block_limit;
    int64_t gas_limit = (int64_t)j_gas_limit;
    int32_t attribute = (int32_t)j_attribute;
    const char* group_id = GET_J_STRING_CONTENT(env, j_group_id);
    const char* chain_id = GET_J_STRING_CONTENT(env, j_chain_id);
    const char* to = GET_J_STRING_CONTENT_DEF(env, j_to, NULL);
    const char* input = GET_J_STRING_CONTENT(env, j_input);
    const char* abi = GET_J_STRING_CONTENT_DEF(env, j_abi, NULL);
    const char* value = GET_J_STRING_CONTENT_DEF(env, j_value, NULL);
    const char* max_fee_per_gas = GET_J_STRING_CONTENT_DEF(env, j_max_fee_per_gas, NULL);
    const char* max_priority_fee_per_gas =
        GET_J_STRING_CONTENT_DEF(env, j_max_priority_fee_per_gas, NULL);
    const char* extra_data = GET_J_STRING_CONTENT_DEF(env, j_extra_data, NULL);

    char* tx_hash = NULL;
    char* signed_tx = NULL;
    bcos_sdk_create_signed_eip1559_transaction_with_full_fields(keypair, group_id, chain_id, to,
        input, abi, block_limit, value, gas_limit, max_fee_per_gas, max_priority_fee_per_gas,
        attribute, extra_data, &tx_hash, &signed_tx);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring j_tx_hash = env->NewStringUTF(tx_hash);
    jstring j_signed_tx = env->NewStringUTF(signed_tx);

    jclass j_txpair = env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TxPair");
    if (j_txpair == NULL)
    {
        env->FatalError(
            "No such class, className: "
            "org/fisco/bcos/sdk/jni/utilities/tx/TxPair");
    }
    jmethodID txpair_mtd =
        env->GetMethodID(j_txpair, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (txpair_mtd == NULL)
    {
        env->FatalError("No such constructor in TxPair, constructor(String, String)");
    }

    jobject txpair = env->NewObject(j_txpair, txpair_mtd, j_tx_hash, j_signed_tx);

    env->ReleaseStringUTFChars(j_group_id, group_id);
    env->ReleaseStringUTFChars(j_chain_id, chain_id);
    env->ReleaseStringUTFChars(j_to, to);
    env->ReleaseStringUTFChars(j_input, input);
    env->ReleaseStringUTFChars(j_abi, abi);
    env->ReleaseStringUTFChars(j_value, value);
    env->ReleaseStringUTFChars(j_max_fee_per_gas, max_fee_per_gas);
    env->ReleaseStringUTFChars(j_max_priority_fee_per_gas, max_priority_fee_per_gas);
    env->ReleaseStringUTFChars(j_extra_data, extra_data);

    env->DeleteLocalRef(j_tx_hash);
    env->DeleteLocalRef(j_signed_tx);

    if (tx_hash)
    {
        free((void*)tx_hash);
        tx_hash = NULL;
    }

    if (signed_tx)
    {
        free((void*)signed_tx);
        signed_tx = NULL;
    }

    return txpair;
}
