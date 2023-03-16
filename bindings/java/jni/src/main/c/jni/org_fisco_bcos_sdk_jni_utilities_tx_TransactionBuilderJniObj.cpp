#include "org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_tx.h"
#include "org_fisco_bcos_sdk_exception.h"
#include <tuple>

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj
 * Method:    createTransactionData
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)J
 */
JNIEXPORT jlong JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj_createTransactionData(JNIEnv* env,
    jclass, jstring jgroup_id, jstring jchain_id, jstring jto, jstring jdata, jstring jabi,
    jlong jblock_limit)
{
    // group id
    const char* group_id = env->GetStringUTFChars(jgroup_id, NULL);
    // chain id
    const char* chain_id = env->GetStringUTFChars(jchain_id, NULL);
    // code
    const char* to = env->GetStringUTFChars(jto, NULL);
    // data
    const char* data = env->GetStringUTFChars(jdata, NULL);
    // abi
    const char* abi = env->GetStringUTFChars(jabi, NULL);
    // block limit
    int64_t block_limit = (int64_t)jblock_limit;

    void* transaction_data =
        bcos_sdk_create_transaction_data(group_id, chain_id, to, data, abi, block_limit);

    env->ReleaseStringUTFChars(jgroup_id, group_id);
    env->ReleaseStringUTFChars(jchain_id, chain_id);
    env->ReleaseStringUTFChars(jto, to);
    env->ReleaseStringUTFChars(jdata, data);
    env->ReleaseStringUTFChars(jabi, abi);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    return reinterpret_cast<jlong>(transaction_data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_receipt_ReceiptBuilderJniObj
 * Method:    createReceiptDataWithJson
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj_createTransactionDataWithJson(
    JNIEnv* env, jclass, jstring jjson)
{
    const char* json = env->GetStringUTFChars(jjson, NULL);
    void* transaction_data = bcos_sdk_create_transaction_data_with_json(json);

    env->ReleaseStringUTFChars(jjson, json);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    return reinterpret_cast<jlong>(transaction_data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj
 * Method:    destroyTransactionData
 * Signature: (J)V
 */
JNIEXPORT void JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj_destroyTransactionData(
    JNIEnv*, jclass, jlong jtransaction_data)
{
    void* transaction_data = reinterpret_cast<void*>(jtransaction_data);
    bcos_sdk_destroy_transaction_data(transaction_data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj
 * Method:    encodeTransactionData
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj_encodeTransactionData(
    JNIEnv* env, jclass, jlong jtransaction_data)
{
    std::ignore = env;
    std::ignore = jtransaction_data;

    void* transaction_data = reinterpret_cast<void*>(jtransaction_data);
    const char* encoded_transaction_data = bcos_sdk_encode_transaction_data(transaction_data);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jencoded_transaction_data = env->NewStringUTF(encoded_transaction_data);

    if (encoded_transaction_data)
    {
        free((void*)encoded_transaction_data);
        encoded_transaction_data = NULL;
    }

    return jencoded_transaction_data;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj
 * Method:    decodeTransactionDataToJsonObj
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj_decodeTransactionDataToJsonObj(
    JNIEnv* env, jclass, jstring jtransaction_bytes)
{
    const char* transaction_data = env->GetStringUTFChars(jtransaction_bytes, NULL);
    const char* transaction_data_json = bcos_sdk_decode_transaction_data(transaction_data);

    env->ReleaseStringUTFChars(jtransaction_bytes, transaction_data);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jtransaction_data_json = env->NewStringUTF(transaction_data_json);
    if (transaction_data_json)
    {
        free((void*)transaction_data_json);
        transaction_data_json = NULL;
    }
    return jtransaction_data_json;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj
 * Method:    calcTransactionDataHash
 * Signature: (IJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj_calcTransactionDataHash(
    JNIEnv* env, jclass, jint jcrypto_type, jlong jtransaction_data)
{
    void* transaction_data = reinterpret_cast<void*>(jtransaction_data);
    int crypto_type = jcrypto_type;
    const char* transaction_data_hash =
        bcos_sdk_calc_transaction_data_hash(crypto_type, transaction_data);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jtransaction_data_hash = env->NewStringUTF(transaction_data_hash);

    if (transaction_data_hash)
    {
        free((void*)transaction_data_hash);
        transaction_data_hash = NULL;
    }

    return jtransaction_data_hash;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj
 * Method:    signTransactionDataHash
 * Signature: (JLjava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj_signTransactionDataHash(
    JNIEnv* env, jclass, jlong jkeypair, jstring jtransaction_data_hash)
{
    //
    void* keypair = reinterpret_cast<void*>(jkeypair);
    // transaction_data_hash
    const char* transaction_data_hash = env->GetStringUTFChars(jtransaction_data_hash, NULL);

    const char* signed_data = bcos_sdk_sign_transaction_data_hash(keypair, transaction_data_hash);

    env->ReleaseStringUTFChars(jtransaction_data_hash, transaction_data_hash);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jsigned_data = env->NewStringUTF(signed_data);

    if (signed_data)
    {
        free((void*)signed_data);
        signed_data = NULL;
    }

    return jsigned_data;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj
 * Method:    createSignedTransaction
 * Signature: (JLjava/lang/String;Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj_createSignedTransaction__JLjava_lang_String_2Ljava_lang_String_2I(
    JNIEnv* env, jclass, jlong jtransaction_data, jstring jtransaction_data_signed_data,
    jstring jtransaction_data_hash, jint jattr)
{
    void* transaction_data = reinterpret_cast<void*>(jtransaction_data);

    const char* transaction_data_signed_data =
        env->GetStringUTFChars(jtransaction_data_signed_data, NULL);
    const char* transaction_data_hash = env->GetStringUTFChars(jtransaction_data_hash, NULL);
    int attr = jattr;

    const char* signed_tx = bcos_sdk_create_signed_transaction_with_signed_data(
        transaction_data, transaction_data_signed_data, transaction_data_hash, attr);

    // release resoure
    env->ReleaseStringUTFChars(jtransaction_data_signed_data, transaction_data_signed_data);
    env->ReleaseStringUTFChars(jtransaction_data_hash, transaction_data_hash);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jsigned_tx = env->NewStringUTF(signed_tx);

    if (signed_tx)
    {
        free((void*)signed_tx);
        signed_tx = NULL;
    }

    return jsigned_tx;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj
 * Method:    createSignedTransaction
 * Signature: (JLjava/lang/String;Ljava/lang/String;ILjava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj_createSignedTransaction__JLjava_lang_String_2Ljava_lang_String_2ILjava_lang_String_2(
    JNIEnv* env, jclass, jlong jtransaction_data, jstring jtransaction_data_signed_data,
    jstring jtransaction_data_hash, jint jattr, jstring jextra_data)
{
    void* transaction_data = reinterpret_cast<void*>(jtransaction_data);

    const char* transaction_data_signed_data =
        env->GetStringUTFChars(jtransaction_data_signed_data, NULL);
    const char* transaction_data_hash = env->GetStringUTFChars(jtransaction_data_hash, NULL);
    int attr = jattr;
    const char* extra_data = env->GetStringUTFChars(jextra_data, NULL);

    const char* signed_tx = bcos_sdk_create_signed_transaction_with_signed_data_ver_extra_data(
        transaction_data, transaction_data_signed_data, transaction_data_hash, attr, extra_data);

    // release resoure
    env->ReleaseStringUTFChars(jtransaction_data_signed_data, transaction_data_signed_data);
    env->ReleaseStringUTFChars(jtransaction_data_hash, transaction_data_hash);
    env->ReleaseStringUTFChars(jextra_data, extra_data);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jsigned_tx = env->NewStringUTF(signed_tx);

    if (signed_tx)
    {
        free((void*)signed_tx);
        signed_tx = NULL;
    }

    return jsigned_tx;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj
 * Method:    createSignedTransaction
 * Signature:
 * (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JI)Lorg/fisco/bcos/sdk/jni/utilities/tx/TxPair;
 */
JNIEXPORT jobject JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj_createSignedTransaction__JLjava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2JI(
    JNIEnv* env, jclass, jlong jkeypair, jstring jgroup_id, jstring jchain_id, jstring jto,
    jstring jdata, jstring jabi, jlong jblock_limit, jint jattr)
{
    // keypair
    void* keypair = reinterpret_cast<void*>(jkeypair);
    // group id
    const char* group_id = env->GetStringUTFChars(jgroup_id, NULL);
    // chain id
    const char* chain_id = env->GetStringUTFChars(jchain_id, NULL);
    // to
    const char* to = env->GetStringUTFChars(jto, NULL);
    // data
    const char* data = env->GetStringUTFChars(jdata, NULL);
    // abi
    const char* abi = env->GetStringUTFChars(jabi, NULL);
    // block limit
    int64_t block_limit = (int64_t)jblock_limit;
    // attr
    int attr = (int64_t)jattr;

    char* tx_hash = NULL;
    char* signed_tx = NULL;
    bcos_sdk_create_signed_transaction(
        keypair, group_id, chain_id, to, data, abi, block_limit, attr, &tx_hash, &signed_tx);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jtx_hash = env->NewStringUTF(tx_hash);
    jstring jsigned_tx = env->NewStringUTF(signed_tx);

    jclass jtxpair = env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TxPair");
    if (jtxpair == NULL)
    {
        env->FatalError(
            "No such class, className: "
            "org/fisco/bcos/sdk/jni/utilities/tx/TxPair");
    }

    jmethodID txpair_mtd =
        env->GetMethodID(jtxpair, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (txpair_mtd == NULL)
    {
        env->FatalError("No such constructor in TxPair, constructor(String, String)");
    }

    jobject txpair = env->NewObject(jtxpair, txpair_mtd, jtx_hash, jsigned_tx);

    // release resoure
    env->ReleaseStringUTFChars(jgroup_id, group_id);
    env->ReleaseStringUTFChars(jchain_id, chain_id);
    env->ReleaseStringUTFChars(jdata, data);
    env->ReleaseStringUTFChars(jto, to);
    env->ReleaseStringUTFChars(jabi, abi);

    env->DeleteLocalRef(jtx_hash);
    env->DeleteLocalRef(jsigned_tx);

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
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj
 * Method:    createSignedTransaction
 * Signature:
 * (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;JILjava/lang/String;)Lorg/fisco/bcos/sdk/jni/utilities/tx/TxPair;
 */
JNIEXPORT jobject JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionBuilderJniObj_createSignedTransaction__JLjava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2JILjava_lang_String_2(
    JNIEnv* env, jclass, jlong jkeypair, jstring jgroup_id, jstring jchain_id, jstring jto,
    jstring jdata, jstring jabi, jlong jblock_limit, jint jattr, jstring jextra_data)
{
    // keypair
    void* keypair = reinterpret_cast<void*>(jkeypair);
    // group id
    const char* group_id = env->GetStringUTFChars(jgroup_id, NULL);
    // chain id
    const char* chain_id = env->GetStringUTFChars(jchain_id, NULL);
    // to
    const char* to = env->GetStringUTFChars(jto, NULL);
    // data
    const char* data = env->GetStringUTFChars(jdata, NULL);
    // abi
    const char* abi = env->GetStringUTFChars(jabi, NULL);
    // block limit
    int64_t block_limit = (int64_t)jblock_limit;
    // attr
    int attr = (int64_t)jattr;
    // extra data
    const char* extra_data = env->GetStringUTFChars(jextra_data, NULL);

    char* tx_hash = NULL;
    char* signed_tx = NULL;
    bcos_sdk_create_signed_transaction_ver_extra_data(keypair, group_id, chain_id, to, data, abi,
        block_limit, attr, extra_data, &tx_hash, &signed_tx);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jtx_hash = env->NewStringUTF(tx_hash);
    jstring jsigned_tx = env->NewStringUTF(signed_tx);

    jclass jtxpair = env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TxPair");
    if (jtxpair == NULL)
    {
        env->FatalError(
            "No such class, className: "
            "org/fisco/bcos/sdk/jni/utilities/tx/TxPair");
    }

    jmethodID txpair_mtd =
        env->GetMethodID(jtxpair, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (txpair_mtd == NULL)
    {
        env->FatalError("No such constructor in TxPair, constructor(String, String)");
    }

    jobject txpair = env->NewObject(jtxpair, txpair_mtd, jtx_hash, jsigned_tx);

    // release resoure
    env->ReleaseStringUTFChars(jgroup_id, group_id);
    env->ReleaseStringUTFChars(jchain_id, chain_id);
    env->ReleaseStringUTFChars(jdata, data);
    env->ReleaseStringUTFChars(jto, to);
    env->ReleaseStringUTFChars(jabi, abi);
    env->ReleaseStringUTFChars(jextra_data, extra_data);

    env->DeleteLocalRef(jtx_hash);
    env->DeleteLocalRef(jsigned_tx);

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