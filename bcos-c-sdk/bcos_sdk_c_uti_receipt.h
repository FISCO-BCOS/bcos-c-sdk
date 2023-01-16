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
 * @file bcos_sdk_c_uti_receipt.h
 * @author: kyonGuo
 * @date 2022/12/7
 */

#ifndef BCOS_C_SDK_BCOS_SDK_C_UTI_RECEIPT_H
#define BCOS_C_SDK_BCOS_SDK_C_UTI_RECEIPT_H

#include "bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif

void* bcos_sdk_create_receipt_data(
    const char* gas_used, const char* contract_address, const char* out_put, int64_t block_number);

/**
 * @brief Create a Transaction Data object with json
 * @param json
 *              version: number
 *              gasUsed: string
 *              contractAddress: string
 *              status: number
 *              output: hex string
 *              logEntries: array<LogEntry>
 *              blockNumber: number
 *              logEntry:
 *                       address: string
 *                       topic: array<hex string>
 *                       data: hex string
 * @return bcostars::TransactionDataUniquePtr
 */
void* bcos_sdk_create_receipt_data_with_json(const char* json);

void bcos_sdk_destroy_receipt_data(void* transaction_data);

const char* bcos_sdk_calc_receipt_data_hash(int crypto_type, void* _receiptData);

const char* bcos_sdk_encode_receipt_data(void* receipt_data);

const char* bcos_sdk_decode_receipt_data(const char* receipt_bytes);


#ifdef __cplusplus
}
#endif
#endif  // BCOS_C_SDK_BCOS_SDK_C_UTI_RECEIPT_H
