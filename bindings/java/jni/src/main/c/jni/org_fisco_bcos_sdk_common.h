
#include "bcos-c-sdk/bcos_sdk_c_common.h"
#include "org_fisco_bcos_sdk_exception.h"
/* Header for class common */

#ifndef _Included_org_fisco_bcos_sdk_jni_common
#define _Included_org_fisco_bcos_sdk_jni_common
#ifdef __cplusplus
extern "C" {
#endif

struct cb_context
{
    JavaVM* jvm;
    jobject jcallback;
};

void* bcos_sdk_get_native_pointer(JNIEnv* env, jobject self);

jclass bcos_sdk_c_find_jclass(JNIEnv* env, const char* className);

struct bcos_sdk_c_config* create_config_from_java_obj(JNIEnv* env, jobject jconfig);

#ifdef __cplusplus
}
#endif
#endif
