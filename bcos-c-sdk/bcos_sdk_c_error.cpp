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
 * @file bcos_sdk_c_error.cpp
 * @author: octopus
 * @date 2021-12-15
 */

#include "bcos_sdk_c_error.h"

#include <mutex>
#include <string>
#include <thread>

// record c error
thread_local int g_global_error = 0;
thread_local std::string g_global_msg;

/**
 * @brief the last sync operation success or not
 *  Note: thread safe operation
 *
 * @return int
 */
int bcos_sdk_is_last_opr_success()
{
    return g_global_error == BCOS_SDK_C_SUCCESS;
}

/**
 * @brief gets status of the most recent sync operation
 *
 * @return int
 */
int bcos_sdk_get_last_error()
{
    return g_global_error;
}

/**
 * @brief gets error message of the most recent sync operation, effect if bcos_sdk_get_last_error
 * return not zero
 *
 * @return const char*
 */
const char* bcos_sdk_get_last_error_msg()
{
    return g_global_msg.c_str();
}

void bcos_sdk_clear_last_error()
{
    g_global_error = 0;
    g_global_msg.clear();
}

void bcos_sdk_set_last_error_msg(int error, const char* msg)
{
    g_global_error = error;
    g_global_msg = std::string(msg);
}