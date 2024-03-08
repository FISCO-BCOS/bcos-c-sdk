#include "org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_tx_struct.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_tx_struct_v1.h"
#include "org_fisco_bcos_sdk_common.h"
#include "org_fisco_bcos_sdk_exception.h"

#include <bcos_sdk_c_uti_tx_struct_v2.h>


// tx data v2: java obj => c struct
struct bcos_sdk_c_transaction_data_v2* convert_to_tx_v2_data_struct(
    JNIEnv* env, jobject transactionObject)
{
    if (transactionObject == nullptr)
    {
        THROW_JNI_EXCEPTION(
            env, "exception in convert_to_tx_data_struct, transactionObject is nullptr");
        return nullptr;
    }

    jclass txDataClass = env->GetObjectClass(transactionObject);

    auto* tx_data_struct = (struct bcos_sdk_c_transaction_data_v2*)malloc(
        sizeof(struct bcos_sdk_c_transaction_data_v2));
    // version
    jfieldID versionField = env->GetFieldID(txDataClass, "version", "I");
    tx_data_struct->base_v1.base.version = env->GetIntField(transactionObject, versionField);

    // blockLimit
    jfieldID blockLimitField = env->GetFieldID(txDataClass, "blockLimit", "J");
    tx_data_struct->base_v1.base.block_limit =
        env->GetLongField(transactionObject, blockLimitField);

    // chainId
    jfieldID chainIdField = env->GetFieldID(txDataClass, "chainId", "Ljava/lang/String;");
    auto chainIdString = (jstring)env->GetObjectField(transactionObject, chainIdField);
    const char* chainIdValue = GET_J_STRING_CONTENT_DEF(env, chainIdString, "");
    tx_data_struct->base_v1.base.chain_id = strdup(chainIdValue);

    // groupId
    jfieldID groupIdField = env->GetFieldID(txDataClass, "groupId", "Ljava/lang/String;");
    auto groupIdString = (jstring)env->GetObjectField(transactionObject, groupIdField);
    const char* groupIdValue = GET_J_STRING_CONTENT_DEF(env, groupIdString, "");
    tx_data_struct->base_v1.base.group_id = strdup(groupIdValue);

    // nonce
    jfieldID nonceField = env->GetFieldID(txDataClass, "nonce", "Ljava/lang/String;");
    auto nonceString = (jstring)env->GetObjectField(transactionObject, nonceField);
    const char* nonceValue = GET_J_STRING_CONTENT_DEF(env, nonceString, "");
    tx_data_struct->base_v1.base.nonce = strdup(nonceValue);

    // to
    jfieldID toField = env->GetFieldID(txDataClass, "to", "Ljava/lang/String;");
    auto toString = (jstring)env->GetObjectField(transactionObject, toField);
    const char* toValue = GET_J_STRING_CONTENT_DEF(env, toString, "");
    tx_data_struct->base_v1.base.to = strdup(toValue);

    // abi
    jfieldID abiField = env->GetFieldID(txDataClass, "abi", "Ljava/lang/String;");
    auto abiString = (jstring)env->GetObjectField(transactionObject, abiField);
    const char* abiValue = GET_J_STRING_CONTENT_DEF(env, abiString, "");
    tx_data_struct->base_v1.base.abi = strdup(abiValue);

    // input byte[]
    jfieldID inputField = env->GetFieldID(txDataClass, "input", "[B");
    auto inputbytes = (jbyteArray)env->GetObjectField(transactionObject, inputField);
    if (inputbytes == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "exception in input filed.");
        return nullptr;
    }
    // input
    struct bcos_sdk_c_bytes* bytes =
        (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
    jbyte* bufferData = env->GetByteArrayElements(inputbytes, nullptr);
    jsize bufferLength = env->GetArrayLength(inputbytes);
    bytes->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
    memmove(bytes->buffer, bufferData, bufferLength);
    bytes->length = bufferLength;

    tx_data_struct->base_v1.base.input = bytes;

    // value
    jfieldID valueField = env->GetFieldID(txDataClass, "value", "Ljava/lang/String;");
    jstring valueString = (jstring)env->GetObjectField(transactionObject, valueField);
    const char* valueChar = GET_J_STRING_CONTENT_DEF(env, valueString, "");
    tx_data_struct->base_v1.value = strdup(valueChar);

    // gas_price
    jfieldID gasPriceField = env->GetFieldID(txDataClass, "gasPrice", "Ljava/lang/String;");
    jstring gasPriceString = (jstring)env->GetObjectField(transactionObject, gasPriceField);
    const char* gasPriceChar = GET_J_STRING_CONTENT_DEF(env, gasPriceString, "");
    tx_data_struct->base_v1.gas_price = strdup(gasPriceChar);

    // gas_limit
    jfieldID gasLimitField = env->GetFieldID(txDataClass, "gasLimit", "J");
    tx_data_struct->base_v1.gas_limit = env->GetLongField(transactionObject, gasLimitField);

    // max_fee_per_gas
    jfieldID maxFeePerGasField = env->GetFieldID(txDataClass, "maxFeePerGas", "Ljava/lang/String;");
    jstring maxFeePerGasString = (jstring)env->GetObjectField(transactionObject, maxFeePerGasField);
    const char* maxFeePerGasChar = GET_J_STRING_CONTENT_DEF(env, maxFeePerGasString, "");
    tx_data_struct->base_v1.max_fee_per_gas = strdup(maxFeePerGasChar);

    // max_priority_fee_per_gas
    jfieldID maxPriorityFeePerGasField =
        env->GetFieldID(txDataClass, "maxPriorityFeePerGas", "Ljava/lang/String;");
    jstring maxPriorityFeePerGasString =
        (jstring)env->GetObjectField(transactionObject, maxPriorityFeePerGasField);
    const char* maxPriorityFeePerGasChar =
        GET_J_STRING_CONTENT_DEF(env, maxPriorityFeePerGasString, "");
    tx_data_struct->base_v1.max_priority_fee_per_gas = strdup(maxPriorityFeePerGasChar);

    // extension byte[]
    jfieldID extensiontField = env->GetFieldID(txDataClass, "extension", "[B");
    auto extensionbytes = (jbyteArray)env->GetObjectField(transactionObject, extensiontField);
    if (extensionbytes == nullptr)
    {
        struct bcos_sdk_c_bytes* extensionBytes =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        extensionBytes->buffer = nullptr;
        extensionBytes->length = 0;
    }
    else
    {
        struct bcos_sdk_c_bytes* extensionBytes =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        jbyte* extensionBufferData = env->GetByteArrayElements(extensionbytes, nullptr);
        jsize extensionBufferLength = env->GetArrayLength(extensionbytes);
        extensionBytes->buffer = (uint8_t*)malloc(extensionBufferLength * sizeof(uint8_t));
        memmove(extensionBytes->buffer, extensionBufferData, extensionBufferLength);
        extensionBytes->length = extensionBufferLength;
        tx_data_struct->extension = extensionBytes;
        if (extensionbytes)
            env->ReleaseByteArrayElements(extensionbytes, extensionBufferData, 0);
    }


    // release source
    if (chainIdString)
        env->ReleaseStringUTFChars(chainIdString, chainIdValue);
    if (groupIdString)
        env->ReleaseStringUTFChars(groupIdString, groupIdValue);
    if (nonceString)
        env->ReleaseStringUTFChars(nonceString, nonceValue);
    if (toString)
        env->ReleaseStringUTFChars(toString, toValue);
    if (abiString)
        env->ReleaseStringUTFChars(abiString, abiValue);
    if (inputbytes)
        env->ReleaseByteArrayElements(inputbytes, bufferData, 0);
    if (valueString)
        env->ReleaseStringUTFChars(valueString, valueChar);
    if (gasPriceString)
        env->ReleaseStringUTFChars(gasPriceString, gasPriceChar);
    if (maxFeePerGasString)
        env->ReleaseStringUTFChars(maxFeePerGasString, maxFeePerGasChar);
    if (maxPriorityFeePerGasString)
        env->ReleaseStringUTFChars(maxPriorityFeePerGasString, maxPriorityFeePerGasChar);

    return tx_data_struct;
}

// tx data v1: java obj => c struct
struct bcos_sdk_c_transaction_data_v1* convert_to_tx_v1_data_struct(
    JNIEnv* env, jobject transactionObject)
{
    if (transactionObject == NULL)
    {
        THROW_JNI_EXCEPTION(
            env, "exception in convert_to_tx_data_struct, transactionObject is nullptr");
        return NULL;
    }

    jclass txDataClass = env->GetObjectClass(transactionObject);

    bcos_sdk_c_transaction_data_v1* tx_data_struct = (struct bcos_sdk_c_transaction_data_v1*)malloc(
        sizeof(struct bcos_sdk_c_transaction_data_v1));
    // version
    jfieldID versionField = env->GetFieldID(txDataClass, "version", "I");
    tx_data_struct->base.version = env->GetIntField(transactionObject, versionField);

    // blockLimit
    jfieldID blockLimitField = env->GetFieldID(txDataClass, "blockLimit", "J");
    tx_data_struct->base.block_limit = env->GetLongField(transactionObject, blockLimitField);

    // chainId
    jfieldID chainIdField = env->GetFieldID(txDataClass, "chainId", "Ljava/lang/String;");
    jstring chainIdString = (jstring)env->GetObjectField(transactionObject, chainIdField);
    const char* chainIdValue = GET_J_STRING_CONTENT_DEF(env, chainIdString, "");
    tx_data_struct->base.chain_id = strdup(chainIdValue);

    // groupId
    jfieldID groupIdField = env->GetFieldID(txDataClass, "groupId", "Ljava/lang/String;");
    jstring groupIdString = (jstring)env->GetObjectField(transactionObject, groupIdField);
    const char* groupIdValue = GET_J_STRING_CONTENT_DEF(env, groupIdString, "");
    tx_data_struct->base.group_id = strdup(groupIdValue);

    // nonce
    jfieldID nonceField = env->GetFieldID(txDataClass, "nonce", "Ljava/lang/String;");
    jstring nonceString = (jstring)env->GetObjectField(transactionObject, nonceField);
    const char* nonceValue = GET_J_STRING_CONTENT_DEF(env, nonceString, "");
    tx_data_struct->base.nonce = strdup(nonceValue);

    // to
    jfieldID toField = env->GetFieldID(txDataClass, "to", "Ljava/lang/String;");
    jstring toString = (jstring)env->GetObjectField(transactionObject, toField);
    const char* toValue = GET_J_STRING_CONTENT_DEF(env, toString, "");
    tx_data_struct->base.to = strdup(toValue);

    // abi
    jfieldID abiField = env->GetFieldID(txDataClass, "abi", "Ljava/lang/String;");
    jstring abiString = (jstring)env->GetObjectField(transactionObject, abiField);
    const char* abiValue = GET_J_STRING_CONTENT_DEF(env, abiString, "");
    tx_data_struct->base.abi = strdup(abiValue);

    // input byte[]
    jfieldID inputField = env->GetFieldID(txDataClass, "input", "[B");
    jbyteArray inputbytes = (jbyteArray)env->GetObjectField(transactionObject, inputField);
    if (inputbytes == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "exception in input filed.");
        return NULL;
    }
    struct bcos_sdk_c_bytes* bytes =
        (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
    jbyte* bufferData = env->GetByteArrayElements(inputbytes, nullptr);
    jsize bufferLength = env->GetArrayLength(inputbytes);
    bytes->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
    memmove(bytes->buffer, bufferData, bufferLength);
    bytes->length = bufferLength;

    tx_data_struct->base.input = bytes;


    // value
    jfieldID valueField = env->GetFieldID(txDataClass, "value", "Ljava/lang/String;");
    jstring valueString = (jstring)env->GetObjectField(transactionObject, valueField);
    const char* valueChar = GET_J_STRING_CONTENT_DEF(env, valueString, "");
    tx_data_struct->value = strdup(valueChar);

    // gas_price
    jfieldID gasPriceField = env->GetFieldID(txDataClass, "gasPrice", "Ljava/lang/String;");
    jstring gasPriceString = (jstring)env->GetObjectField(transactionObject, gasPriceField);
    const char* gasPriceChar = GET_J_STRING_CONTENT_DEF(env, gasPriceString, "");
    tx_data_struct->gas_price = strdup(gasPriceChar);

    // gas_limit
    jfieldID gasLimitField = env->GetFieldID(txDataClass, "gasLimit", "J");
    tx_data_struct->gas_limit = env->GetLongField(transactionObject, gasLimitField);

    // max_fee_per_gas
    jfieldID maxFeePerGasField = env->GetFieldID(txDataClass, "maxFeePerGas", "Ljava/lang/String;");
    jstring maxFeePerGasString = (jstring)env->GetObjectField(transactionObject, maxFeePerGasField);
    const char* maxFeePerGasChar = GET_J_STRING_CONTENT_DEF(env, maxFeePerGasString, "");
    tx_data_struct->max_fee_per_gas = strdup(maxFeePerGasChar);

    // max_priority_fee_per_gas
    jfieldID maxPriorityFeePerGasField =
        env->GetFieldID(txDataClass, "maxPriorityFeePerGas", "Ljava/lang/String;");
    jstring maxPriorityFeePerGasString =
        (jstring)env->GetObjectField(transactionObject, maxPriorityFeePerGasField);
    const char* maxPriorityFeePerGasChar =
        GET_J_STRING_CONTENT_DEF(env, maxPriorityFeePerGasString, "");
    tx_data_struct->max_priority_fee_per_gas = strdup(maxPriorityFeePerGasChar);

    // release source
    if (chainIdString)
        env->ReleaseStringUTFChars(chainIdString, chainIdValue);
    if (groupIdString)
        env->ReleaseStringUTFChars(groupIdString, groupIdValue);
    if (nonceString)
        env->ReleaseStringUTFChars(nonceString, nonceValue);
    if (toString)
        env->ReleaseStringUTFChars(toString, toValue);
    if (abiString)
        env->ReleaseStringUTFChars(abiString, abiValue);
    if (inputbytes)
        env->ReleaseByteArrayElements(inputbytes, bufferData, 0);
    if (valueString)
        env->ReleaseStringUTFChars(valueString, valueChar);
    if (gasPriceString)
        env->ReleaseStringUTFChars(gasPriceString, gasPriceChar);
    if (maxFeePerGasString)
        env->ReleaseStringUTFChars(maxFeePerGasString, maxFeePerGasChar);
    if (maxPriorityFeePerGasString)
        env->ReleaseStringUTFChars(maxPriorityFeePerGasString, maxPriorityFeePerGasChar);

    return tx_data_struct;
}

// tx data: java obj => c struct
struct bcos_sdk_c_transaction_data* convert_to_tx_data_struct(
    JNIEnv* env, jobject transactionObject)
{
    if (transactionObject == NULL)
    {
        THROW_JNI_EXCEPTION(
            env, "exception in convert_to_tx_data_struct, transactionObject is nullptr");
        return NULL;
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
    const char* chainIdValue = GET_J_STRING_CONTENT_DEF(env, chainIdString, "");
    tx_data_struct->chain_id = strdup(chainIdValue);

    // groupId
    jfieldID groupIdField = env->GetFieldID(txDataClass, "groupId", "Ljava/lang/String;");
    jstring groupIdString = (jstring)env->GetObjectField(transactionObject, groupIdField);
    const char* groupIdValue = GET_J_STRING_CONTENT_DEF(env, groupIdString, "");
    tx_data_struct->group_id = strdup(groupIdValue);

    // nonce
    jfieldID nonceField = env->GetFieldID(txDataClass, "nonce", "Ljava/lang/String;");
    jstring nonceString = (jstring)env->GetObjectField(transactionObject, nonceField);
    const char* nonceValue = GET_J_STRING_CONTENT_DEF(env, nonceString, "");
    tx_data_struct->nonce = strdup(nonceValue);

    // to
    jfieldID toField = env->GetFieldID(txDataClass, "to", "Ljava/lang/String;");
    jstring toString = (jstring)env->GetObjectField(transactionObject, toField);
    const char* toValue = GET_J_STRING_CONTENT_DEF(env, toString, "");
    tx_data_struct->to = strdup(toValue);

    // abi
    jfieldID abiField = env->GetFieldID(txDataClass, "abi", "Ljava/lang/String;");
    jstring abiString = (jstring)env->GetObjectField(transactionObject, abiField);
    const char* abiValue = GET_J_STRING_CONTENT_DEF(env, abiString, "");
    tx_data_struct->abi = strdup(abiValue);

    // input byte[]
    jfieldID inputField = env->GetFieldID(txDataClass, "input", "[B");
    jbyteArray inputbytes = (jbyteArray)env->GetObjectField(transactionObject, inputField);
    if (inputbytes == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "exception in input filed.");
        return NULL;
    }
    struct bcos_sdk_c_bytes* bytes =
        (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
    jbyte* bufferData = env->GetByteArrayElements(inputbytes, nullptr);
    jsize bufferLength = env->GetArrayLength(inputbytes);
    bytes->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
    memmove(bytes->buffer, bufferData, bufferLength);
    bytes->length = bufferLength;

    tx_data_struct->input = bytes;

    // release source
    if (chainIdString)
        env->ReleaseStringUTFChars(chainIdString, chainIdValue);
    if (groupIdString)
        env->ReleaseStringUTFChars(groupIdString, groupIdValue);
    if (nonceString)
        env->ReleaseStringUTFChars(nonceString, nonceValue);
    if (toString)
        env->ReleaseStringUTFChars(toString, toValue);
    if (abiString)
        env->ReleaseStringUTFChars(abiString, abiValue);
    if (inputbytes)
        env->ReleaseByteArrayElements(inputbytes, bufferData, 0);

    return tx_data_struct;
}

// tx data: c struct => java obj
jobject convert_to_tx_data_jobject(
    JNIEnv* env, const struct bcos_sdk_c_transaction_data* transactionData)
{
    if (transactionData == NULL)
    {
        THROW_JNI_EXCEPTION(
            env, "exception in convert_to_tx_data_jobject, transactionData is nullptr");
        return NULL;
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

    if (transactionData->input)
    {
        jfieldID inputField = env->GetFieldID(txDataClass, "input", "[B");
        jbyteArray bufferArray =
            env->NewByteArray(static_cast<jsize>(transactionData->input->length));
        env->SetByteArrayRegion(bufferArray, 0, static_cast<jsize>(transactionData->input->length),
            reinterpret_cast<jbyte*>(transactionData->input->buffer));
        env->SetObjectField(jTxDataObj, inputField, bufferArray);
    }
    return jTxDataObj;
}

// tx data v1: c struct => java obj
jobject convert_to_tx_v1_data_jobject(
    JNIEnv* env, const struct bcos_sdk_c_transaction_data_v1* transactionData)
{
    if (transactionData == NULL)
    {
        THROW_JNI_EXCEPTION(
            env, "exception in convert_to_tx_data_jobject, transactionData is nullptr");
        return NULL;
    }

    jclass txDataClass = env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV1");
    if (txDataClass == NULL)
    {
        env->FatalError(
            "No such class, className: "
            "org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV1");
    }
    jmethodID txDataMtd = env->GetMethodID(txDataClass, "<init>", "()V");
    if (txDataMtd == NULL)
    {
        env->FatalError("No such constructor in TransactionData, constructor()");
    }
    jobject jTxDataObj = env->NewObject(txDataClass, txDataMtd);

    // version
    jfieldID versionField = env->GetFieldID(txDataClass, "version", "I");
    env->SetIntField(jTxDataObj, versionField, transactionData->base.version);
    // block_limit
    jfieldID blockLimitField = env->GetFieldID(txDataClass, "blockLimit", "J");
    env->SetLongField(jTxDataObj, blockLimitField, transactionData->base.block_limit);
    // chain_id
    jfieldID chainIdField = env->GetFieldID(txDataClass, "chainId", "Ljava/lang/String;");
    env->SetObjectField(
        jTxDataObj, chainIdField, env->NewStringUTF(transactionData->base.chain_id));
    // group_id
    jfieldID groupIdField = env->GetFieldID(txDataClass, "groupId", "Ljava/lang/String;");
    env->SetObjectField(
        jTxDataObj, groupIdField, env->NewStringUTF(transactionData->base.group_id));
    // nonce
    jfieldID nonceField = env->GetFieldID(txDataClass, "nonce", "Ljava/lang/String;");
    env->SetObjectField(jTxDataObj, nonceField, env->NewStringUTF(transactionData->base.nonce));
    // to
    jfieldID toField = env->GetFieldID(txDataClass, "to", "Ljava/lang/String;");
    env->SetObjectField(jTxDataObj, toField, env->NewStringUTF(transactionData->base.to));
    // abi
    jfieldID abiField = env->GetFieldID(txDataClass, "abi", "Ljava/lang/String;");
    env->SetObjectField(jTxDataObj, abiField, env->NewStringUTF(transactionData->base.abi));
    // input
    if (transactionData->base.input)
    {
        jfieldID inputField = env->GetFieldID(txDataClass, "input", "[B");
        jbyteArray bufferArray =
            env->NewByteArray(static_cast<jsize>(transactionData->base.input->length));
        env->SetByteArrayRegion(bufferArray, 0,
            static_cast<jsize>(transactionData->base.input->length),
            reinterpret_cast<jbyte*>(transactionData->base.input->buffer));
        env->SetObjectField(jTxDataObj, inputField, bufferArray);
    }

    // value
    jfieldID valueField = env->GetFieldID(txDataClass, "value", "Ljava/lang/String;");
    env->SetObjectField(jTxDataObj, valueField, env->NewStringUTF(transactionData->value));
    // gas_price
    jfieldID gasPriceField = env->GetFieldID(txDataClass, "gasPrice", "Ljava/lang/String;");
    env->SetObjectField(jTxDataObj, gasPriceField, env->NewStringUTF(transactionData->gas_price));
    // gas_limit
    jfieldID gasLimitField = env->GetFieldID(txDataClass, "gasLimit", "J");
    env->SetLongField(jTxDataObj, gasLimitField, transactionData->gas_limit);
    // max_fee_per_gas
    jfieldID maxFeePerGasField = env->GetFieldID(txDataClass, "maxFeePerGas", "Ljava/lang/String;");
    env->SetObjectField(
        jTxDataObj, maxFeePerGasField, env->NewStringUTF(transactionData->max_fee_per_gas));
    // max_priority_fee_per_gas
    jfieldID maxPriorityFeePerGasField =
        env->GetFieldID(txDataClass, "maxPriorityFeePerGas", "Ljava/lang/String;");
    env->SetObjectField(jTxDataObj, maxPriorityFeePerGasField,
        env->NewStringUTF(transactionData->max_priority_fee_per_gas));


    return jTxDataObj;
}

// tx data v2: c struct => java obj
jobject convert_to_tx_v2_data_jobject(
    JNIEnv* env, const struct bcos_sdk_c_transaction_data_v2* transactionData)
{
    if (transactionData == nullptr)
    {
        THROW_JNI_EXCEPTION(
            env, "exception in convert_to_tx_data_jobject, transactionData is nullptr");
        return nullptr;
    }

    jclass txDataClass = env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2");
    if (txDataClass == nullptr)
    {
        env->FatalError(
            "No such class, className: "
            "org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2");
    }
    jmethodID txDataMtd = env->GetMethodID(txDataClass, "<init>", "()V");
    if (txDataMtd == nullptr)
    {
        env->FatalError("No such constructor in TransactionData, constructor()");
    }
    jobject jTxDataObj = env->NewObject(txDataClass, txDataMtd);

    // version
    jfieldID versionField = env->GetFieldID(txDataClass, "version", "I");
    env->SetIntField(jTxDataObj, versionField, transactionData->base_v1.base.version);
    // block_limit
    jfieldID blockLimitField = env->GetFieldID(txDataClass, "blockLimit", "J");
    env->SetLongField(jTxDataObj, blockLimitField, transactionData->base_v1.base.block_limit);
    // chain_id
    jfieldID chainIdField = env->GetFieldID(txDataClass, "chainId", "Ljava/lang/String;");
    env->SetObjectField(
        jTxDataObj, chainIdField, env->NewStringUTF(transactionData->base_v1.base.chain_id));
    // group_id
    jfieldID groupIdField = env->GetFieldID(txDataClass, "groupId", "Ljava/lang/String;");
    env->SetObjectField(
        jTxDataObj, groupIdField, env->NewStringUTF(transactionData->base_v1.base.group_id));
    // nonce
    jfieldID nonceField = env->GetFieldID(txDataClass, "nonce", "Ljava/lang/String;");
    env->SetObjectField(
        jTxDataObj, nonceField, env->NewStringUTF(transactionData->base_v1.base.nonce));
    // to
    jfieldID toField = env->GetFieldID(txDataClass, "to", "Ljava/lang/String;");
    env->SetObjectField(jTxDataObj, toField, env->NewStringUTF(transactionData->base_v1.base.to));
    // abi
    jfieldID abiField = env->GetFieldID(txDataClass, "abi", "Ljava/lang/String;");
    env->SetObjectField(jTxDataObj, abiField, env->NewStringUTF(transactionData->base_v1.base.abi));
    // input
    if (transactionData->base_v1.base.input)
    {
        jfieldID inputField = env->GetFieldID(txDataClass, "input", "[B");
        jbyteArray bufferArray =
            env->NewByteArray(static_cast<jsize>(transactionData->base_v1.base.input->length));
        env->SetByteArrayRegion(bufferArray, 0,
            static_cast<jsize>(transactionData->base_v1.base.input->length),
            reinterpret_cast<jbyte*>(transactionData->base_v1.base.input->buffer));
        env->SetObjectField(jTxDataObj, inputField, bufferArray);
    }

    // value
    jfieldID valueField = env->GetFieldID(txDataClass, "value", "Ljava/lang/String;");
    env->SetObjectField(jTxDataObj, valueField, env->NewStringUTF(transactionData->base_v1.value));
    // gas_price
    jfieldID gasPriceField = env->GetFieldID(txDataClass, "gasPrice", "Ljava/lang/String;");
    env->SetObjectField(
        jTxDataObj, gasPriceField, env->NewStringUTF(transactionData->base_v1.gas_price));
    // gas_limit
    jfieldID gasLimitField = env->GetFieldID(txDataClass, "gasLimit", "J");
    env->SetLongField(jTxDataObj, gasLimitField, transactionData->base_v1.gas_limit);
    // max_fee_per_gas
    jfieldID maxFeePerGasField = env->GetFieldID(txDataClass, "maxFeePerGas", "Ljava/lang/String;");
    env->SetObjectField(
        jTxDataObj, maxFeePerGasField, env->NewStringUTF(transactionData->base_v1.max_fee_per_gas));
    // max_priority_fee_per_gas
    jfieldID maxPriorityFeePerGasField =
        env->GetFieldID(txDataClass, "maxPriorityFeePerGas", "Ljava/lang/String;");
    env->SetObjectField(jTxDataObj, maxPriorityFeePerGasField,
        env->NewStringUTF(transactionData->base_v1.max_priority_fee_per_gas));

    // extension
    if (transactionData->extension)
    {
        jfieldID extensionField = env->GetFieldID(txDataClass, "extension", "[B");
        jbyteArray bufferArray =
            env->NewByteArray(static_cast<jsize>(transactionData->extension->length));
        env->SetByteArrayRegion(bufferArray, 0,
            static_cast<jsize>(transactionData->extension->length),
            reinterpret_cast<jbyte*>(transactionData->extension->buffer));
        env->SetObjectField(jTxDataObj, extensionField, bufferArray);
    }

    return jTxDataObj;
}

// tx: c struct => java obj
jobject convert_to_tx_jobject(JNIEnv* env, const struct bcos_sdk_c_transaction* tx_struct)
{
    if (tx_struct == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_jobject, tx_struct is nullptr");
        return NULL;
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
    jfieldID dataHashField = env->GetFieldID(txClass, "dataHash", "[B");
    jfieldID signatureField = env->GetFieldID(txClass, "signature", "[B");
    jfieldID senderField = env->GetFieldID(txClass, "sender", "[B");
    jfieldID importTimeField = env->GetFieldID(txClass, "importTime", "J");
    jfieldID attributeField = env->GetFieldID(txClass, "attribute", "I");
    jfieldID extraDataField = env->GetFieldID(txClass, "extraData", "Ljava/lang/String;");

    jobject javaTxObj = env->NewObject(txClass, txMtd);
    // TransactionData
    jobject javaTxDataObj = convert_to_tx_data_jobject(env, tx_struct->transaction_data);
    if (javaTxDataObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in TransactionData field.");
        return NULL;
    }
    env->SetObjectField(javaTxObj, txDataField, javaTxDataObj);
    // DataHash byte[]
    {
        if (tx_struct->data_hash != NULL)
        {
            jbyteArray bufferArray =
                env->NewByteArray(static_cast<jsize>(tx_struct->data_hash->length));
            env->SetByteArrayRegion(bufferArray, 0,
                static_cast<jsize>(tx_struct->data_hash->length),
                reinterpret_cast<jbyte*>(tx_struct->data_hash->buffer));
            env->SetObjectField(javaTxObj, dataHashField, bufferArray);
        }
    }
    // Signature byte[]
    {
        if (tx_struct->signature != NULL)
        {
            jbyteArray bufferArray =
                env->NewByteArray(static_cast<jsize>(tx_struct->signature->length));
            env->SetByteArrayRegion(bufferArray, 0,
                static_cast<jsize>(tx_struct->signature->length),
                reinterpret_cast<jbyte*>(tx_struct->signature->buffer));
            env->SetObjectField(javaTxObj, signatureField, bufferArray);
        }
    }
    // Sender byte[]
    {
        if (tx_struct->sender != NULL)
        {
            jbyteArray bufferArray =
                env->NewByteArray(static_cast<jsize>(tx_struct->sender->length));
            env->SetByteArrayRegion(bufferArray, 0, static_cast<jsize>(tx_struct->sender->length),
                reinterpret_cast<jbyte*>(tx_struct->sender->buffer));
            env->SetObjectField(javaTxObj, senderField, bufferArray);
        }
    }
    // ImportTime
    env->SetLongField(javaTxObj, importTimeField, tx_struct->import_time);
    // Attribute
    env->SetIntField(javaTxObj, attributeField, tx_struct->attribute);
    // ExtraData
    jstring javaExtraData = env->NewStringUTF(tx_struct->extra_data);
    env->SetObjectField(javaTxObj, extraDataField, javaExtraData);

    return javaTxObj;
}

// tx v1: c struct => java obj
jobject convert_to_tx_v1_jobject(JNIEnv* env, const struct bcos_sdk_c_transaction_v1* tx_struct)

{
    if (tx_struct == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_jobject, tx_struct is nullptr");
        return NULL;
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
    jfieldID dataHashField = env->GetFieldID(txClass, "dataHash", "[B");
    jfieldID signatureField = env->GetFieldID(txClass, "signature", "[B");
    jfieldID senderField = env->GetFieldID(txClass, "sender", "[B");
    jfieldID importTimeField = env->GetFieldID(txClass, "importTime", "J");
    jfieldID attributeField = env->GetFieldID(txClass, "attribute", "I");
    jfieldID extraDataField = env->GetFieldID(txClass, "extraData", "Ljava/lang/String;");

    jobject javaTxObj = env->NewObject(txClass, txMtd);
    // TransactionData
    jobject javaTxDataObj = convert_to_tx_v1_data_jobject(env, tx_struct->transaction_data);
    if (javaTxDataObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in TransactionData field.");
        return NULL;
    }
    env->SetObjectField(javaTxObj, txDataField, javaTxDataObj);
    // DataHash byte[]
    {
        if (tx_struct->data_hash)
        {
            jbyteArray bufferArray =
                env->NewByteArray(static_cast<jsize>(tx_struct->data_hash->length));
            env->SetByteArrayRegion(bufferArray, 0,
                static_cast<jsize>(tx_struct->data_hash->length),
                reinterpret_cast<jbyte*>(tx_struct->data_hash->buffer));
            env->SetObjectField(javaTxObj, dataHashField, bufferArray);
        }
    }
    // Signature byte[]
    {
        if (tx_struct->signature)
        {
            jbyteArray bufferArray =
                env->NewByteArray(static_cast<jsize>(tx_struct->signature->length));
            env->SetByteArrayRegion(bufferArray, 0,
                static_cast<jsize>(tx_struct->signature->length),
                reinterpret_cast<jbyte*>(tx_struct->signature->buffer));
            env->SetObjectField(javaTxObj, signatureField, bufferArray);
        }
    }
    // Sender byte[]
    {
        if (tx_struct->sender)
        {
            jbyteArray bufferArray =
                env->NewByteArray(static_cast<jsize>(tx_struct->sender->length));
            env->SetByteArrayRegion(bufferArray, 0, static_cast<jsize>(tx_struct->sender->length),
                reinterpret_cast<jbyte*>(tx_struct->sender->buffer));
            env->SetObjectField(javaTxObj, senderField, bufferArray);
        }
    }
    // ImportTime
    env->SetLongField(javaTxObj, importTimeField, tx_struct->import_time);
    // Attribute
    env->SetIntField(javaTxObj, attributeField, tx_struct->attribute);
    // ExtraData
    jstring javaExtraData = env->NewStringUTF(tx_struct->extra_data);
    env->SetObjectField(javaTxObj, extraDataField, javaExtraData);

    return javaTxObj;
}

// tx v2: c struct => java obj
jobject convert_to_tx_v2_jobject(JNIEnv* env, const struct bcos_sdk_c_transaction_v2* tx_struct)

{
    if (tx_struct == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_jobject, tx_struct is nullptr");
        return nullptr;
    }

    jclass txClass = env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/Transaction");
    if (txClass == nullptr)
    {
        env->FatalError(
            "No such class, className: "
            "org/fisco/bcos/sdk/jni/utilities/tx/Transaction");
    }
    jmethodID txMtd = env->GetMethodID(txClass, "<init>", "()V");
    if (txMtd == nullptr)
    {
        env->FatalError("No such constructor in Transaction, constructor()");
    }
    jfieldID txDataField = env->GetFieldID(
        txClass, "transactionData", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;");
    jfieldID dataHashField = env->GetFieldID(txClass, "dataHash", "[B");
    jfieldID signatureField = env->GetFieldID(txClass, "signature", "[B");
    jfieldID senderField = env->GetFieldID(txClass, "sender", "[B");
    jfieldID importTimeField = env->GetFieldID(txClass, "importTime", "J");
    jfieldID attributeField = env->GetFieldID(txClass, "attribute", "I");
    jfieldID extraDataField = env->GetFieldID(txClass, "extraData", "Ljava/lang/String;");

    jobject javaTxObj = env->NewObject(txClass, txMtd);
    // TransactionData
    jobject javaTxDataObj = convert_to_tx_v2_data_jobject(env, tx_struct->transaction_data);
    if (javaTxDataObj == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "exception in TransactionData field.");
        return nullptr;
    }
    env->SetObjectField(javaTxObj, txDataField, javaTxDataObj);
    // DataHash byte[]
    {
        if (tx_struct->data_hash)
        {
            jbyteArray bufferArray =
                env->NewByteArray(static_cast<jsize>(tx_struct->data_hash->length));
            env->SetByteArrayRegion(bufferArray, 0,
                static_cast<jsize>(tx_struct->data_hash->length),
                reinterpret_cast<jbyte*>(tx_struct->data_hash->buffer));
            env->SetObjectField(javaTxObj, dataHashField, bufferArray);
        }
    }
    // Signature byte[]
    {
        if (tx_struct->signature)
        {
            jbyteArray bufferArray =
                env->NewByteArray(static_cast<jsize>(tx_struct->signature->length));
            env->SetByteArrayRegion(bufferArray, 0,
                static_cast<jsize>(tx_struct->signature->length),
                reinterpret_cast<jbyte*>(tx_struct->signature->buffer));
            env->SetObjectField(javaTxObj, signatureField, bufferArray);
        }
    }
    // Sender byte[]
    {
        if (tx_struct->sender)
        {
            jbyteArray bufferArray =
                env->NewByteArray(static_cast<jsize>(tx_struct->sender->length));
            env->SetByteArrayRegion(bufferArray, 0, static_cast<jsize>(tx_struct->sender->length),
                reinterpret_cast<jbyte*>(tx_struct->sender->buffer));
            env->SetObjectField(javaTxObj, senderField, bufferArray);
        }
    }
    // ImportTime
    env->SetLongField(javaTxObj, importTimeField, tx_struct->import_time);
    // Attribute
    env->SetIntField(javaTxObj, attributeField, tx_struct->attribute);
    // ExtraData
    jstring javaExtraData = env->NewStringUTF(tx_struct->extra_data);
    env->SetObjectField(javaTxObj, extraDataField, javaExtraData);

    return javaTxObj;
}

// tx: java obj => c struct
struct bcos_sdk_c_transaction* convert_to_tx_struct(JNIEnv* env, jobject jTxObj)
{
    if (jTxObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct, Transaction is nullptr");
        return NULL;
    }

    jclass javaTxClass = env->GetObjectClass(jTxObj);
    jfieldID transactionDataField = env->GetFieldID(
        javaTxClass, "transactionData", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;");
    jfieldID dataHashField = env->GetFieldID(javaTxClass, "dataHash", "[B");
    jfieldID signatureField = env->GetFieldID(javaTxClass, "signature", "[B");
    jfieldID senderField = env->GetFieldID(javaTxClass, "sender", "[B");
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
        THROW_JNI_EXCEPTION(env, "exception in txDataStruct field.");
        return NULL;
    }
    txStruct->transaction_data = txDataStruct;
    // DataHash byte[]
    {
        jbyteArray dataHashBytes = (jbyteArray)env->GetObjectField(jTxObj, dataHashField);
        if (dataHashBytes == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in DataHash field.");
            return NULL;
        }
        struct bcos_sdk_c_bytes* dataHash =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        jbyte* bufferData = env->GetByteArrayElements(dataHashBytes, nullptr);
        jsize bufferLength = env->GetArrayLength(dataHashBytes);
        dataHash->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
        memmove(dataHash->buffer, bufferData, bufferLength);
        dataHash->length = bufferLength;

        // release temp dataHashBytes
        env->ReleaseByteArrayElements(dataHashBytes, bufferData, 0);
        txStruct->data_hash = dataHash;
    }

    // Signature byte[]
    {
        jbyteArray signatureBytes = (jbyteArray)env->GetObjectField(jTxObj, signatureField);
        if (signatureBytes == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in Signature field.");
            return NULL;
        }
        struct bcos_sdk_c_bytes* signature =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        jbyte* bufferData = env->GetByteArrayElements(signatureBytes, nullptr);
        jsize bufferLength = env->GetArrayLength(signatureBytes);
        signature->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
        memmove(signature->buffer, bufferData, bufferLength);
        signature->length = bufferLength;

        // release temp signatureBytes
        env->ReleaseByteArrayElements(signatureBytes, bufferData, 0);
        txStruct->signature = signature;
    }
    // Sender byte[]
    {
        jbyteArray senderBytes = (jbyteArray)env->GetObjectField(jTxObj, senderField);
        if (senderBytes == NULL)
        {
            // sender can be null
            txStruct->sender = NULL;
        }
        else
        {
            struct bcos_sdk_c_bytes* sender =
                (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
            jbyte* bufferData = env->GetByteArrayElements(senderBytes, nullptr);
            jsize bufferLength = env->GetArrayLength(senderBytes);
            sender->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
            memmove(sender->buffer, bufferData, bufferLength);
            sender->length = bufferLength;

            // release temp senderBytes
            env->ReleaseByteArrayElements(senderBytes, bufferData, 0);
            txStruct->sender = sender;
        }
    }
    // ImportTime
    jlong importTimeValue = env->GetLongField(jTxObj, importTimeField);
    txStruct->import_time = (int64_t)importTimeValue;
    // Attribute
    jint attributeValue = env->GetIntField(jTxObj, attributeField);
    txStruct->attribute = (int32_t)attributeValue;
    // ExtraData
    jstring javaExtraData = (jstring)env->GetObjectField(jTxObj, extraDataField);
    const char* extraDataValue = GET_J_STRING_CONTENT_DEF(env, javaExtraData, "");
    txStruct->extra_data = strdup(extraDataValue);

    env->ReleaseStringUTFChars(javaExtraData, extraDataValue);

    return txStruct;
}

// tx v1: java obj => c struct
struct bcos_sdk_c_transaction_v1* convert_to_tx_v1_struct(JNIEnv* env, jobject jTxObj)
{
    if (jTxObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct, Transaction is nullptr");
        return NULL;
    }

    jclass javaTxClass = env->GetObjectClass(jTxObj);
    jfieldID transactionDataField = env->GetFieldID(
        javaTxClass, "transactionData", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;");
    jfieldID dataHashField = env->GetFieldID(javaTxClass, "dataHash", "[B");
    jfieldID signatureField = env->GetFieldID(javaTxClass, "signature", "[B");
    jfieldID senderField = env->GetFieldID(javaTxClass, "sender", "[B");
    jfieldID importTimeField = env->GetFieldID(javaTxClass, "importTime", "J");
    jfieldID attributeField = env->GetFieldID(javaTxClass, "attribute", "I");
    jfieldID extraDataField = env->GetFieldID(javaTxClass, "extraData", "Ljava/lang/String;");

    struct bcos_sdk_c_transaction_v1* txStruct =
        (struct bcos_sdk_c_transaction_v1*)malloc(sizeof(struct bcos_sdk_c_transaction_v1));
    // TransactionData
    jobject javaTxDataObj = env->GetObjectField(jTxObj, transactionDataField);
    struct bcos_sdk_c_transaction_data_v1* txDataStruct =
        convert_to_tx_v1_data_struct(env, javaTxDataObj);
    if (txDataStruct == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in txDataStruct field.");
        return NULL;
    }
    txStruct->transaction_data = txDataStruct;
    // DataHash byte[]
    {
        jbyteArray dataHashBytes = (jbyteArray)env->GetObjectField(jTxObj, dataHashField);
        if (dataHashBytes == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in DataHash field.");
            return NULL;
        }
        struct bcos_sdk_c_bytes* dataHash =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        jbyte* bufferData = env->GetByteArrayElements(dataHashBytes, nullptr);
        jsize bufferLength = env->GetArrayLength(dataHashBytes);
        dataHash->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
        memmove(dataHash->buffer, bufferData, bufferLength);
        dataHash->length = bufferLength;

        // release temp dataHashBytes
        env->ReleaseByteArrayElements(dataHashBytes, bufferData, 0);
        txStruct->data_hash = dataHash;
    }

    // Signature byte[]
    {
        jbyteArray signatureBytes = (jbyteArray)env->GetObjectField(jTxObj, signatureField);
        if (signatureBytes == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in Signature field.");
            return NULL;
        }
        struct bcos_sdk_c_bytes* signature =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        jbyte* bufferData = env->GetByteArrayElements(signatureBytes, nullptr);
        jsize bufferLength = env->GetArrayLength(signatureBytes);
        signature->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
        memmove(signature->buffer, bufferData, bufferLength);
        signature->length = bufferLength;

        // release temp signatureBytes
        env->ReleaseByteArrayElements(signatureBytes, bufferData, 0);
        txStruct->signature = signature;
    }
    // Sender byte[]
    {
        jbyteArray senderBytes = (jbyteArray)env->GetObjectField(jTxObj, senderField);
        if (senderBytes == NULL)
        {
            // sender can be null
            txStruct->sender = NULL;
        }
        else
        {
            struct bcos_sdk_c_bytes* sender =
                (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
            jbyte* bufferData = env->GetByteArrayElements(senderBytes, nullptr);
            jsize bufferLength = env->GetArrayLength(senderBytes);
            sender->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
            memmove(sender->buffer, bufferData, bufferLength);
            sender->length = bufferLength;
            // release temp senderBytes
            env->ReleaseByteArrayElements(senderBytes, bufferData, 0);
            txStruct->sender = sender;
        }
    }
    // ImportTime
    jlong importTimeValue = env->GetLongField(jTxObj, importTimeField);
    txStruct->import_time = (int64_t)importTimeValue;
    // Attribute
    jint attributeValue = env->GetIntField(jTxObj, attributeField);
    txStruct->attribute = (int32_t)attributeValue;
    // ExtraData
    jstring javaExtraData = (jstring)env->GetObjectField(jTxObj, extraDataField);
    const char* extraDataValue = GET_J_STRING_CONTENT_DEF(env, javaExtraData, "");
    txStruct->extra_data = strdup(extraDataValue);

    env->ReleaseStringUTFChars(javaExtraData, extraDataValue);

    return txStruct;
}

// tx v2: java obj => c struct
struct bcos_sdk_c_transaction_v2* convert_to_tx_v2_struct(JNIEnv* env, jobject jTxObj)
{
    if (jTxObj == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct, Transaction is nullptr");
        return nullptr;
    }

    jclass javaTxClass = env->GetObjectClass(jTxObj);
    jfieldID transactionDataField = env->GetFieldID(
        javaTxClass, "transactionData", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;");
    jfieldID dataHashField = env->GetFieldID(javaTxClass, "dataHash", "[B");
    jfieldID signatureField = env->GetFieldID(javaTxClass, "signature", "[B");
    jfieldID senderField = env->GetFieldID(javaTxClass, "sender", "[B");
    jfieldID importTimeField = env->GetFieldID(javaTxClass, "importTime", "J");
    jfieldID attributeField = env->GetFieldID(javaTxClass, "attribute", "I");
    jfieldID extraDataField = env->GetFieldID(javaTxClass, "extraData", "Ljava/lang/String;");

    struct bcos_sdk_c_transaction_v2* txStruct =
        (struct bcos_sdk_c_transaction_v2*)malloc(sizeof(struct bcos_sdk_c_transaction_v2));
    // TransactionData
    jobject javaTxDataObj = env->GetObjectField(jTxObj, transactionDataField);
    struct bcos_sdk_c_transaction_data_v2* txDataStruct =
        convert_to_tx_v2_data_struct(env, javaTxDataObj);
    if (txDataStruct == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "exception in txDataStruct field.");
        return nullptr;
    }
    txStruct->transaction_data = txDataStruct;
    // DataHash byte[]
    {
        auto dataHashBytes = (jbyteArray)env->GetObjectField(jTxObj, dataHashField);
        if (dataHashBytes == nullptr)
        {
            THROW_JNI_EXCEPTION(env, "exception in DataHash field.");
            return nullptr;
        }
        struct bcos_sdk_c_bytes* dataHash =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        jbyte* bufferData = env->GetByteArrayElements(dataHashBytes, nullptr);
        jsize bufferLength = env->GetArrayLength(dataHashBytes);
        dataHash->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
        memmove(dataHash->buffer, bufferData, bufferLength);
        dataHash->length = bufferLength;

        // release temp dataHashBytes
        env->ReleaseByteArrayElements(dataHashBytes, bufferData, 0);
        txStruct->data_hash = dataHash;
    }

    // Signature byte[]
    {
        auto signatureBytes = (jbyteArray)env->GetObjectField(jTxObj, signatureField);
        if (signatureBytes == nullptr)
        {
            THROW_JNI_EXCEPTION(env, "exception in Signature field.");
            return nullptr;
        }
        struct bcos_sdk_c_bytes* signature =
            (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
        jbyte* bufferData = env->GetByteArrayElements(signatureBytes, nullptr);
        jsize bufferLength = env->GetArrayLength(signatureBytes);
        signature->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
        memmove(signature->buffer, bufferData, bufferLength);
        signature->length = bufferLength;

        // release temp signatureBytes
        env->ReleaseByteArrayElements(signatureBytes, bufferData, 0);
        txStruct->signature = signature;
    }
    // Sender byte[]
    {
        auto senderBytes = (jbyteArray)env->GetObjectField(jTxObj, senderField);
        if (senderBytes == nullptr)
        {
            // sender can be null
            txStruct->sender = nullptr;
        }
        else
        {
            struct bcos_sdk_c_bytes* sender =
                (struct bcos_sdk_c_bytes*)malloc(sizeof(struct bcos_sdk_c_bytes));
            jbyte* bufferData = env->GetByteArrayElements(senderBytes, nullptr);
            jsize bufferLength = env->GetArrayLength(senderBytes);
            sender->buffer = (uint8_t*)malloc(bufferLength * sizeof(uint8_t));
            memmove(sender->buffer, bufferData, bufferLength);
            sender->length = bufferLength;
            // release temp senderBytes
            env->ReleaseByteArrayElements(senderBytes, bufferData, 0);
            txStruct->sender = sender;
        }
    }
    // ImportTime
    jlong importTimeValue = env->GetLongField(jTxObj, importTimeField);
    txStruct->import_time = (int64_t)importTimeValue;
    // Attribute
    jint attributeValue = env->GetIntField(jTxObj, attributeField);
    txStruct->attribute = (int32_t)attributeValue;
    // ExtraData
    auto javaExtraData = (jstring)env->GetObjectField(jTxObj, extraDataField);
    const char* extraDataValue = GET_J_STRING_CONTENT_DEF(env, javaExtraData, "");
    txStruct->extra_data = strdup(extraDataValue);

    env->ReleaseStringUTFChars(javaExtraData, extraDataValue);

    return txStruct;
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
    if (jTransactionDataObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "transactionDataObject is nullptr");
        return NULL;
    }
    const char* tx_data_hex = nullptr;
    if (env->IsInstanceOf(jTransactionDataObj,
            env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV1")) == JNI_TRUE)
    {
        bcos_sdk_c_transaction_data_v1* tx_data_struct =
            convert_to_tx_v1_data_struct(env, jTransactionDataObj);
        if (tx_data_struct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
            return nullptr;
        }
        tx_data_hex = bcos_sdk_encode_transaction_data_struct_to_hex_v1(tx_data_struct);
        if (tx_data_struct)
        {
            bcos_sdk_destroy_transaction_data_struct_v1(tx_data_struct);
        }
    }
    else
    {
        struct bcos_sdk_c_transaction_data* tx_data_struct =
            convert_to_tx_data_struct(env, jTransactionDataObj);
        if (tx_data_struct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
            return nullptr;
        }
        tx_data_hex = bcos_sdk_encode_transaction_data_struct_to_hex(tx_data_struct);
        if (tx_data_struct)
        {
            bcos_sdk_destroy_transaction_data_struct(tx_data_struct);
        }
    }
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
        return nullptr;
    }

    jstring jTxDataHex = env->NewStringUTF(tx_data_hex);

    if (tx_data_hex)
    {
        free((void*)tx_data_hex);
        tx_data_hex = NULL;
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
    if (jTransactionDataObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "transactionDataObject is nullptr");
        return NULL;
    }
    const char* tx_data_json = nullptr;
    if (env->IsInstanceOf(jTransactionDataObj,
            env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV1")) == JNI_TRUE)
    {
        bcos_sdk_c_transaction_data_v1* tx_data_struct =
            convert_to_tx_v1_data_struct(env, jTransactionDataObj);
        if (tx_data_struct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
            return nullptr;
        }
        tx_data_json = bcos_sdk_encode_transaction_data_struct_to_json_v1(tx_data_struct);
        if (tx_data_struct)
        {
            bcos_sdk_destroy_transaction_data_struct_v1(tx_data_struct);
        }
    }
    else
    {
        struct bcos_sdk_c_transaction_data* tx_data_struct =
            convert_to_tx_data_struct(env, jTransactionDataObj);
        if (tx_data_struct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
            return nullptr;
        }
        tx_data_json = bcos_sdk_encode_transaction_data_struct_to_json(tx_data_struct);
        if (tx_data_struct)
        {
            bcos_sdk_destroy_transaction_data_struct(tx_data_struct);
        }
    }
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
        return nullptr;
    }

    jstring jTxDataJson = env->NewStringUTF(tx_data_json);

    if (tx_data_json)
    {
        free((void*)tx_data_json);
        tx_data_json = NULL;
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
    CHECK_OBJECT_NOT_NULL(env, jTxDataHexStr, nullptr);
    const char* tx_data_hex_str = env->GetStringUTFChars(jTxDataHexStr, nullptr);
    struct bcos_sdk_c_transaction_data* tx_data_struct =
        bcos_sdk_decode_transaction_data_struct_from_hex(tx_data_hex_str);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
        return nullptr;
    }

    jobject jTxDataObj = convert_to_tx_data_jobject(env, tx_data_struct);
    if (jTxDataObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_jobject");
        return nullptr;
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
 * Method:    decodeTransactionDataStructv1
 * Signature: (Ljava/lang/String;)Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV1;
 */
JNIEXPORT jobject JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj_decodeTransactionDataStructV1(
    JNIEnv* env, jclass, jstring jTxDataHexStr)
{
    CHECK_OBJECT_NOT_NULL(env, jTxDataHexStr, nullptr);

    const char* tx_data_hex_str = env->GetStringUTFChars(jTxDataHexStr, nullptr);
    struct bcos_sdk_c_transaction_data_v1* tx_data_struct_v1 =
        bcos_sdk_decode_transaction_data_struct_from_hex_v1(tx_data_hex_str);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
        return nullptr;
    }

    jobject jTxDataObj = convert_to_tx_v1_data_jobject(env, tx_data_struct_v1);
    if (jTxDataObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_v1_data_jobject");
        return nullptr;
    }

    // release source
    if (tx_data_struct_v1)
    {
        bcos_sdk_destroy_transaction_data_struct_v1(tx_data_struct_v1);
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
    if (jTransactionDataObj == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "transactionDataObject is nullptr");
        return nullptr;
    }
    const char* tx_data_hash = nullptr;
    int crypto_type = jCrytpTyte;
    if (env->IsInstanceOf(jTransactionDataObj,
            env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2")) == JNI_TRUE)
    {
        bcos_sdk_c_transaction_data_v2* tx_data_struct =
            convert_to_tx_v2_data_struct(env, jTransactionDataObj);
        if (tx_data_struct == nullptr)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
            return nullptr;
        }
        tx_data_hash = bcos_sdk_calc_transaction_data_struct_hash_v2(crypto_type, tx_data_struct);
        bcos_sdk_destroy_transaction_data_struct_v2(tx_data_struct);
    }
    else if (env->IsInstanceOf(jTransactionDataObj,
                 env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV1")) ==
             JNI_TRUE)
    {
        bcos_sdk_c_transaction_data_v1* tx_data_struct =
            convert_to_tx_v1_data_struct(env, jTransactionDataObj);
        if (tx_data_struct == nullptr)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
            return nullptr;
        }
        tx_data_hash = bcos_sdk_calc_transaction_data_struct_hash_v1(crypto_type, tx_data_struct);
        bcos_sdk_destroy_transaction_data_struct_v1(tx_data_struct);
    }
    else
    {
        struct bcos_sdk_c_transaction_data* tx_data_struct =
            convert_to_tx_data_struct(env, jTransactionDataObj);
        if (tx_data_struct == nullptr)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
            return nullptr;
        }
        tx_data_hash = bcos_sdk_calc_transaction_data_struct_hash(crypto_type, tx_data_struct);
        bcos_sdk_destroy_transaction_data_struct(tx_data_struct);
    }
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
        return nullptr;
    }

    jstring jTxDataHash = env->NewStringUTF(tx_data_hash);

    // release source
    if (tx_data_hash)
    {
        free((void*)tx_data_hash);
        tx_data_hash = nullptr;
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
    CHECK_OBJECT_NOT_NULL(env, jTxDataObj, nullptr);
    CHECK_OBJECT_NOT_NULL(env, jSignature, nullptr);
    CHECK_OBJECT_NOT_NULL(env, jTxDataHash, nullptr);
    CHECK_OBJECT_NOT_NULL(env, jExtraData, nullptr);

    const char* tx_str = nullptr;
    const char* signature = env->GetStringUTFChars(jSignature, nullptr);
    const char* tx_data_hash = env->GetStringUTFChars(jTxDataHash, nullptr);
    int attribute = jAttribute;
    const char* extra_data = env->GetStringUTFChars(jExtraData, nullptr);
    if (env->IsInstanceOf(jTxDataObj,
            env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2")) == JNI_TRUE)
    {
        bcos_sdk_c_transaction_data_v2* tx_data_struct =
            convert_to_tx_v2_data_struct(env, jTxDataObj);
        if (tx_data_struct == nullptr)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
            return nullptr;
        }
        tx_str = bcos_sdk_create_encoded_transaction_v2(
            tx_data_struct, signature, tx_data_hash, attribute, extra_data);
        bcos_sdk_destroy_transaction_data_struct_v2(tx_data_struct);
    }
    else if (env->IsInstanceOf(jTxDataObj,
                 env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV1")) ==
             JNI_TRUE)
    {
        bcos_sdk_c_transaction_data_v1* tx_data_struct =
            convert_to_tx_v1_data_struct(env, jTxDataObj);
        if (tx_data_struct == nullptr)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
            return nullptr;
        }
        tx_str = bcos_sdk_create_encoded_transaction_v1(
            tx_data_struct, signature, tx_data_hash, attribute, extra_data);
        bcos_sdk_destroy_transaction_data_struct_v1(tx_data_struct);
    }
    else
    {
        struct bcos_sdk_c_transaction_data* tx_data_struct =
            convert_to_tx_data_struct(env, jTxDataObj);
        if (tx_data_struct == nullptr)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_data_struct");
            return nullptr;
        }
        tx_str = bcos_sdk_create_encoded_transaction(
            tx_data_struct, signature, tx_data_hash, attribute, extra_data);
        bcos_sdk_destroy_transaction_data_struct(tx_data_struct);
    }


    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
        return nullptr;
    }

    jstring jTxStr = env->NewStringUTF(tx_str);

    // release resource
    env->ReleaseStringUTFChars(jSignature, signature);
    env->ReleaseStringUTFChars(jTxDataHash, tx_data_hash);
    env->ReleaseStringUTFChars(jExtraData, extra_data);
    if (tx_str)
    {
        free((void*)tx_str);
        tx_str = nullptr;
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
    CHECK_OBJECT_NOT_NULL(env, jTransactionObj, nullptr);

    jclass javaTxClass = env->GetObjectClass(jTransactionObj);
    jfieldID transactionDataField = env->GetFieldID(
        javaTxClass, "transactionData", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;");
    jobject javaTxDataObj = env->GetObjectField(jTransactionObj, transactionDataField);
    CHECK_OBJECT_NOT_NULL(env, javaTxDataObj, nullptr);
    const char* tx_hex = nullptr;
    if (env->IsInstanceOf(javaTxDataObj,
            env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV2")) == JNI_TRUE)
    {
        bcos_sdk_c_transaction_v2* tx_struct = convert_to_tx_v2_struct(env, jTransactionObj);
        if (tx_struct == nullptr)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct");
            return nullptr;
        }
        tx_hex = bcos_sdk_encode_transaction_struct_to_hex_v2(tx_struct);
        bcos_sdk_destroy_transaction_struct_v2(tx_struct);
    }
    else if (env->IsInstanceOf(javaTxDataObj,
                 env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV1")) ==
             JNI_TRUE)
    {
        bcos_sdk_c_transaction_v1* tx_struct = convert_to_tx_v1_struct(env, jTransactionObj);
        if (tx_struct == nullptr)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct");
            return nullptr;
        }
        tx_hex = bcos_sdk_encode_transaction_struct_to_hex_v1(tx_struct);
        bcos_sdk_destroy_transaction_struct_v1(tx_struct);
    }
    else
    {
        struct bcos_sdk_c_transaction* tx_struct = convert_to_tx_struct(env, jTransactionObj);
        if (tx_struct == nullptr)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct");
            return nullptr;
        }
        tx_hex = bcos_sdk_encode_transaction_struct_to_hex(tx_struct);
        bcos_sdk_destroy_transaction_struct(tx_struct);
    }
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
        return nullptr;
    }

    jstring jTxHex = env->NewStringUTF(tx_hex);

    // release source
    if (tx_hex)
    {
        free((void*)tx_hex);
        tx_hex = nullptr;
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
    CHECK_OBJECT_NOT_NULL(env, jTransactionObj, nullptr);
    jclass javaTxClass = env->GetObjectClass(jTransactionObj);
    jfieldID transactionDataField = env->GetFieldID(
        javaTxClass, "transactionData", "Lorg/fisco/bcos/sdk/jni/utilities/tx/TransactionData;");
    jobject javaTxDataObj = env->GetObjectField(jTransactionObj, transactionDataField);
    CHECK_OBJECT_NOT_NULL(env, javaTxDataObj, nullptr);
    const char* tx_json = nullptr;
    if (env->IsInstanceOf(javaTxDataObj,
            env->FindClass("org/fisco/bcos/sdk/jni/utilities/tx/TransactionDataV1")) == JNI_TRUE)
    {
        bcos_sdk_c_transaction_v1* tx_struct = convert_to_tx_v1_struct(env, jTransactionObj);
        if (tx_struct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct");
            return nullptr;
        }
        tx_json = bcos_sdk_encode_transaction_struct_to_json_v1(tx_struct);
        if (tx_struct)
        {
            bcos_sdk_destroy_transaction_struct_v1(tx_struct);
        }
    }
    else
    {
        struct bcos_sdk_c_transaction* tx_struct = convert_to_tx_struct(env, jTransactionObj);
        if (tx_struct == NULL)
        {
            THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_struct");
            return nullptr;
        }
        tx_json = bcos_sdk_encode_transaction_struct_to_json(tx_struct);
        if (tx_struct)
        {
            bcos_sdk_destroy_transaction_struct(tx_struct);
        }
    }
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
        return nullptr;
    }

    jstring jTxJson = env->NewStringUTF(tx_json);

    if (tx_json)
    {
        free((void*)tx_json);
        tx_json = NULL;
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
    CHECK_OBJECT_NOT_NULL(env, jTxHexStr, nullptr);

    const char* tx_hex_str = env->GetStringUTFChars(jTxHexStr, nullptr);
    struct bcos_sdk_c_transaction* tx_struct =
        bcos_sdk_decode_transaction_struct_from_hex(tx_hex_str);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
        return nullptr;
    }

    jobject jTxObj = convert_to_tx_jobject(env, tx_struct);
    if (jTxObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_jobject");
        return nullptr;
    }

    // release source
    if (tx_struct)
    {
        bcos_sdk_destroy_transaction_struct(tx_struct);
    }
    env->ReleaseStringUTFChars(jTxHexStr, tx_hex_str);

    return jTxObj;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj
 * Method:    decodeTransactionStructv1
 * Signature: (Ljava/lang/String;)Lorg/fisco/bcos/sdk/jni/utilities/tx/Transactionv1;
 */
JNIEXPORT jobject JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj_decodeTransactionStructV1(
    JNIEnv* env, jclass, jstring jTxHexStr)
{
    CHECK_OBJECT_NOT_NULL(env, jTxHexStr, nullptr);

    const char* tx_hex_str = env->GetStringUTFChars(jTxHexStr, nullptr);
    struct bcos_sdk_c_transaction_v1* tx_struct_v1 =
        bcos_sdk_decode_transaction_struct_from_hex_v1(tx_hex_str);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
        return nullptr;
    }

    jobject jTxObj = convert_to_tx_v1_jobject(env, tx_struct_v1);
    if (jTxObj == NULL)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_v1_jobject");
        return nullptr;
    }

    // release source
    if (tx_struct_v1)
    {
        bcos_sdk_destroy_transaction_struct_v1(tx_struct_v1);
    }
    env->ReleaseStringUTFChars(jTxHexStr, tx_hex_str);

    return jTxObj;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj
 * Method:    decodeTransactionStructV2
 * Signature: (Ljava/lang/String;)Lorg/fisco/bcos/sdk/jni/utilities/tx/Transaction;
 */
JNIEXPORT jobject JNICALL
Java_org_fisco_bcos_sdk_jni_utilities_tx_TransactionStructBuilderJniObj_decodeTransactionStructV2(
    JNIEnv* env, jclass, jstring jTxHexStr)
{
    CHECK_OBJECT_NOT_NULL(env, jTxHexStr, nullptr);

    const char* tx_hex_str = env->GetStringUTFChars(jTxHexStr, nullptr);
    struct bcos_sdk_c_transaction_v2* tx_struct_v2 =
        bcos_sdk_decode_transaction_struct_from_hex_v2(tx_hex_str);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
        return nullptr;
    }

    jobject jTxObj = convert_to_tx_v2_jobject(env, tx_struct_v2);
    if (jTxObj == nullptr)
    {
        THROW_JNI_EXCEPTION(env, "exception in convert_to_tx_v2_jobject");
        return nullptr;
    }

    // release source
    bcos_sdk_destroy_transaction_struct_v2(tx_struct_v2);
    env->ReleaseStringUTFChars(jTxHexStr, tx_hex_str);

    return jTxObj;
}
