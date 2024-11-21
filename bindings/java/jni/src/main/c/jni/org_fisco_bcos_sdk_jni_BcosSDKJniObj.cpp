
#include "jni/org_fisco_bcos_sdk_jni_BcosSDKJniObj.h"
#include "bcos-c-sdk/bcos_sdk_c.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "jni/org_fisco_bcos_sdk_common.h"
#include <string>

/*
 * Class:     org_fisco_bcos_sdk_jni_BcosSDKJniObj
 * Method:    create
 * Signature: (Lorg/fisco/bcos/sdk/jni/common/JniConfig;)J
 */
JNIEXPORT jlong JNICALL Java_org_fisco_bcos_sdk_jni_BcosSDKJniObj_create(
    JNIEnv* env, jclass self, jobject jconfig)
{
    CHECK_OBJECT_NOT_NULL(env, jconfig, 0);
    (void)self;
    // config
    try
    {
        struct bcos_sdk_c_config* config = create_config_from_java_obj(env, jconfig);
        // create sdk obj
        void* sdk = bcos_sdk_create(config);
        // destroy config
        bcos_sdk_c_config_destroy(config);
        if (bcos_sdk_get_last_error() == 0)
        {
            return reinterpret_cast<jlong>(sdk);
        }
    }
    catch (std::exception const& e)
    {
        auto msg = boost::diagnostic_information(e);
        bcos_sdk_set_last_error_msg(-1, msg.c_str());
    }


    // throw exception in java
    THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());

    return 0;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_BcosSDKJniObj
 * Method:    start
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_BcosSDKJniObj_start(JNIEnv* env, jobject self)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);
    if (sdk)
    {
        bcos_sdk_start(sdk);
    }
}

/*
 * Class:     org_fisco_bcos_sdk_jni_BcosSDKJniObj
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_BcosSDKJniObj_stop(JNIEnv* env, jobject self)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);
    if (sdk)
    {
        bcos_sdk_stop(sdk);
    }
}

/*
 * Class:     org_fisco_bcos_sdk_jni_BcosSDKJniObj
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_BcosSDKJniObj_destroy(JNIEnv* env, jobject self)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);
    if (sdk)
    {
        bcos_sdk_destroy(sdk);
    }
}

static void on_receive_block_notifier(const char* group, int64_t block_number, void* ctx)
{
    auto* context = (cb_context*)ctx;

    jobject jcallback = context->jcallback;
    JavaVM* jvm = context->jvm;
    // Note: the context should not be delete for is is not once used
    // delete context;

    JNIEnv* env;
    jvm->AttachCurrentThread((void**)&env, nullptr);

    std::string className = "org/fisco/bcos/sdk/jni/BlockNotifier";

    jclass cbClass = env->GetObjectClass(jcallback);
    // void onResponse(String groupId, BigInteger blockNumber)
    jmethodID onReqMethodID =
        env->GetMethodID(cbClass, "onResponse", "(Ljava/lang/String;Ljava/math/BigInteger;)V");
    if (onReqMethodID == nullptr)
    {
        env->FatalError(
            ("No such method in the class, className: " + className + " ,method: onResponse")
                .c_str());
    }

    std::string bigClassName = "java/math/BigInteger";
    jclass bigClass = bcos_sdk_c_find_jclass(env, bigClassName.c_str());

    jmethodID mid = env->GetMethodID(bigClass, "<init>", "(Ljava/lang/String;)V");
    if (mid == nullptr)
    {
        env->FatalError(("No such constructor method in the class, className: " + bigClassName +
                         " ,method: constructor")
                .c_str());
    }

    std::string strBlk = std::to_string(block_number);
    jstring jblk = env->NewStringUTF(strBlk.c_str());

    jobject jBig = env->NewObject(bigClass, mid, jblk);

    jstring jgroup = env->NewStringUTF(group);

    env->CallObjectMethod(jcallback, onReqMethodID, jgroup, jBig);

    env->DeleteLocalRef(jblk);
    env->DeleteLocalRef(jgroup);
    env->DeleteLocalRef(jBig);

    //  Note: the callback should not be unreference, it is not
    // once used
    // env->DeleteGlobalRef(jcallback);
}


/*
 * Class:     org_fisco_bcos_sdk_jni_BcosSDKJniObj
 * Method:    localProtocolInfo
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_fisco_bcos_sdk_jni_BcosSDKJniObj_localProtocolInfo(
    JNIEnv* env, jobject self)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);
    uint32_t info = bcos_sdk_get_local_protocol_info(sdk);
    return (jint)info;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_BcosSDKJniObj
 * Method:    negotiatedProtocolInfo
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_fisco_bcos_sdk_jni_BcosSDKJniObj_negotiatedProtocolInfo(
    JNIEnv* env, jobject self)
{
    void* sdk = bcos_sdk_get_native_pointer(env, self);
    uint32_t info = bcos_sdk_get_negotiated_protocol_info(sdk);
    return (jint)info;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_BcosSDKJniObj
 * Method:    registerBlockNotifier
 * Signature: (Ljava/lang/String;Lorg/fisco/bcos/sdk/jni/BlockNotifier;)V
 */
JNIEXPORT void JNICALL Java_org_fisco_bcos_sdk_jni_BcosSDKJniObj_registerBlockNotifier(
    JNIEnv* env, jobject self, jstring jgroup, jobject jcallback)
{
    CHECK_OBJECT_NOT_NULL_RET_VOID(env, jgroup);

    void* sdk = bcos_sdk_get_native_pointer(env, self);
    if (!sdk)
    {
        return;
    }

    // Note: The JNIEnv pointer, passed as the first argument to every native method, can only
    // be
    // used in the thread with which it is associated. It is wrong to cache the JNIEnv interface
    // pointer obtained from one thread, and use that pointer in another thread.
    JavaVM* jvm;
    env->GetJavaVM(&jvm);

    auto* context = new cb_context();
    context->jcallback = env->NewGlobalRef(jcallback);
    context->jvm = jvm;

    const char* group = env->GetStringUTFChars(jgroup, nullptr);

    std::string className = "org/fisco/bcos/sdk/jni/BlockNotifier";
    bcos_sdk_c_find_jclass(env, className.c_str());

    std::string bigClassName = "java/math/BigInteger";
    bcos_sdk_c_find_jclass(env, bigClassName.c_str());

    bcos_sdk_register_block_notifier(sdk, group, context, on_receive_block_notifier);

    env->ReleaseStringUTFChars(jgroup, group);
}