/**
 *  Copyright (C) 2024 FISCO BCOS.
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
 * @file Common.cpp
 * @author: kyonGuo
 * @date 2024/11/21
 */

#include "Common.h"

jobject convert_to_jni_keypair_jobject(JNIEnv* env, const struct bcos_key_pair* key_pair)
{
    if (key_pair == nullptr)
    {
        THROW_JNI_EXCEPTION(
            env, "exception in convert_to_jni_keypair_jobject, key_pair is nullptr");
        return nullptr;
    }
    jclass key_pair_class = env->FindClass("org/fisco/bcos/sdk/jni/utilities/keypair/JniKeyPair");
    if (key_pair_class == nullptr)
    {
        env->FatalError(
            "exception in convert_to_jni_keypair_jobject, can not find class in "
            "org/fisco/bcos/sdk/jni/utilities/keypair/JniKeyPair");
        return nullptr;
    }
    jmethodID constructor = env->GetMethodID(key_pair_class, "<init>", "()V");
    if (constructor == nullptr)
    {
        env->FatalError(
            "exception in convert_to_jni_keypair_jobject, can not find constructor in "
            "org/fisco/bcos/sdk/jni/utilities/keypair/JniKeyPair");
        return nullptr;
    }
    jobject j_keypair_obj = env->NewObject(key_pair_class, constructor);
    if (j_keypair_obj == nullptr)
    {
        env->FatalError("exception in convert_to_jni_keypair_jobject, failed to create new object");
        return nullptr;
    }

    if (key_pair->pri != nullptr)
    {
        jfieldID private_key_field = env->GetFieldID(key_pair_class, "privateKey", "[B");
        jbyteArray private_key_array = env->NewByteArray(static_cast<jsize>(key_pair->pri->length));
        if (private_key_array == nullptr)
        {
            env->DeleteLocalRef(j_keypair_obj);
            THROW_JNI_EXCEPTION(env,
                "exception in convert_to_jni_keypair_jobject, failed to create private key array");
            return nullptr;
        }
        env->SetByteArrayRegion(private_key_array, 0, static_cast<jsize>(key_pair->pri->length),
            reinterpret_cast<jbyte*>(key_pair->pri->buffer));
        env->SetObjectField(j_keypair_obj, private_key_field, private_key_array);
        env->DeleteLocalRef(private_key_array);
    }
    if (key_pair->pub != nullptr)
    {
        jfieldID public_key_field = env->GetFieldID(key_pair_class, "publicKey", "[B");
        jbyteArray public_key_array = env->NewByteArray(static_cast<jsize>(key_pair->pub->length));
        if (public_key_array == nullptr)
        {
            env->DeleteLocalRef(j_keypair_obj);
            THROW_JNI_EXCEPTION(env,
                "exception in convert_to_jni_keypair_jobject, failed to create public key array");
            return nullptr;
        }
        env->SetByteArrayRegion(public_key_array, 0, static_cast<jsize>(key_pair->pub->length),
            reinterpret_cast<jbyte*>(key_pair->pub->buffer));
        env->SetObjectField(j_keypair_obj, public_key_field, public_key_array);
        env->DeleteLocalRef(public_key_array);
    }
    jfieldID type = env->GetFieldID(key_pair_class, "cryptoType", "I");
    env->SetIntField(j_keypair_obj, type, key_pair->type);

    return j_keypair_obj;
}

void convert_to_keypair(JNIEnv* env, jobject j_keypair_obj, struct bcos_key_pair* key_pair)
{
    if (j_keypair_obj == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_keypair, j_keypair_obj is nullptr");
        return;
    }
    jclass key_pair_class = env->GetObjectClass(j_keypair_obj);

    jfieldID private_key_field = env->GetFieldID(key_pair_class, "privateKey", "[B");
    jfieldID public_key_field = env->GetFieldID(key_pair_class, "publicKey", "[B");
    jfieldID type = env->GetFieldID(key_pair_class, "cryptoType", "I");
    // private key
    auto private_key_array = (jbyteArray)env->GetObjectField(j_keypair_obj, private_key_field);
    if (private_key_array == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "exception in private key field.");
        return;
    }
    auto* pri_key_bytes = (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
    if (pri_key_bytes == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_keypair, malloc failed");
        return;
    }

    jbyte* pri_key_data = env->GetByteArrayElements(private_key_array, nullptr);
    jsize key_len = env->GetArrayLength(private_key_array);
    pri_key_bytes->buffer = (uint8_t*)malloc(key_len * sizeof(uint8_t));
    if (pri_key_bytes->buffer == nullptr)
    {
        free(pri_key_bytes);
        THROW_JNI_EXCEPTION(env, "exception in convert_to_keypair, malloc failed");
        return;
    }

    memmove(pri_key_bytes->buffer, pri_key_data, key_len);
    pri_key_bytes->length = key_len;

    // public key
    auto public_key_array = (jbyteArray)env->GetObjectField(j_keypair_obj, public_key_field);
    if (public_key_array == nullptr)
    {
        free(pri_key_bytes->buffer);
        free(pri_key_bytes);
        THROW_JNI_EXCEPTION(env, "exception in public key field.");
        return;
    }
    auto* pub_key_bytes = (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
    if (pub_key_bytes == nullptr)
    {
        free(pri_key_bytes->buffer);
        free(pri_key_bytes);
        THROW_JNI_EXCEPTION(env, "exception in convert_to_keypair, malloc failed");
        return;
    }
    jbyte* pub_key_data = env->GetByteArrayElements(public_key_array, nullptr);
    jsize pub_key_len = env->GetArrayLength(public_key_array);
    pub_key_bytes->buffer = (uint8_t*)malloc(pub_key_len * sizeof(uint8_t));
    if (pub_key_bytes->buffer == nullptr)
    {
        free(pri_key_bytes->buffer);
        free(pri_key_bytes);
        free(pub_key_bytes);
        THROW_JNI_EXCEPTION(env, "exception in convert_to_keypair, malloc failed");
        return;
    }

    memmove(pub_key_bytes->buffer, pub_key_data, pub_key_len);
    pub_key_bytes->length = pub_key_len;

    jint crypto_type = env->GetIntField(j_keypair_obj, type);

    key_pair->pri = pri_key_bytes;
    key_pair->pub = pub_key_bytes;
    key_pair->type = crypto_type;
}
