#include "org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_tx_struct.h"
#include "org_fisco_bcos_sdk_common.h"
#include "org_fisco_bcos_sdk_exception.h"

struct bcos_sdk_c_bytes* convert_to_bytes_struct(JNIEnv* env, jobject bytesObj)
{
    try
    {
        if (bytesObj == NULL)
        {
            return NULL;
        }

        struct bcos_sdk_c_bytes* bytes =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        jclass bytesClass = env->GetObjectClass(bytesObj);
        jfieldID bufferField = env->GetFieldID(bytesClass, "buffer", "[B");
        jfieldID lengthField = env->GetFieldID(bytesClass, "length", "I");

        jbyteArray bufferArray = (jbyteArray)env->GetObjectField(bytesObj, bufferField);
        jbyte* bufferData = env->GetByteArrayElements(bufferArray, nullptr);
        jsize bufferLength = env->GetArrayLength(bufferArray);
        bytes->length = (uint32_t)env->GetIntField(bytesObj, lengthField);
        bytes->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
        memcpy(bytes->buffer, bufferData, bufferLength);

        env->ReleaseByteArrayElements(bufferArray, bufferData, 0);

        return bytes;
    }
    catch (...)
    {
        return NULL;
    }
}

jobject convert_to_bytes_jobject(JNIEnv* env, const struct bcos_sdk_c_bytes* bytes_struct)
{
    try
    {
        if (bytes_struct == NULL)
        {
            return NULL;
        }

        jclass txBytesClass =
            env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes");
        if (txBytesClass == NULL)
        {
            env->FatalError(
                "No such class, className: "
                "org/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes");
        }
        jmethodID txBytesMtd = env->GetMethodID(txBytesClass, "<init>", "()V");
        if (txBytesMtd == NULL)
        {
            env->FatalError("No such constructor in TransactionBytes, constructor()");
        }
        jfieldID bufferField = env->GetFieldID(txBytesClass, "buffer", "[B");
        jfieldID lengthField = env->GetFieldID(txBytesClass, "length", "I");

        jobject jTxBytesObj = env->NewObject(txBytesClass, txBytesMtd);
        // buffer
        jbyteArray bufferArray = env->NewByteArray(bytes_struct->length);
        env->SetByteArrayRegion(
            bufferArray, 0, bytes_struct->length, reinterpret_cast<jbyte*>(bytes_struct->buffer));
        env->SetObjectField(jTxBytesObj, bufferField, bufferArray);
        // length
        env->SetIntField(jTxBytesObj, lengthField, bytes_struct->length);

        env->DeleteLocalRef(bufferArray);

        return jTxBytesObj;
    }
    catch (...)
    {
        return NULL;
    }
}

struct bcos_sdk_c_transaction_data* convert_to_tx_data_struct(
    JNIEnv* env, jobject transactionObject)
{
    try
    {
        if (transactionObject == NULL)
        {
            THROW_JNI_EXCEPTION(
                env, "exception in convert_to_tx_data_struct, transactionObject is nullptr");
        }

        bcos_sdk_c_transaction_data* tx_data_struct =
            (struct bcos_sdk_c_transaction_data*)malloc(sizeof(struct bcos_sdk_c_transaction_data));
        jclass txDataClass = env->GetObjectClass(transactionObject);

        // version
        jfieldID versionField = env->GetFieldID(txDataClass, "version", "I");
        tx_data_struct->version = env->GetIntField(transactionObject, versionField);

        // blockLimit
        jfieldID blockLimitField = env->GetFieldID(txDataClass, "blockLimit", "J");
        tx_data_struct->block_limit = env->GetLongField(transactionObject, blockLimitField);

        // chainId
        jfieldID chainIdField = env->GetFieldID(txDataClass, "chainId", "Ljava/lang/String;");
        jstring chainIdString = (jstring)env->GetObjectField(transactionObject, chainIdField);
        const char* chainIdValue = env->GetStringUTFChars(chainIdString, nullptr);
        tx_data_struct->chain_id = strdup(chainIdValue);

        // groupId
        jfieldID groupIdField = env->GetFieldID(txDataClass, "groupId", "Ljava/lang/String;");
        jstring groupIdString = (jstring)env->GetObjectField(transactionObject, groupIdField);
        const char* groupIdValue = env->GetStringUTFChars(groupIdString, nullptr);
        tx_data_struct->group_id = strdup(groupIdValue);

        // nonce
        jfieldID nonceField = env->GetFieldID(txDataClass, "nonce", "Ljava/lang/String;");
        jstring nonceString = (jstring)env->GetObjectField(transactionObject, nonceField);
        const char* nonceValue = env->GetStringUTFChars(nonceString, nullptr);
        tx_data_struct->nonce = strdup(nonceValue);

        // to
        jfieldID toField = env->GetFieldID(txDataClass, "to", "Ljava/lang/String;");
        jstring toString = (jstring)env->GetObjectField(transactionObject, toField);
        const char* toValue = env->GetStringUTFChars(toString, nullptr);
        tx_data_struct->to = strdup(toValue);

        // abi
        jfieldID abiField = env->GetFieldID(txDataClass, "abi", "Ljava/lang/String;");
        jstring abiString = (jstring)env->GetObjectField(transactionObject, abiField);
        const char* abiValue = env->GetStringUTFChars(abiString, nullptr);
        tx_data_struct->abi = strdup(abiValue);

        // input
        jfieldID inputField = env->GetFieldID(
            txDataClass, "input", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        jobject inputObject = env->GetObjectField(transactionObject, inputField);
        struct bcos_sdk_c_bytes* inputStruct = convert_to_bytes_struct(env, inputObject);
        if (inputStruct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_struct");
        }
        tx_data_struct->input = inputStruct;

        // release source
        env->ReleaseStringUTFChars(chainIdString, chainIdValue);
        env->ReleaseStringUTFChars(groupIdString, groupIdValue);
        env->ReleaseStringUTFChars(nonceString, nonceValue);
        env->ReleaseStringUTFChars(toString, toValue);
        env->ReleaseStringUTFChars(abiString, abiValue);

        return tx_data_struct;
    }
    catch (...)
    {
        return NULL;
    }
}

jobject convert_to_tx_data_jobject(
    JNIEnv* env, const struct bcos_sdk_c_transaction_data* transactionData)
{
    try
    {
        if (transactionData == NULL)
        {
            THROW_JNI_EXCEPTION(
                env, "exception in convert_to_tx_data_jobject, transactionData is nullptr");
        }

        jclass txDataClass = env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionData");
        if (txDataClass == NULL)
        {
            env->FatalError(
                "No such class, className: "
                "org/fisco/bcos/sdk/jni/utilities/tx/TransactionData");
        }
        jmethodID txDataMtd = env->GetMethodID(txDataClass, "<init>", "()V");
        if (txDataMtd == NULL)
        {
            env->FatalError("No such constructor in TransactionData, constructor()");
        }
        jobject jTxDataObj = env->NewObject(txDataClass, txDataMtd);

        // version
        jfieldID versionField = env->GetFieldID(txDataClass, "version", "I");
        env->SetIntField(jTxDataObj, versionField, transactionData->version);
        // block_limit
        jfieldID blockLimitField = env->GetFieldID(txDataClass, "blockLimit", "J");
        env->SetLongField(jTxDataObj, blockLimitField, transactionData->block_limit);
        // chain_id
        jfieldID chainIdField = env->GetFieldID(txDataClass, "chainId", "Ljava/lang/String;");
        env->SetObjectField(jTxDataObj, chainIdField, env->NewStringUTF(transactionData->chain_id));
        // group_id
        jfieldID groupIdField = env->GetFieldID(txDataClass, "groupId", "Ljava/lang/String;");
        env->SetObjectField(jTxDataObj, groupIdField, env->NewStringUTF(transactionData->group_id));
        // nonce
        jfieldID nonceField = env->GetFieldID(txDataClass, "nonce", "Ljava/lang/String;");
        env->SetObjectField(jTxDataObj, nonceField, env->NewStringUTF(transactionData->nonce));
        // to
        jfieldID toField = env->GetFieldID(txDataClass, "to", "Ljava/lang/String;");
        env->SetObjectField(jTxDataObj, toField, env->NewStringUTF(transactionData->to));
        // abi
        jfieldID abiField = env->GetFieldID(txDataClass, "abi", "Ljava/lang/String;");
        env->SetObjectField(jTxDataObj, abiField, env->NewStringUTF(transactionData->abi));

        // input
        jobject jInput = convert_to_bytes_jobject(env, transactionData->input);
        if (jInput == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_jobject");
        }
        jfieldID inputField = env->GetFieldID(
            txDataClass, "input", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        env->SetObjectField(jTxDataObj, inputField, jInput);

        // release
        env->DeleteLocalRef(jInput);

        return jTxDataObj;
    }
    catch (...)
    {
        return NULL;
    }
}

jobject convert_to_tx_jobject(JNIEnv* env, const struct bcos_sdk_c_transaction* tx_struct)
{
    try
    {
        if (tx_struct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_jobject, tx_struct is nullptr");
        }

        jclass txClass = env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/Transaction");
        if (txClass == NULL)
        {
            env->FatalError(
                "No such class, className: "
                "org/fisco/bcos/sdk/jni/utilities/tx/Transaction");
        }
        jmethodID txMtd = env->GetMethodID(txClass, "<init>", "()V");
        if (txMtd == NULL)
        {
            env->FatalError("No such constructor in Transaction, constructor()");
        }
        jfieldID txDataField = env->GetFieldID(
            txClass, "transactionData", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;");
        jfieldID dataHashField = env->GetFieldID(
            txClass, "dataHash", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        jfieldID signatureField = env->GetFieldID(
            txClass, "signature", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        jfieldID senderField = env->GetFieldID(
            txClass, "sender", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        jfieldID importTimeField = env->GetFieldID(txClass, "importTime", "J");
        jfieldID attributeField = env->GetFieldID(txClass, "attribute", "I");
        jfieldID extraDataField = env->GetFieldID(txClass, "extraData", "Ljava/lang/String;");

        jobject javaTxObj = env->NewObject(txClass, txMtd);
        // TransactionData
        jobject javaTxDataObj = convert_to_tx_data_jobject(env, tx_struct->transaction_data);
        if (javaTxDataObj == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_jobject");
        }
        env->SetObjectField(javaTxObj, txDataField, javaTxDataObj);
        // DataHash
        jobject javaDataHashObj = convert_to_bytes_jobject(env, tx_struct->data_hash);
        if (javaDataHashObj == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_jobject");
        }
        env->SetObjectField(javaTxObj, dataHashField, javaDataHashObj);
        // Signature
        jobject javaSignatureObj = convert_to_bytes_jobject(env, tx_struct->signature);
        if (javaSignatureObj == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_jobject");
        }
        env->SetObjectField(javaTxObj, signatureField, javaSignatureObj);
        // Sender
        jobject javaSenderObj = convert_to_bytes_jobject(env, tx_struct->sender);
        env->SetObjectField(javaTxObj, senderField, javaSenderObj);
        // ImportTime
        env->SetLongField(javaTxObj, importTimeField, tx_struct->import_time);
        // Attribute
        env->SetIntField(javaTxObj, attributeField, tx_struct->attribute);
        // ExtraData
        jstring javaExtraData = env->NewStringUTF(tx_struct->extra_data);
        env->SetObjectField(javaTxObj, extraDataField, javaExtraData);

        return javaTxObj;
    }
    catch (...)
    {
        return NULL;
    }
}

struct bcos_sdk_c_transaction* convert_to_tx_struct(JNIEnv* env, jobject jTxObj)
{
    try
    {
        if (jTxObj == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct, jTxObj is nullptr");
        }

        jclass javaTxClass = env->GetObjectClass(jTxObj);
        jfieldID transactionDataField = env->GetFieldID(javaTxClass, "transactionData",
            "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;");
        jfieldID dataHashField = env->GetFieldID(
            javaTxClass, "dataHash", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        jfieldID signatureField = env->GetFieldID(
            javaTxClass, "signature", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        jfieldID senderField = env->GetFieldID(
            javaTxClass, "sender", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        jfieldID importTimeField = env->GetFieldID(javaTxClass, "importTime", "J");
        jfieldID attributeField = env->GetFieldID(javaTxClass, "attribute", "I");
        jfieldID extraDataField = env->GetFieldID(javaTxClass, "extraData", "Ljava/lang/String;");

        struct bcos_sdk_c_transaction* txStruct =
            (struct bcos_sdk_c_transaction*)malloc(sizeof(struct bcos_sdk_c_transaction));
        // TransactionData
        jobject javaTxDataObj = env->GetObjectField(jTxObj, transactionDataField);
        struct bcos_sdk_c_transaction_data* txDataStruct =
            convert_to_tx_data_struct(env, javaTxDataObj);
        if (txDataStruct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
        }
        txStruct->transaction_data = txDataStruct;
        // DataHash
        jobject javaDataHashObj = env->GetObjectField(jTxObj, dataHashField);
        struct bcos_sdk_c_bytes* dataHashStruct = convert_to_bytes_struct(env, javaDataHashObj);
        if (dataHashStruct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_struct");
        }
        txStruct->data_hash = dataHashStruct;
        // Signature
        jobject javaSignatureObj = env->GetObjectField(jTxObj, signatureField);
        struct bcos_sdk_c_bytes* signatureStruct = convert_to_bytes_struct(env, javaSignatureObj);
        if (signatureStruct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_struct");
        }
        txStruct->signature = signatureStruct;
        // Sender
        jobject javaSenderObj = env->GetObjectField(jTxObj, senderField);
        struct bcos_sdk_c_bytes* senderStruct = convert_to_bytes_struct(env, javaSenderObj);
        txStruct->sender = senderStruct;
        // ImportTime
        jlong importTimeValue = env->GetLongField(jTxObj, importTimeField);
        txStruct->import_time = (int64_t)importTimeValue;
        // Attribute
        jint attributeValue = env->GetIntField(jTxObj, attributeField);
        txStruct->attribute = (int32_t)attributeValue;
        // ExtraData
        jstring javaExtraData = (jstring)env->GetObjectField(jTxObj, extraDataField);
        const char* extraDataValue = env->GetStringUTFChars(javaExtraData, NULL);
        txStruct->extra_data = strdup(extraDataValue);

        env->ReleaseStringUTFChars(javaExtraData, extraDataValue);

        return txStruct;
    }
    catch (...)
    {
        return NULL;
    }
}

void destroy_bytes_jobject(JNIEnv* env, jobject jTxBytes)
{
    if (jTxBytes == NULL)
    {
        return;
    }

    jclass jTxBytesClass = env->GetObjectClass(jTxBytes);
    jfieldID jBufferFieldID = env->GetFieldID(jTxBytesClass, "buffer", "[B");
    jbyteArray jBufferArray = (jbyteArray)env->GetObjectField(jTxBytes, jBufferFieldID);

    env->DeleteLocalRef(jBufferArray);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj
 * Method:    encodeTransactionDataStruct
 * Signature: (Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj_encodeTransactionDataStruct(
    JNIEnv* env, jclass, jobject jTransactionDataObj)
{
    struct bcos_sdk_c_transaction_data* tx_data_struct =
        convert_to_tx_data_struct(env, jTransactionDataObj);
    if (tx_data_struct == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
    }
    const char* tx_data_hex = bcos_sdk_encode_transaction_data_struct(tx_data_struct);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jTxDataHex = env->NewStringUTF(tx_data_hex);

    if (tx_data_hex)
    {
        free((void*)tx_data_hex);
        tx_data_hex = NULL;
    }
    if (tx_data_struct)
    {
        bcos_sdk_destroy_transaction_data_struct(tx_data_struct);
    }

    return jTxDataHex;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj
 * Method:    encodeTransactionDataStructToJson
 * Signature: (Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj_encodeTransactionDataStructToJson(
    JNIEnv* env, jclass, jobject jTransactionDataObj)
{
    struct bcos_sdk_c_transaction_data* tx_data_struct =
        convert_to_tx_data_struct(env, jTransactionDataObj);
    if (tx_data_struct == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
    }
    const char* tx_data_json = bcos_sdk_encode_transaction_data_struct_to_json(tx_data_struct);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jTxDataJson = env->NewStringUTF(tx_data_json);

    if (tx_data_json)
    {
        free((void*)tx_data_json);
        tx_data_json = NULL;
    }
    if (tx_data_struct)
    {
        bcos_sdk_destroy_transaction_data_struct(tx_data_struct);
    }

    return jTxDataJson;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj
 * Method:    decodeTransactionDataStruct
 * Signature: (Ljava/lang/String;)Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;
 */
JNIEXPORT jobject JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj_decodeTransactionDataStruct(
    JNIEnv* env, jclass, jstring jTxDataHexStr)
{
    checkJString(env, jTxDataHexStr);
    const char* tx_data_hex_str = env->GetStringUTFChars(jTxDataHexStr, nullptr);
    struct bcos_sdk_c_transaction_data* tx_data_struct =
        bcos_sdk_decode_transaction_data_struct(tx_data_hex_str);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jobject jTxDataObj = convert_to_tx_data_jobject(env, tx_data_struct);
    if (jTxDataObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_jobject");
    }

    // release source
    if (tx_data_struct)
    {
        bcos_sdk_destroy_transaction_data_struct(tx_data_struct);
    }
    env->ReleaseStringUTFChars(jTxDataHexStr, tx_data_hex_str);

    return jTxDataObj;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj
 * Method:    calcTransactionDataStructHash
 * Signature: (ILorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj_calcTransactionDataStructHash(
    JNIEnv* env, jclass, jint jCrytpTyte, jobject jTransactionDataObj)
{
    struct bcos_sdk_c_transaction_data* tx_data_struct =
        convert_to_tx_data_struct(env, jTransactionDataObj);
    if (tx_data_struct == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
    }
    int crypto_type = jCrytpTyte;
    const char* tx_data_hash =
        bcos_sdk_calc_transaction_data_struct_hash(crypto_type, tx_data_struct);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jTxDataHash = env->NewStringUTF(tx_data_hash);

    // release source
    if (tx_data_struct)
    {
        bcos_sdk_destroy_transaction_data_struct(tx_data_struct);
    }
    if (tx_data_hash)
    {
        free((void*)tx_data_hash);
        tx_data_hash = NULL;
    }

    return jTxDataHash;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj
 * Method:    createEncodedTransaction
 * Signature:
 * (Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj_createEncodedTransaction(
    JNIEnv* env, jclass, jobject jTxDataObj, jstring jSignature, jstring jTxDataHash,
    jint jAttribute, jstring jExtraData)
{
    checkJString(env, jSignature);
    checkJString(env, jTxDataHash);
    checkJString(env, jExtraData);

    struct bcos_sdk_c_transaction_data* tx_data_struct = convert_to_tx_data_struct(env, jTxDataObj);
    if (tx_data_struct == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
    }
    const char* signature = env->GetStringUTFChars(jSignature, NULL);
    const char* tx_data_hash = env->GetStringUTFChars(jTxDataHash, NULL);
    int attribute = jAttribute;
    const char* extra_data = env->GetStringUTFChars(jExtraData, NULL);

    const char* tx_str = bcos_sdk_create_encoded_transaction(
        tx_data_struct, signature, tx_data_hash, attribute, extra_data);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jTxStr = env->NewStringUTF(tx_str);

    // release resource
    env->ReleaseStringUTFChars(jSignature, signature);
    env->ReleaseStringUTFChars(jTxDataHash, tx_data_hash);
    env->ReleaseStringUTFChars(jExtraData, extra_data);
    if (tx_data_struct)
    {
        bcos_sdk_destroy_transaction_data_struct(tx_data_struct);
    }
    if (tx_str)
    {
        free((void*)tx_str);
        tx_str = NULL;
    }

    return jTxStr;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj
 * Method:    encodeTransactionStruct
 * Signature: (Lorg/fisco/bcos/sdk/jni/utilities/tx/Transaction;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj_encodeTransactionStruct(
    JNIEnv* env, jclass, jobject jTransactionObj)
{
    struct bcos_sdk_c_transaction* tx_struct = convert_to_tx_struct(env, jTransactionObj);
    if (tx_struct == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct");
    }
    const char* tx_hex = bcos_sdk_encode_transaction_struct(tx_struct);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jTxHex = env->NewStringUTF(tx_hex);

    // release source
    if (tx_hex)
    {
        free((void*)tx_hex);
        tx_hex = NULL;
    }
    if (tx_struct)
    {
        bcos_sdk_destroy_transaction_struct(tx_struct);
    }

    return jTxHex;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj
 * Method:    encodeTransactionStructToJson
 * Signature: (Lorg/fisco/bcos/sdk/jni/utilities/tx/Transaction;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj_encodeTransactionStructToJson(
    JNIEnv* env, jclass, jobject jTransactionObj)
{
    struct bcos_sdk_c_transaction* tx_struct = convert_to_tx_struct(env, jTransactionObj);
    if (tx_struct == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct");
    }
    const char* tx_json = bcos_sdk_encode_transaction_struct_to_json(tx_struct);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jTxJson = env->NewStringUTF(tx_json);

    if (tx_json)
    {
        free((void*)tx_json);
        tx_json = NULL;
    }
    if (tx_struct)
    {
        bcos_sdk_destroy_transaction_struct(tx_struct);
    }

    return jTxJson;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj
 * Method:    decodeTransactionStruct
 * Signature: (Ljava/lang/String;)Lorg/fisco/bcos/sdk/jni/utilities/tx/Transaction;
 */
JNIEXPORT jobject JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj_decodeTransactionStruct(
    JNIEnv* env, jclass, jstring jTxHexStr)
{
    checkJString(env, jTxHexStr);

    const char* tx_hex_str = env->GetStringUTFChars(jTxHexStr, nullptr);
    struct bcos_sdk_c_transaction* tx_struct = bcos_sdk_decode_transaction_struct(tx_hex_str);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jobject jTxObj = convert_to_tx_jobject(env, tx_struct);

    // release source
    if (tx_struct)
    {
        bcos_sdk_destroy_transaction_struct(tx_struct);
    }
    env->ReleaseStringUTFChars(jTxHexStr, tx_hex_str);

    return jTxObj;
}
