#include <jni.h>
#include <memory>
#include <thread>


#ifndef _Included_org_fisco_bcos_sdk_jni_thread_attacher
#define _Included_org_fisco_bcos_sdk_jni_thread_attacher

class JNIThreadAttacher
{
public:
    JNIThreadAttacher(JavaVM* jvm) : jvm_(jvm)
    {
        jint result = jvm_->GetEnv(reinterpret_cast<void**>(&env_), JNI_VERSION_1_6);
        if (result == JNI_EDETACHED)
        {
            if (jvm_->AttachCurrentThread(reinterpret_cast<void**>(&env_), NULL) == JNI_OK)
            {
                attached_ = true;
            }
        }
    }

    ~JNIThreadAttacher()
    {
        if (attached_)
        {
            jvm_->DetachCurrentThread();
        }
    }

    JNIEnv* getEnv() const { return env_; }

private:
    JavaVM* jvm_;
    JNIEnv* env_;
    bool attached_ = false;
};

#endif
