
#include <bcos-utilities/Exceptions.h>
#include <jni.h>
#include <string>
/* Header for class common */

#ifndef _Included_org_fisco_bcos_sdk_jni_exception
#define _Included_org_fisco_bcos_sdk_jni_exception
#ifdef __cplusplus
extern "C" {
#endif

#define THROW_JNI_EXCEPTION(_ENV_, _INFO_) ThrowJNIException(_ENV_, __FILE__, __LINE__, _INFO_);

void ThrowJNIException(JNIEnv* env, const char* kpFile, int iLine, const std::string& message);

#ifdef __cplusplus
}
#endif
#endif
