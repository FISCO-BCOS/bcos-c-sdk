#include "org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_receipt.h"
#include "org_fisco_bcos_sdk_exception.h"
#include <tuple>

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj
 * Method:    createReceiptData
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj_createReceiptData(JNIEnv* env,
    jclass, jstring jgas_used, jstring jcontract_address, jstring jout_put,
    jlong jblock_number)
{
    // group id
    const char* gas_used = env->GetStringUTFChars(jgas_used, NULL);
    // chain id
    const char* contract_address = env->GetStringUTFChars(jcontract_address, NULL);
    // code
    const char* out_put = env->GetStringUTFChars(jout_put, NULL);
    // block limit
    int64_t block_number = (int64_t)jblock_number;

    void* receipt_data =
        bcos_sdk_create_receipt_data(gas_used, contract_address, out_put, block_number);

    env->ReleaseStringUTFChars(jgas_used, gas_used);
    env->ReleaseStringUTFChars(jcontract_address, contract_address);
    env->ReleaseStringUTFChars(jout_put, out_put);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    return reinterpret_cast<jlong>(receipt_data);
}


/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj
 * Method:    createReceiptDataWithJson
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj_createReceiptDataWithJson
  (JNIEnv *env, jclass, jstring jjson)
{
    const char* json = env->GetStringUTFChars(jjson, NULL);
    void* receipt_data = bcos_sdk_create_receipt_data_with_json(json);

    env->ReleaseStringUTFChars(jjson, json);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    return reinterpret_cast<jlong>(receipt_data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj
 * Method:    destroyReceiptData
 * Signature: (J)V
 */
JNIEXPORT void JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj_destroyReceiptData(
    JNIEnv*, jclass, jlong jreceipt_data)
{
    void* receipt_data = reinterpret_cast<void*>(jreceipt_data);
    bcos_sdk_destroy_receipt_data(receipt_data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj
 * Method:    encodeReceiptData
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj_encodeReceiptData(
    JNIEnv* env, jclass, jlong jreceipt_data)
{
    void* receipt_data = reinterpret_cast<void*>(jreceipt_data);
    const char* encoded_receipt_data = bcos_sdk_encode_receipt_data(receipt_data);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jencoded_receipt_data = env->NewStringUTF(encoded_receipt_data);

    if (encoded_receipt_data)
    {
        free((void*)encoded_receipt_data);
        encoded_receipt_data = NULL;
    }

    return jencoded_receipt_data;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj
 * Method:    decodeReceiptDataToJsonObj
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj_decodeReceiptDataToJsonObj
  (JNIEnv* env, jclass, jstring jreceipt_bytes)
{
    const char* receipt_data = env->GetStringUTFChars(jreceipt_bytes, NULL);
    const char* receipt_data_json = bcos_sdk_decode_receipt_data(receipt_data);

    env->ReleaseStringUTFChars(jreceipt_bytes, receipt_data);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jreceipt_data_json = env->NewStringUTF(receipt_data_json);
    if (receipt_data_json)
    {
        free((void*)receipt_data_json);
        receipt_data_json = NULL;
    }
    return jreceipt_data_json;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj
 * Method:    calcReceiptDataHash
 * Signature: (IJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj_calcReceiptDataHash(
    JNIEnv* env, jclass, jint jcrypto_type, jlong jreceipt_data)
{
    void* receipt_data = reinterpret_cast<void*>(jreceipt_data);
    int crypto_type = jcrypto_type;
    const char* receipt_data_hash =
        bcos_sdk_calc_receipt_data_hash(crypto_type, receipt_data);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jreceipt_data_hash = env->NewStringUTF(receipt_data_hash);

    if (receipt_data_hash)
    {
        free((void*)receipt_data_hash);
        receipt_data_hash = NULL;
    }

    return jreceipt_data_hash;
}