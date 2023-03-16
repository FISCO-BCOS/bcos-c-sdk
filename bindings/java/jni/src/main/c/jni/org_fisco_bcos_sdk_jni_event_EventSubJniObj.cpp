#include "org_fisco_bcos_sdk_jni_event_EventSubJniObj.h"
#include "bcos-c-sdk/bcos_sdk_c.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_event_sub.h"
#include "jni/org_fisco_bcos_sdk_common.h"
#include <string>

static void on_receive_event_sub_response(struct bcos_sdk_c_struct_response* resp)
{
    cb_context* context = (cb_context*)resp->context;

    jobject jcallback = context->jcallback;
    JavaVM* jvm = context->jvm;
    // Note: delete cb_context
    // delete context;

    JNIEnv* env;
    jvm->AttachCurrentThread((void**)&env, NULL);

    jclass cbClass = env->GetObjectClass(jcallback);

    std::string className = "org/fisco/bcos/sdk/jni/common/Response";
    std::string onRespSig = "(Lorg/fisco/bcos/sdk/jni/common/Response;)V";
    // void onResponse(Response)
    jmethodID onRespMethodID = env->GetMethodID(cbClass, "onResponse", onRespSig.c_str());
    if (onRespMethodID == NULL)
    {
        env->FatalError(("No such method in the class, className: " + className +
                         " ,method: onResponse ,signature: " + onRespSig)
                            .c_str());
    }

    int error = resp->error;
    char* desc = resp->desc ? resp->desc : (char*)"";

    // Response obj construct begin
    jclass responseClass = bcos_sdk_c_find_jclass(env, className.c_str());

    jmethodID mid = env->GetMethodID(responseClass, "<init>", "()V");
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
            ("No such field in the class, className: " + className + " ,filedName: errorMessage")
                .c_str());
    }

    // byte[] data
    jfieldID dataFieldID = env->GetFieldID(responseClass, "data", "[B");
    if (errorMsgFieldID == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + " ,filedName: data").c_str());
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

    //  Note: the callback should not be unreference, it is not
    // once used
    // env->DeleteGlobalRef(jcallback);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_event_EventSubJniObj
 * Method:    start
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_event_EventSubJniObj_start(
    JNIEnv* env, jobject self)
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
 * Class:     org_fisco_bcos_sdk_jni_event_EventSubJniObj
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_event_EventSubJniObj_stop(
    JNIEnv* env, jobject self)
{
    void* event = bcos_sdk_get_native_pointer(env, self);
    if (event)
    {
        bcos_sdk_stop(event);
    }
}

/*
 * Class:     org_fisco_bcos_sdk_jni_event_EventSubJniObj
 * Method:    subscribeEvent
 * Signature:
 * (Ljava/lang/String;Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/event/EventSubscribeCallback;)V
 */
JNIEXPORT jstring JNICALL Java_org_fisco_bcos_sdk_jni_event_EventSubJniObj_subscribeEvent(
    JNIEnv* env, jobject self, jstring jgroup, jstring jparams, jobject jcallback)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);
    const char* group = env->GetStringUTFChars(jgroup, 0);
    const char* params = env->GetStringUTFChars(jparams, 0);

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    cb_context* context = new cb_context();
    context->jcallback = env->NewGlobalRef(jcallback);
    context->jvm = jvm;

    std::string className = "org/fisco/bcos/sdk/jni/common/Response";
    bcos_sdk_c_find_jclass(env, className.c_str());

    const char* eventSubId =
        bcos_event_sub_subscribe_event(sdk, group, params, on_receive_event_sub_response, context);

    // release params
    env->ReleaseStringUTFChars(jparams, params);
    // release group
    env->ReleaseStringUTFChars(jgroup, group);

    jstring result = env->NewStringUTF(eventSubId);
    free((void*)eventSubId);
    return result;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_event_EventSubJniObj
 * Method:    unsubscribeEvent
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_event_EventSubJniObj_unsubscribeEvent(
    JNIEnv* env, jobject self, jstring jeventId)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);
    const char* eventid = env->GetStringUTFChars(jeventId, 0);

    bcos_event_sub_unsubscribe_event(sdk, eventid);

    // release params
    env->ReleaseStringUTFChars(jeventId, eventid);
}

/*
 * Class:     org_fisco_bcos_sdk_jni_event_EventSubJniObj
 * Method:    getAllSubscribedEvents
 * Signature: ()Ljava/util/Set;
 */
JNIEXPORT jobject JNICALL Java_org_fisco_bcos_sdk_jni_event_EventSubJniObj_getAllSubscribedEvents(
    JNIEnv*, jobject)
{
    // TODO:
    return jobject();
}