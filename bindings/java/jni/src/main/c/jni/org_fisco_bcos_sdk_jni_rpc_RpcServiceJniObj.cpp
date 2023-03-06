#include "org_fisco_bcos_sdk_jni_rpc_RpcServiceJniObj.h"
#include "bcos-c-sdk/bcos_sdk_c.h"
#include "bcos-c-sdk/bcos_sdk_c_common.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_rpc.h"
#include "bcos-c-sdk/bcos_sdk_c_rpc_service.h"
#include "jni/org_fisco_bcos_sdk_common.h"
#include "org_fisco_bcos_sdk_class_cache.h"
#include "org_fisco_bcos_sdk_exception.h"
#include <stdint.h>
#include <stdio.h>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>

// receive rpc response and call the cb
static void on_receive_tars_rpc_response(struct bcos_sdk_c_struct_response* resp)
{
    cb_context* context = (cb_context*)resp->context;

    jobject jcallback = context->jcallback;
    JavaVM* jvm = context->jvm;
    // Note: delete cb_context
    delete context;

    JNIEnv* env;
    jvm->AttachCurrentThread((void**)&env, NULL);

    std::string className = "org/fisco/bcos/sdk/jni/common/Response";
    std::string onRespSig = "(Lorg/fisco/bcos/sdk/jni/common/Response;)V";

    jclass cbClass = env->GetObjectClass(jcallback);
    // void onResponse(Response)
    jmethodID onRespMethodID = env->GetMethodID(cbClass, "onResponse", onRespSig.c_str());
    if (onRespMethodID == NULL)
    {
        env->FatalError(
            ("No such method in the class, className: " + className + " ,method: onResponse")
                .c_str());
    }

    int error = resp->error;
    char* desc = resp->desc ? resp->desc : (char*)"";

    jclass responseClass = bcos_sdk_c_find_jclass(env, className.c_str());

    jmethodID mid = env->GetMethodID(responseClass, "<init>", "()V");
    if (mid == NULL)
    {
        env->FatalError(("No constructor in the class, className: " + className).c_str());
    }

    jobject responseObj = env->NewObject(responseClass, mid);

    // errorCode
    jfieldID errorCodeFieldID = env->GetFieldID(responseClass, "errorCode", "I");
    if (errorCodeFieldID == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + " ,fieldName: errorCode")
                .c_str());
    }

    // errorMessage
    jfieldID errorMsgFieldID = env->GetFieldID(responseClass, "errorMessage", "Ljava/lang/String;");
    if (errorMsgFieldID == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + " ,fieldName: errorMessage")
                .c_str());
    }

    // byte[] data
    jfieldID dataFieldID = env->GetFieldID(responseClass, "data", "[B");
    if (errorMsgFieldID == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + " ,fieldName: data").c_str());
    }

    jstring errorString = env->NewStringUTF(desc);
    env->SetLongField(responseObj, errorCodeFieldID, jint(error));
    env->SetObjectField(responseObj, errorMsgFieldID, errorString);

    jbyteArray byteArrayObj = env->NewByteArray(resp->size);
    if (resp->size > 0)
    {
        jbyte* data = (jbyte*)resp->data;
        env->SetByteArrayRegion(byteArrayObj, 0, resp->size, data);
    }
    env->SetObjectField(responseObj, dataFieldID, byteArrayObj);

    env->CallObjectMethod(jcallback, onRespMethodID, responseObj);

    env->DeleteLocalRef(errorString);
    env->DeleteLocalRef(byteArrayObj);
    env->DeleteLocalRef(responseObj);

    // release callback global reference
    env->DeleteGlobalRef(jcallback);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcServiceJniObj
 * Method:    sendTransaction
 * Signature:
 * (JJLjava/lang/String;Ljava/lang/String;Ljava/lang/String;[BLjava/lang/String;ILjava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcServiceJniObj_sendTransaction(
    JNIEnv* env, jclass, jlong jsdk, jlong jkeypair, jstring jgroup, jstring jnode, jstring jto,
    jbyteArray jdata, jstring jabi, jint jatti, jstring jextra_data, jobject jcallback)
{
    void* sdk = reinterpret_cast<void*>(jsdk);
    void* keypair = reinterpret_cast<void*>(jkeypair);
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    const char* node = env->GetStringUTFChars(jnode, NULL);
    const char* to = env->GetStringUTFChars(jto, NULL);
    jbyte* data = (jbyte*)env->GetByteArrayElements(jdata, 0);
    jsize len = env->GetArrayLength(jdata);
    const char* abi = env->GetStringUTFChars(jabi, NULL);
    int attr = jatti;
    const char* extra_data = env->GetStringUTFChars(jextra_data, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(jcallback);
    context->jvm = jvm;

    const char* tx_hash = bcos_rpc_service_send_transaction(sdk, keypair, group, node, to, data,
        len, abi, attr, extra_data, on_receive_tars_rpc_response, context);

    // // release resoure
    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
    env->ReleaseStringUTFChars(jto, to);
    env->ReleaseStringUTFChars(jabi, abi);
    env->ReleaseStringUTFChars(jextra_data, extra_data);

    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    jstring jtx_hash = env->NewStringUTF(tx_hash);
    if (tx_hash)
    {
        free((void*)tx_hash);
        tx_hash = NULL;
    }

    return jtx_hash;
}
