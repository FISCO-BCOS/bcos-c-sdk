#include "org_fisco_bcos_sdk_exception.h"
#include <iostream>
#include <string>

void ThrowJNIException(JNIEnv* env, const char* kpFile, int iLine, const std::string& message)
{
    const char* className = "org/fisco/bcos/sdk/jni/common/JniException";
    (void)kpFile;
    (void)iLine;

    // Find the exception class.
    jclass tClass = env->FindClass(className);
    if (env->ExceptionCheck())
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        std::cerr << " Not found exception class: " << className << std::endl;
        tClass = env->FindClass("java/lang/Exception");
        if (tClass == NULL)
        {
            std::cerr << " Not found exception class: "
                      << "java/lang/Exception" << std::endl;
            env->DeleteLocalRef(tClass);
            return;
        }
    }

    // Throw the exception with error info
    env->ThrowNew(tClass, message.c_str());
    env->DeleteLocalRef(tClass);
}
