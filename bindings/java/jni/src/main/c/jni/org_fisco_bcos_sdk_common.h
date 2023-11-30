
#include "bcos-c-sdk/bcos_sdk_c_common.h"
#include "org_fisco_bcos_sdk_exception.h"
/* Header for class common */

#ifndef _Included_org_fisco_bcos_sdk_jni_common
#define _Included_org_fisco_bcos_sdk_jni_common
#ifdef __cplusplus
extern "C" {
#endif

#define GET_J_STRING_CONTENT(env, jstr) \
    (env)->GetStringUTFChars((jstr), NULL)
#define GET_J_STRING_CONTENT_DEF(env, jstr, def) \
    ((jstr) == NULL ? (def) : GET_J_STRING_CONTENT((env), (jstr)))

struct cb_context
{
    JavaVM* jvm;
    jobject jcallback;
};

void* bcos_sdk_get_native_pointer(JNIEnv* env, jobject self);

jclass bcos_sdk_c_find_jclass(JNIEnv* env, const char* className);

struct bcos_sdk_c_config* create_config_from_java_obj(JNIEnv* env, jobject jconfig);

// check jstring not null
inline void checkJString(JNIEnv* env, jstring jstr)
{
    if (jstr == NULL)
    {
        THROW_JNI_EXCEPTION(env, "illegal NULL string parameter");
    }
}
// check jByteArray not null
inline void checkJByteArray(JNIEnv* env, jbyteArray jbyte_array)
{
    if (jbyte_array == NULL)
    {
        THROW_JNI_EXCEPTION(env, "illegal NULL byteArray parameter");
    }
}

#ifdef __cplusplus
}
#endif
#endif
