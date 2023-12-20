#include "org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_tx_struct.h"
#include "org_fisco_bcos_sdk_exception.h"

struct bcos_sdk_c_bytes* convert_to_bytes_struct_v2(JNIEnv* env, jobject bytesObj)
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

jobject convert_to_bytes_jobject_v2(JNIEnv* env, const struct bcos_sdk_c_bytes* bytes_struct)
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

struct bcos_sdk_c_transaction_data_v2* convert_to_tx_data_struct_v2(
    JNIEnv* env, jobject transactionObject)
{
    try
    {
        if (transactionObject == NULL)
        {
            THROW_JNI_EXCEPTION(
                env, "exception in convert_to_tx_data_struct_v2, transactionObject is nullptr");
        }

        bcos_sdk_c_transaction_data_v2* tx_data_struct_v2 =
            (struct bcos_sdk_c_transaction_data_v2*)malloc(
                sizeof(struct bcos_sdk_c_transaction_data_v2));
        jclass txDataClass = env->GetObjectClass(transactionObject);

        // version
        jfieldID versionField = env->GetFieldID(txDataClass, "version", "I");
        tx_data_struct_v2->version = env->GetIntField(transactionObject, versionField);

        // blockLimit
        jfieldID blockLimitField = env->GetFieldID(txDataClass, "blockLimit", "J");
        tx_data_struct_v2->block_limit = env->GetLongField(transactionObject, blockLimitField);

        // chainId
        jfieldID chainIdField = env->GetFieldID(txDataClass, "chainId", "Ljava/lang/String;");
        jstring chainIdString = (jstring)env->GetObjectField(transactionObject, chainIdField);
        const char* chainIdValue = env->GetStringUTFChars(chainIdString, nullptr);
        tx_data_struct_v2->chain_id = strdup(chainIdValue);

        // groupId
        jfieldID groupIdField = env->GetFieldID(txDataClass, "groupId", "Ljava/lang/String;");
        jstring groupIdString = (jstring)env->GetObjectField(transactionObject, groupIdField);
        const char* groupIdValue = env->GetStringUTFChars(groupIdString, nullptr);
        tx_data_struct_v2->group_id = strdup(groupIdValue);

        // nonce
        jfieldID nonceField = env->GetFieldID(txDataClass, "nonce", "Ljava/lang/String;");
        jstring nonceString = (jstring)env->GetObjectField(transactionObject, nonceField);
        const char* nonceValue = env->GetStringUTFChars(nonceString, nullptr);
        tx_data_struct_v2->nonce = strdup(nonceValue);

        // to
        jfieldID toField = env->GetFieldID(txDataClass, "to", "Ljava/lang/String;");
        jstring toString = (jstring)env->GetObjectField(transactionObject, toField);
        const char* toValue = env->GetStringUTFChars(toString, nullptr);
        tx_data_struct_v2->to = strdup(toValue);

        // abi
        jfieldID abiField = env->GetFieldID(txDataClass, "abi", "Ljava/lang/String;");
        jstring abiString = (jstring)env->GetObjectField(transactionObject, abiField);
        const char* abiValue = env->GetStringUTFChars(abiString, nullptr);
        tx_data_struct_v2->abi = strdup(abiValue);

        // input
        jfieldID inputField = env->GetFieldID(
            txDataClass, "input", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        jobject inputObject = env->GetObjectField(transactionObject, inputField);
        struct bcos_sdk_c_bytes* inputStruct = convert_to_bytes_struct_v2(env, inputObject);
        if (inputStruct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_struct_v2");
        }
        tx_data_struct_v2->input = inputStruct;

        // value
        jfieldID valueField = env->GetFieldID(txDataClass, "value", "Ljava/lang/String;");
        jstring valueString = (jstring)env->GetObjectField(transactionObject, valueField);
        const char* valueChar = env->GetStringUTFChars(valueString, nullptr);
        tx_data_struct_v2->value = strdup(valueChar);

        // gas_price
        jfieldID gasPriceField = env->GetFieldID(txDataClass, "gasPrice", "Ljava/lang/String;");
        jstring gasPriceString = (jstring)env->GetObjectField(transactionObject, gasPriceField);
        const char* gasPriceChar = env->GetStringUTFChars(gasPriceString, nullptr);
        tx_data_struct_v2->gas_price = strdup(gasPriceChar);

        // gas_limit
        jfieldID gasLimitField = env->GetFieldID(txDataClass, "gasLimit", "J");
        tx_data_struct_v2->gas_limit = env->GetLongField(transactionObject, gasLimitField);

        // max_fee_per_gas
        jfieldID maxFeePerGasField =
            env->GetFieldID(txDataClass, "maxFeePerGas", "Ljava/lang/String;");
        jstring maxFeePerGasString =
            (jstring)env->GetObjectField(transactionObject, maxFeePerGasField);
        const char* maxFeePerGasChar = env->GetStringUTFChars(maxFeePerGasString, nullptr);
        tx_data_struct_v2->max_fee_per_gas = strdup(maxFeePerGasChar);

        // max_priority_fee_per_gas
        jfieldID maxPriorityFeePerGasField =
            env->GetFieldID(txDataClass, "maxPriorityFeePerGas", "Ljava/lang/String;");
        jstring maxPriorityFeePerGasString =
            (jstring)env->GetObjectField(transactionObject, maxPriorityFeePerGasField);
        const char* maxPriorityFeePerGasChar =
            env->GetStringUTFChars(maxPriorityFeePerGasString, nullptr);
        tx_data_struct_v2->max_priority_fee_per_gas = strdup(maxPriorityFeePerGasChar);

        // release source
        env->ReleaseStringUTFChars(chainIdString, chainIdValue);
        env->ReleaseStringUTFChars(groupIdString, groupIdValue);
        env->ReleaseStringUTFChars(nonceString, nonceValue);
        env->ReleaseStringUTFChars(toString, toValue);
        env->ReleaseStringUTFChars(abiString, abiValue);
        env->ReleaseStringUTFChars(valueString, valueChar);
        env->ReleaseStringUTFChars(gasPriceString, gasPriceChar);
        env->ReleaseStringUTFChars(maxFeePerGasString, maxPriorityFeePerGasChar);
        env->ReleaseStringUTFChars(maxPriorityFeePerGasString, maxPriorityFeePerGasChar);

        return tx_data_struct_v2;
    }
    catch (...)
    {
        return NULL;
    }
}

jobject convert_to_tx_data_jobject_v2(
    JNIEnv* env, const struct bcos_sdk_c_transaction_data_v2* transactionDataV2)
{
    try
    {
        jclass txDataClass =
            env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2");
        if (txDataClass == NULL)
        {
            env->FatalError(
                "No such class, className: "
                "org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2");
        }
        jmethodID txDataMtd = env->GetMethodID(txDataClass, "<init>", "()V");
        if (txDataMtd == NULL)
        {
            env->FatalError("No such constructor in TransactionDataV2, constructor()");
        }
        jobject jTxDataObj = env->NewObject(txDataClass, txDataMtd);

        // version
        jfieldID versionField = env->GetFieldID(txDataClass, "version", "I");
        env->SetIntField(jTxDataObj, versionField, transactionDataV2->version);
        // block_limit
        jfieldID blockLimitField = env->GetFieldID(txDataClass, "blockLimit", "J");
        env->SetLongField(jTxDataObj, blockLimitField, transactionDataV2->block_limit);
        // chain_id
        jfieldID chainIdField = env->GetFieldID(txDataClass, "chainId", "Ljava/lang/String;");
        env->SetObjectField(
            jTxDataObj, chainIdField, env->NewStringUTF(transactionDataV2->chain_id));
        // group_id
        jfieldID groupIdField = env->GetFieldID(txDataClass, "groupId", "Ljava/lang/String;");
        env->SetObjectField(
            jTxDataObj, groupIdField, env->NewStringUTF(transactionDataV2->group_id));
        // nonce
        jfieldID nonceField = env->GetFieldID(txDataClass, "nonce", "Ljava/lang/String;");
        env->SetObjectField(jTxDataObj, nonceField, env->NewStringUTF(transactionDataV2->nonce));
        // to
        jfieldID toField = env->GetFieldID(txDataClass, "to", "Ljava/lang/String;");
        env->SetObjectField(jTxDataObj, toField, env->NewStringUTF(transactionDataV2->to));
        // abi
        jfieldID abiField = env->GetFieldID(txDataClass, "abi", "Ljava/lang/String;");
        env->SetObjectField(jTxDataObj, abiField, env->NewStringUTF(transactionDataV2->abi));
        // input
        jobject jInput = convert_to_bytes_jobject_v2(env, transactionDataV2->input);
        if (jInput == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_jobject_v2");
        }
        jfieldID inputField = env->GetFieldID(
            txDataClass, "input", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        env->SetObjectField(jTxDataObj, inputField, jInput);
        // value
        jfieldID valueField = env->GetFieldID(txDataClass, "value", "Ljava/lang/String;");
        env->SetObjectField(jTxDataObj, valueField, env->NewStringUTF(transactionDataV2->value));
        // gas_price
        jfieldID gasPriceField = env->GetFieldID(txDataClass, "gasPrice", "Ljava/lang/String;");
        env->SetObjectField(
            jTxDataObj, gasPriceField, env->NewStringUTF(transactionDataV2->gas_price));
        // gas_limit
        jfieldID gasLimitField = env->GetFieldID(txDataClass, "gasLimit", "J");
        env->SetLongField(jTxDataObj, gasLimitField, transactionDataV2->gas_limit);
        // max_fee_per_gas
        jfieldID maxFeePerGasField =
            env->GetFieldID(txDataClass, "maxFeePerGas", "Ljava/lang/String;");
        env->SetObjectField(
            jTxDataObj, maxFeePerGasField, env->NewStringUTF(transactionDataV2->max_fee_per_gas));
        // max_priority_fee_per_gas
        jfieldID maxPriorityFeePerGasField =
            env->GetFieldID(txDataClass, "maxPriorityFeePerGas", "Ljava/lang/String;");
        env->SetObjectField(jTxDataObj, maxPriorityFeePerGasField,
            env->NewStringUTF(transactionDataV2->max_priority_fee_per_gas));

        // release
        env->DeleteLocalRef(jInput);

        return jTxDataObj;
    }
    catch (...)
    {
        return NULL;
    }
}

jobject convert_to_tx_jobject_v2(JNIEnv* env, const struct bcos_sdk_c_transaction_v2* tx_struct_v2)
{
    try
    {
        if (tx_struct_v2 == NULL)
        {
            THROW_JNI_EXCEPTION(
                env, "exception in convert_to_tx_jobject_v2, tx_struct_v2 is nullptr");
        }

        jclass txClass = env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionV2");
        if (txClass == NULL)
        {
            env->FatalError(
                "No such class, className: "
                "org/fisco/bcos/sdk/jni/utilities/tx/TransactionV2");
        }
        jmethodID txMtd = env->GetMethodID(txClass, "<init>", "()V");
        if (txMtd == NULL)
        {
            env->FatalError("No such constructor in TransactionV2, constructor()");
        }
        jfieldID txDataField = env->GetFieldID(txClass, "transactionDataV2",
            "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2;");
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
        jobject javaTxDataObj = convert_to_tx_data_jobject_v2(env, tx_struct_v2->transaction_data);
        if (javaTxDataObj == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_jobject_v2");
        }
        env->SetObjectField(javaTxObj, txDataField, javaTxDataObj);
        // DataHash
        jobject javaDataHashObj = convert_to_bytes_jobject_v2(env, tx_struct_v2->data_hash);
        if (javaDataHashObj == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_jobject_v2");
        }
        env->SetObjectField(javaTxObj, dataHashField, javaDataHashObj);
        // Signature
        jobject javaSignatureObj = convert_to_bytes_jobject_v2(env, tx_struct_v2->signature);
        if (javaSignatureObj == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_jobject_v2");
        }
        env->SetObjectField(javaTxObj, signatureField, javaSignatureObj);
        // Sender
        jobject javaSenderObj = convert_to_bytes_jobject_v2(env, tx_struct_v2->sender);
        if (javaSenderObj == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_jobject_v2");
        }
        env->SetObjectField(javaTxObj, senderField, javaSenderObj);
        // ImportTime
        env->SetLongField(javaTxObj, importTimeField, tx_struct_v2->import_time);
        // Attribute
        env->SetIntField(javaTxObj, attributeField, tx_struct_v2->attribute);
        // ExtraData
        jstring javaExtraData = env->NewStringUTF(tx_struct_v2->extra_data);
        env->SetObjectField(javaTxObj, extraDataField, javaExtraData);

        return javaTxObj;
    }
    catch (...)
    {
        return NULL;
    }
}

struct bcos_sdk_c_transaction_v2* convert_to_tx_struct_v2(JNIEnv* env, jobject jTxObj)
{
    try
    {
        jclass javaTxClass = env->GetObjectClass(jTxObj);
        jfieldID transactionDataField = env->GetFieldID(javaTxClass, "transactionData",
            "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2;");
        jfieldID dataHashField = env->GetFieldID(
            javaTxClass, "dataHash", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        jfieldID signatureField = env->GetFieldID(
            javaTxClass, "signature", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        jfieldID senderField = env->GetFieldID(
            javaTxClass, "sender", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionBytes;");
        jfieldID importTimeField = env->GetFieldID(javaTxClass, "importTime", "J");
        jfieldID attributeField = env->GetFieldID(javaTxClass, "attribute", "I");
        jfieldID extraDataField = env->GetFieldID(javaTxClass, "extraData", "Ljava/lang/String;");

        struct bcos_sdk_c_transaction_v2* txStruct =
            (struct bcos_sdk_c_transaction_v2*)malloc(sizeof(struct bcos_sdk_c_transaction_v2));
        // TransactionData
        jobject javaTxDataObj = env->GetObjectField(jTxObj, transactionDataField);
        struct bcos_sdk_c_transaction_data_v2* txDataStruct =
            convert_to_tx_data_struct_v2(env, javaTxDataObj);
        if (txDataStruct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct_v2");
        }
        txStruct->transaction_data = txDataStruct;
        // DataHash
        jobject javaDataHashObj = env->GetObjectField(jTxObj, dataHashField);
        struct bcos_sdk_c_bytes* dataHashStruct = convert_to_bytes_struct_v2(env, javaDataHashObj);
        if (dataHashStruct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_struct_v2");
        }
        txStruct->data_hash = dataHashStruct;
        // Signature
        jobject javaSignatureObj = env->GetObjectField(jTxObj, signatureField);
        struct bcos_sdk_c_bytes* signatureStruct =
            convert_to_bytes_struct_v2(env, javaSignatureObj);
        if (signatureStruct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_struct_v2");
        }
        txStruct->signature = signatureStruct;
        // Sender
        jobject javaSenderObj = env->GetObjectField(jTxObj, senderField);
        struct bcos_sdk_c_bytes* senderStruct = convert_to_bytes_struct_v2(env, javaSenderObj);
        if (senderStruct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_bytes_struct_v2");
        }
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

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj
 * Method:    encodeTransactionDataStructV2
 * Signature: (Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj_encodeTransactionDataStructV2(
    JNIEnv* env, jclass, jobject jTransactionDataObj)
{
    struct bcos_sdk_c_transaction_data_v2* tx_data_struct_v2 =
        convert_to_tx_data_struct_v2(env, jTransactionDataObj);
    if (tx_data_struct_v2 == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct_v2");
    }
    const char* tx_data_hex = bcos_sdk_encode_transaction_data_struct_v2(tx_data_struct_v2);
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
    if (tx_data_struct_v2)
    {
        bcos_sdk_destroy_transaction_data_struct_v2(tx_data_struct_v2);
    }

    return jTxDataHex;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj
 * Method:    encodeTransactionDataStructToJsonV2
 * Signature: (Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj_encodeTransactionDataStructToJsonV2(
    JNIEnv* env, jclass, jobject jTransactionDataObj)
{
    struct bcos_sdk_c_transaction_data_v2* tx_data_struct_v2 =
        convert_to_tx_data_struct_v2(env, jTransactionDataObj);
    if (tx_data_struct_v2 == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct_v2");
    }
    const char* tx_data_json =
        bcos_sdk_encode_transaction_data_struct_to_json_v2(tx_data_struct_v2);
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
    if (tx_data_struct_v2)
    {
        bcos_sdk_destroy_transaction_data_struct_v2(tx_data_struct_v2);
    }

    return jTxDataJson;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj
 * Method:    decodeTransactionDataStructV2
 * Signature: (Ljava/lang/String;)Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2;
 */
JNIEXPORT jobject JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj_decodeTransactionDataStructV2(
    JNIEnv* env, jclass, jstring jTxDataHexStr)
{
    const char* tx_data_hex_str = env->GetStringUTFChars(jTxDataHexStr, nullptr);
    struct bcos_sdk_c_transaction_data_v2* tx_data_struct_v2 =
        bcos_sdk_decode_transaction_data_struct_v2(tx_data_hex_str);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jobject jTxDataObj = convert_to_tx_data_jobject_v2(env, tx_data_struct_v2);
    if (jTxDataObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_jobject_v2");
    }

    // release source
    if (tx_data_struct_v2)
    {
        bcos_sdk_destroy_transaction_data_struct_v2(tx_data_struct_v2);
    }
    env->ReleaseStringUTFChars(jTxDataHexStr, tx_data_hex_str);

    return jTxDataObj;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj
 * Method:    calcTransactionDataStructHashV2
 * Signature: (ILorg/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj_calcTransactionDataStructHashV2(
    JNIEnv* env, jclass, jint jCrytpTyte, jobject jTransactionDataObj)
{
    struct bcos_sdk_c_transaction_data_v2* tx_data_struct_v2 =
        convert_to_tx_data_struct_v2(env, jTransactionDataObj);
    if (tx_data_struct_v2 == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct_v2");
    }
    int crypto_type = jCrytpTyte;
    const char* tx_data_hash =
        bcos_sdk_calc_transaction_data_struct_hash_v2(crypto_type, tx_data_struct_v2);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jTxDataHash = env->NewStringUTF(tx_data_hash);

    // release source
    if (tx_data_struct_v2)
    {
        bcos_sdk_destroy_transaction_data_struct_v2(tx_data_struct_v2);
    }
    if (tx_data_hash)
    {
        free((void*)tx_data_hash);
        tx_data_hash = NULL;
    }

    return jTxDataHash;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj
 * Method:    createEncodedTransactionV2
 * Signature:
 * (Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj_createEncodedTransactionV2(
    JNIEnv* env, jclass, jobject jTxDataObj, jstring jSignature, jstring jTxDataHash,
    jint jAttribute, jstring jExtraData)
{
    struct bcos_sdk_c_transaction_data_v2* tx_data_struct_v2 =
        convert_to_tx_data_struct_v2(env, jTxDataObj);
    if (tx_data_struct_v2 == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct_v2");
    }
    const char* signature = env->GetStringUTFChars(jSignature, NULL);
    const char* tx_data_hash = env->GetStringUTFChars(jTxDataHash, NULL);
    int attribute = jAttribute;
    const char* extra_data = env->GetStringUTFChars(jExtraData, NULL);

    const char* tx_str = bcos_sdk_create_encoded_transaction_v2(
        tx_data_struct_v2, signature, tx_data_hash, attribute, extra_data);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jTxStr = env->NewStringUTF(tx_str);

    // release resource
    env->ReleaseStringUTFChars(jSignature, signature);
    env->ReleaseStringUTFChars(jTxDataHash, tx_data_hash);
    env->ReleaseStringUTFChars(jExtraData, extra_data);
    if (tx_data_struct_v2)
    {
        bcos_sdk_destroy_transaction_data_struct_v2(tx_data_struct_v2);
    }
    if (tx_str)
    {
        free((void*)tx_str);
        tx_str = NULL;
    }

    return jTxStr;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj
 * Method:    encodeTransactionStructV2
 * Signature: (Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionV2;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj_encodeTransactionStructV2(
    JNIEnv* env, jclass, jobject jTransactionObj)
{
    struct bcos_sdk_c_transaction_v2* tx_struct_v2 = convert_to_tx_struct_v2(env, jTransactionObj);
    if (tx_struct_v2 == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct_v2");
    }
    const char* tx_hex = bcos_sdk_encode_transaction_struct_v2(tx_struct_v2);
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
    if (tx_struct_v2)
    {
        bcos_sdk_destroy_transaction_struct_v2(tx_struct_v2);
    }

    return jTxHex;
}


/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj
 * Method:    encodeTransactionStructToJsonV2
 * Signature: (Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionV2;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj_encodeTransactionStructToJsonV2(
    JNIEnv* env, jclass, jobject jTransactionObj)
{
    struct bcos_sdk_c_transaction_v2* tx_struct_v2 = convert_to_tx_struct_v2(env, jTransactionObj);
    if (tx_struct_v2 == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct_v2");
    }
    const char* tx_json = bcos_sdk_encode_transaction_struct_to_json_v2(tx_struct_v2);
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
    if (tx_struct_v2)
    {
        bcos_sdk_destroy_transaction_struct_v2(tx_struct_v2);
    }

    return jTxJson;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj
 * Method:    decodeTransactionStructV2
 * Signature: (Ljava/lang/String;)Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionV2;
 */
JNIEXPORT jobject JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderV2JniObj_decodeTransactionStructV2(
    JNIEnv* env, jclass, jstring jTxHexStr)
{
    const char* tx_hex_str = env->GetStringUTFChars(jTxHexStr, nullptr);
    struct bcos_sdk_c_transaction_v2* tx_struct_v2 =
        bcos_sdk_decode_transaction_struct_v2(tx_hex_str);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jobject jTxObj = convert_to_tx_jobject_v2(env, tx_struct_v2);

    // release source
    if (tx_struct_v2)
    {
        bcos_sdk_destroy_transaction_struct_v2(tx_struct_v2);
    }
    env->ReleaseStringUTFChars(jTxHexStr, tx_hex_str);

    return jTxObj;
}
