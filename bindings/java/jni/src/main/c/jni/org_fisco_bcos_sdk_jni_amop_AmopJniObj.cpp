#include "org_fisco_bcos_sdk_jni_amop_AmopJniObj.h"
#include "bcos-c-sdk/bcos_sdk_c.h"
#include "bcos-c-sdk/bcos_sdk_c_amop.h"
#include "bcos-c-sdk/bcos_sdk_c_common.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "jni/org_fisco_bcos_sdk_common.h"
#include <string.h>
#include <algorithm>
#include <cstdio>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

static void on_receive_amop_request(
    const char* endpoint, const char* seq, struct bcos_sdk_c_struct_response* resp)
{
    cb_context* context = (cb_context*)resp->context;

    jobject jcallback = context->jcallback;
    JavaVM* jvm = context->jvm;
    // Note: the context should not be delete for is is not once used
    // delete context;

    JNIEnv* env;
    jvm->AttachCurrentThread((void**)&env, NULL);

    jclass cbClass = env->GetObjectClass(jcallback);

    std::string className = "org/fisco/bcos/sdk/jni/amop/AmopRequestCallback";
    // onRequest(String endpoint, String seq, byte[] msg)
    jmethodID onReqMethodID =
        env->GetMethodID(cbClass, "onRequest", "(Ljava/lang/String;Ljava/lang/String;[B)V");
    if (onReqMethodID == NULL)
    {
        env->FatalError(
            ("No such method in the class, className: " + className + " ,method: onRequest")
                .c_str());
    }

    jstring jendpoint = env->NewStringUTF(endpoint);
    jstring jseq = env->NewStringUTF(seq);

    // byte[] msg
    jbyteArray byteArrayObj = env->NewByteArray(resp->size);
    if (resp->size > 0)
    {
        jbyte* data = (jbyte*)resp->data;
        env->SetByteArrayRegion(byteArrayObj, 0, resp->size, data);
    }

    env->CallObjectMethod(jcallback, onReqMethodID, jendpoint, jseq, byteArrayObj);

    // Note: the callback should not be unreference, it is not
    // once used

    // env->DeleteGlobalRef(jcallback);
}

static void on_receive_amop_response(struct bcos_sdk_c_struct_response* resp)
{
    cb_context* context = (cb_context*)resp->context;

    jobject jcallback = context->jcallback;
    JavaVM* jvm = context->jvm;
    // Note: the context should not be delete for is is not once used
    // delete context;

    JNIEnv* env;
    jvm->AttachCurrentThread((void**)&env, NULL);

    std::string amopClassName = "org/fisco/bcos/sdk/jni/amop/AmopResponseCallback";

    jclass cbClass = env->GetObjectClass(jcallback);
    // void onResponse(Response)
    jmethodID onReqMethodID =
        env->GetMethodID(cbClass, "onResponse", "(Lorg/fisco/bcos/sdk/jni/common/Response;)V");
    if (onReqMethodID == NULL)
    {
        env->FatalError(
            ("No such method in the class, className: " + amopClassName + " ,method: onResponse")
                .c_str());
    }


    int error = resp->error;
    char* desc = resp->desc ? resp->desc : (char*)"";

    std::string className = "org/fisco/bcos/sdk/jni/common/Response";

    jclass responseClass = bcos_sdk_c_find_jclass(env, className.c_str());

    jmethodID mid = env->GetMethodID(responseClass, "<init>", "()V");
    jobject responseObj = env->NewObject(responseClass, mid);

    // errorCode
    jfieldID errorCodeFieldID = env->GetFieldID(responseClass, "errorCode", "I");
    if (errorCodeFieldID == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + " ,field: errorCode").c_str());
    }

    // errorMessage
    jfieldID errorMsgFieldID = env->GetFieldID(responseClass, "errorMessage", "Ljava/lang/String;");
    if (errorMsgFieldID == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + " ,field: errorMessage")
                .c_str());
    }

    // byte[] data
    jfieldID dataFieldID = env->GetFieldID(responseClass, "data", "[B");
    if (errorMsgFieldID == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + " ,field: data").c_str());
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

    env->CallObjectMethod(jcallback, onReqMethodID, responseObj);

    env->DeleteLocalRef(errorString);
    env->DeleteLocalRef(byteArrayObj);
    env->DeleteLocalRef(responseObj);
    //  Note: the callback should not be unreference, it is not
    // once used
    // env->DeleteGlobalRef(jcallback);
}


/*
 * Class:     org_fisco_bcos_sdk_jni_amop_AmopJniObj
 * Method:    start
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_amop_AmopJniObj_start(JNIEnv* env, jobject self)
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
 * Class:     org_fisco_bcos_sdk_jni_amop_AmopJniObj
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_amop_AmopJniObj_stop(JNIEnv* env, jobject self)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);
    if (sdk)
    {
        bcos_sdk_stop(sdk);
    }
}

/*
 * Class:     org_fisco_bcos_sdk_jni_amop_AmopJniObj
 * Method:    subscribeTopic
 * Signature: (Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/amop/AmopRequestCallback;)V
 */
JNIEXPORT void JNICALL
Java_org_fisco_bcos_sdk_jni_amop_AmopJniObj_subscribeTopic__Ljava_lang_String_2Lorg_fisco_bcos_sdk_jni_amop_AmopRequestCallback_2(
    JNIEnv* env, jobject self, jstring jtopic, jobject jcallback)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);

    const char* topic = env->GetStringUTFChars(jtopic, 0);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(jcallback);
    context->jvm = jvm;

    std::string className = "org/fisco/bcos/sdk/jni/amop/AmopRequestCallback";
    bcos_sdk_c_find_jclass(env, className.c_str());

    bcos_amop_subscribe_topic_with_cb(sdk, topic, on_receive_amop_request, context);

    // release topic
    env->ReleaseStringUTFChars(jtopic, topic);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_amop_AmopJniObj
 * Method:    subscribeTopic
 * Signature: (Ljava/util/Set;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_amop_AmopJniObj_subscribeTopic__Ljava_util_Set_2(
    JNIEnv* env, jobject self, jobject jtopics)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);

    jclass setClass = env->GetObjectClass(jtopics);

    /*
      Use javap to get propper descriptor
      > javap -s -p java.util.Set | grep -A 1 toArray
      public abstract java.lang.Object[] toArray();
      descriptor: ()[Ljava/lang/Object;
    */
    jmethodID toArrayMethodID = env->GetMethodID(setClass, "toArray", "()[Ljava/lang/Object;");

    // Set.toArray() => set array
    jobjectArray arrayOfElements = (jobjectArray)env->CallObjectMethod(jtopics, toArrayMethodID);

    // set array size
    int arraySize = env->GetArrayLength(arrayOfElements);
    if (arraySize > 0)
    {
        char** topics = (char**)malloc(arraySize * sizeof(char*));
        for (int i = 0; i < arraySize; i++)
        {
            jstring stringObj = (jstring)env->GetObjectArrayElement(arrayOfElements, i);
            const char* topic = env->GetStringUTFChars(stringObj, 0);

            // printf(" ==> index: %d, topic: %s\n", i, topic);

            topics[i] = strdup(topic);
            env->ReleaseStringUTFChars(stringObj, topic);
        }

        bcos_amop_subscribe_topic(sdk, topics, arraySize);

        for (int i = 0; i < arraySize; i++)
        {
            free(topics[i]);
        }
    }
}

/*
 * Class:     org_fisco_bcos_sdk_jni_amop_AmopJniObj
 * Method:    subscribeTopic
 * Signature: (Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/amop/AmopRequestCallback;)V
 */
JNIEXPORT void JNICALL
Java_org_fisco_bcos_sdk_jni_amop_AmopJniObj_subscribeTopic__Ljava_lang_String_2Lorg_fisco_bcos_sdk_jni_amop_AmopJniObjRequestCallback_2(
    JNIEnv* env, jobject self, jstring jtopic, jobject jcallback)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);

    const char* topic = env->GetStringUTFChars(jtopic, 0);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(jcallback);
    context->jvm = jvm;

    std::string className = "org/fisco/bcos/sdk/jni/amop/AmopRequestCallback";
    bcos_sdk_c_find_jclass(env, className.c_str());

    bcos_amop_subscribe_topic_with_cb(sdk, topic, on_receive_amop_request, context);

    // release topic
    env->ReleaseStringUTFChars(jtopic, topic);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_amop_AmopJniObj
 * Method:    unsubscribeTopic
 * Signature: (Ljava/util/Set;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_amop_AmopJniObj_unsubscribeTopic(
    JNIEnv* env, jobject self, jobject jtopics)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);

    jclass setClass = env->GetObjectClass(jtopics);

    /*
      Use javap to get propper descriptor
      > javap -s -p java.util.Set | grep -A 1 toArray
      public abstract java.lang.Object[] toArray();
      descriptor: ()[Ljava/lang/Object;
    */
    jmethodID toArrayMethodID = env->GetMethodID(setClass, "toArray", "()[Ljava/lang/Object;");

    // Set.toArray() => set array
    jobjectArray arrayOfElements = (jobjectArray)env->CallObjectMethod(jtopics, toArrayMethodID);

    // set array size
    int arraySize = env->GetArrayLength(arrayOfElements);
    if (arraySize > 0)
    {
        char** topics = (char**)malloc(arraySize * sizeof(char*));
        for (int i = 0; i < arraySize; i++)
        {
            jstring stringObj = (jstring)env->GetObjectArrayElement(arrayOfElements, i);
            const char* topic = env->GetStringUTFChars(stringObj, 0);
            topics[i] = strdup(topic);
            env->ReleaseStringUTFChars(stringObj, topic);
        }

        bcos_amop_unsubscribe_topic(sdk, topics, arraySize);

        for (int i = 0; i < arraySize; i++)
        {
            free(topics[i]);
        }
    }
}

/*
 * Class:     org_fisco_bcos_sdk_jni_amop_AmopJniObj
 * Method:    setCallback
 * Signature: (Lorg/fisco/bcos/sdk/jni/amop/AmopCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_amop_AmopJniObj_setCallback(
    JNIEnv* env, jobject self, jobject jcallback)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(jcallback);
    context->jvm = jvm;

    std::string className = "org/fisco/bcos/sdk/jni/amop/AmopRequestCallback";
    bcos_sdk_c_find_jclass(env, className.c_str());

    bcos_amop_set_subscribe_topic_cb(sdk, on_receive_amop_request, context);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_amop_AmopJniObj
 * Method:    sendAmopMsg
 * Signature: (Ljava/lang/String;[BILorg/fisco/bcos/sdk/jni/amop/AmopResponseCallback;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_amop_AmopJniObj_sendAmopMsg(
    JNIEnv* env, jobject self, jstring jtopic, jbyteArray jdata, jint jtimeout, jobject jcallback)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(jcallback);
    context->jvm = jvm;

    const char* topic = env->GetStringUTFChars(jtopic, 0);

    int timeout = (int)jtimeout;

    jsize len = env->GetArrayLength(jdata);
    jbyte* data = (jbyte*)env->GetByteArrayElements(jdata, 0);

    std::string className = "org/fisco/bcos/sdk/jni/amop/AmopResponseCallback";
    bcos_sdk_c_find_jclass(env, className.c_str());

    bcos_amop_publish(
        sdk, topic, (void*)data, (size_t)len, timeout, on_receive_amop_response, context);
    // release topic
    env->ReleaseStringUTFChars(jtopic, topic);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_amop_AmopJniObj
 * Method:    broadcastAmopMsg
 * Signature: (Ljava/lang/String;[B)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_amop_AmopJniObj_broadcastAmopMsg(
    JNIEnv* env, jobject self, jstring jtopic, jbyteArray jdata)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);

    const char* topic = env->GetStringUTFChars(jtopic, 0);
    jsize len = env->GetArrayLength(jdata);
    jbyte* data = (jbyte*)env->GetByteArrayElements(jdata, 0);

    bcos_amop_broadcast(sdk, topic, (void*)data, (size_t)len);

    // release topic
    env->ReleaseStringUTFChars(jtopic, topic);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_amop_AmopJniObj
 * Method:    sendResponse
 * Signature: (Ljava/lang/String;Ljava/lang/String;[B)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_amop_AmopJniObj_sendResponse(
    JNIEnv* env, jobject self, jstring jendpoint, jstring jseq, jbyteArray jdata)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);

    const char* endpoint = env->GetStringUTFChars(jendpoint, 0);
    const char* seq = env->GetStringUTFChars(jseq, 0);

    jsize len = env->GetArrayLength(jdata);
    jbyte* data = (jbyte*)env->GetByteArrayElements(jdata, 0);

    bcos_amop_send_response(sdk, endpoint, seq, (void*)data, (size_t)len);

    // release endpoint
    env->ReleaseStringUTFChars(jendpoint, endpoint);
    // release seq
    env->ReleaseStringUTFChars(jseq, seq);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_amop_AmopJniObj
 * Method:    getSubTopics
 * Signature: ()Ljava/util/Set;
 */
JNIEXPORT jobject JNICALL Java_org_fisco_bcos_sdk_jni_amop_AmopJniObj_getSubTopics(
    JNIEnv* env, jobject self)
{
    // TODO:

    std::ignore = env;
    std::ignore = self;

    return jobject();
}