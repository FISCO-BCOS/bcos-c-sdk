#include "org_fisco_bcos_sdk_jni_rpc_RpcJniObj.h"
#include "bcos-c-sdk/bcos_sdk_c.h"
#include "bcos-c-sdk/bcos_sdk_c_common.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_rpc.h"
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
static void on_receive_rpc_response(struct bcos_sdk_c_struct_response* resp)
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
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    start
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_start(JNIEnv* env, jobject self)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);
    bcos_sdk_start(sdk);
    if (bcos_sdk_get_last_error() != 0)
    {
        // throw exception in java
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_stop(JNIEnv* env, jobject self)
{
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    if (rpc)
    {
        bcos_sdk_stop(rpc);
    }
}


/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    genericMethod
 * Signature: (Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL
Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_genericMethod__Ljava_lang_String_2Lorg_fisco_bcos_sdk_jni_rpc_RpcCallback_2(
    JNIEnv* env, jobject self, jstring jdata, jobject jcallback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(jcallback);
    context->jvm = jvm;

    // data
    const char* data = env->GetStringUTFChars(jdata, NULL);

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_generic_method_call(rpc, data, on_receive_rpc_response, context);
    env->ReleaseStringUTFChars(jdata, data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    genericMethod
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL
Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_genericMethod__Ljava_lang_String_2Ljava_lang_String_2Lorg_fisco_bcos_sdk_jni_rpc_RpcCallback_2(
    JNIEnv* env, jobject self, jstring jgroup, jstring jdata, jobject jcallback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(jcallback);
    context->jvm = jvm;

    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // data
    const char* data = env->GetStringUTFChars(jdata, NULL);

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_generic_method_call_to_group(rpc, group, data, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jdata, data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    genericMethod
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL
Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_genericMethod__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Lorg_fisco_bcos_sdk_jni_rpc_RpcCallback_2(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jstring jdata, jobject jcallback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(jcallback);
    context->jvm = jvm;

    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);
    // data
    const char* data = env->GetStringUTFChars(jdata, NULL);

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_generic_method_call_to_group_node(
        rpc, group, node, data, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
    env->ReleaseStringUTFChars(jdata, data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    call
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_call(JNIEnv* env, jobject self,
    jstring jgroup, jstring jnode, jstring jto, jstring jdata, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);
    // to
    const char* to = env->GetStringUTFChars(jto, NULL);
    // data
    const char* data = env->GetStringUTFChars(jdata, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_call(rpc, group, node, to, data, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
    env->ReleaseStringUTFChars(jto, to);
    env->ReleaseStringUTFChars(jdata, data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    sendTransaction
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZLorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_sendTransaction(JNIEnv* env,
    jobject self, jstring jgroup, jstring jnode, jstring jdata, jboolean jproof, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);
    // data
    const char* data = env->GetStringUTFChars(jdata, NULL);
    // proof
    int proof = (jproof == JNI_TRUE ? 1 : 0);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_send_transaction(rpc, group, node, data, proof, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
    env->ReleaseStringUTFChars(jdata, data);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getTransaction
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZLorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getTransaction(JNIEnv* env,
    jobject self, jstring jgroup, jstring jnode, jstring jtx_hash, jboolean jproof,
    jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);
    // data
    const char* tx_hash = env->GetStringUTFChars(jtx_hash, NULL);
    // proof
    int proof = (jproof == JNI_TRUE ? 1 : 0);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_transaction(rpc, group, node, tx_hash, proof, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
    env->ReleaseStringUTFChars(jtx_hash, tx_hash);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getTransactionReceipt
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZLorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getTransactionReceipt(JNIEnv* env,
    jobject self, jstring jgroup, jstring jnode, jstring jtx_hash, jboolean jproof,
    jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);
    // data
    const char* tx_hash = env->GetStringUTFChars(jtx_hash, NULL);
    // proof
    int proof = (jproof == JNI_TRUE ? 1 : 0);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_transaction_receipt(
        rpc, group, node, tx_hash, proof, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
    env->ReleaseStringUTFChars(jtx_hash, tx_hash);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getBlockByHash
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ZZLorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getBlockByHash(JNIEnv* env,
    jobject self, jstring jgroup, jstring jnode, jstring jblock_hash, jboolean jonly_header,
    jboolean jonly_txhash, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);
    // data
    const char* block_hash = env->GetStringUTFChars(jblock_hash, NULL);
    // only_header
    int only_header = (jonly_header == JNI_TRUE ? 1 : 0);
    // only_txhash
    int only_txhash = (jonly_txhash == JNI_TRUE ? 1 : 0);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_block_by_hash(
        rpc, group, node, block_hash, only_header, only_txhash, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jblock_hash, block_hash);
    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getBlockByNumber
 * Signature: (Ljava/lang/String;Ljava/lang/String;JZZLorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getBlockByNumber(JNIEnv* env,
    jobject self, jstring jgroup, jstring jnode, jlong jnumber, jboolean jonly_header,
    jboolean jonly_txhash, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);
    // block number
    long block_number = static_cast<long>(jnumber);
    // only_header
    int only_header = (jonly_header == JNI_TRUE ? 1 : 0);
    // only_txhash
    int only_txhash = (jonly_txhash == JNI_TRUE ? 1 : 0);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_block_by_number(
        rpc, group, node, block_number, only_header, only_txhash, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getBlockHashByNumber
 * Signature: (Ljava/lang/String;Ljava/lang/String;JLorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getBlockHashByNumber(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jlong jnumber, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    // block number
    long block_number = static_cast<long>(jnumber);
    bcos_rpc_get_block_hash_by_number(
        rpc, group, node, block_number, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getBlockLimit
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getBlockLimit(
    JNIEnv* env, jobject self, jstring jgroup)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);

    int64_t blk = bcos_rpc_get_block_limit(rpc, group);
    env->ReleaseStringUTFChars(jgroup, group);
    return (jlong)blk;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getBlockNumber
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getBlockNumber(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_block_number(rpc, group, node, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getCode
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getCode(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jstring jaddress, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);
    // data
    const char* address = env->GetStringUTFChars(jaddress, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_code(rpc, group, node, address, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jaddress, address);
    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getSealerList
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getSealerList(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_sealer_list(rpc, group, node, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getObserverList
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getObserverList(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_observer_list(rpc, group, node, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getPendingTxSize
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getPbftView(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);
    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_pbft_view(rpc, group, node, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getPendingTxSize
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getPendingTxSize(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_pending_tx_size(rpc, group, node, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getSyncStatus
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getSyncStatus(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_sync_status(rpc, group, node, on_receive_rpc_response, context);
    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getSystemConfigByKey
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getSystemConfigByKey(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jstring jkey, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);
    // data
    const char* key = env->GetStringUTFChars(jkey, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_system_config_by_key(rpc, group, node, key, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jkey, key);
    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getTotalTransactionCount
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getTotalTransactionCount(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_total_transaction_count(rpc, group, node, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
    env->ReleaseStringUTFChars(jnode, node);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getGroupPeers
 * Signature: (Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getGroupPeers(
    JNIEnv* env, jobject self, jstring jgroup, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_group_peers(rpc, group, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getPeers
 * Signature: (Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getPeers(
    JNIEnv* env, jobject self, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_peers(rpc, on_receive_rpc_response, context);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getGroupList
 * Signature: (Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getGroupList(
    JNIEnv* env, jobject self, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_group_list(rpc, on_receive_rpc_response, context);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getGroupInfo
 * Signature: (Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getGroupInfo(
    JNIEnv* env, jobject self, jstring jgroup, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_group_info(rpc, group, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jgroup, group);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getGroupInfoList
 * Signature: (Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getGroupInfoList(
    JNIEnv* env, jobject self, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_group_info_list(rpc, on_receive_rpc_response, context);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_rpc_RpcJniObj
 * Method:    getGroupNodeInfo
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/rpc/RpcCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_rpc_RpcJniObj_getGroupNodeInfo(
    JNIEnv* env, jobject self, jstring jgroup, jstring jnode, jobject callback)
{
    // rpc obj handler
    void* rpc = bcos_sdk_get_native_pointer(env, self);
    // group
    const char* group = env->GetStringUTFChars(jgroup, NULL);
    // node
    const char* node = env->GetStringUTFChars(jnode, NULL);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(callback);
    context->jvm = jvm;

    const char* respClassName = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, respClassName);

    bcos_rpc_get_group_node_info(rpc, group, node, on_receive_rpc_response, context);

    env->ReleaseStringUTFChars(jnode, node);
    env->ReleaseStringUTFChars(jgroup, group);
}