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
 * @file Common.h
 * @author: kyonGuo
 * @date 2024/11/21
 */

#pragma once
#include "org_fisco_bcos_sdk_common.h"
#include <jni.h>


// convert c struct to java object
jobject convert_to_jni_keypair_jobject(JNIEnv* env, const struct bcos_key_pair* key_pair);

// convert java object to c struct
void convert_to_keypair(JNIEnv* env, jobject j_keypair_obj, struct bcos_key_pair* key_pair);
