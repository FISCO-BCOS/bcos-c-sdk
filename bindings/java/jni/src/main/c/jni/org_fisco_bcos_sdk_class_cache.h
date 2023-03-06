
#include <jni.h>
#include <boost/thread/thread.hpp>
#include <string>
#include <unordered_map>


#ifndef _Included_org_fisco_bcos_sdk_class_cache
#define _Included_org_fisco_bcos_sdk_class_cache

class JClassCache
{
private:
    mutable boost::shared_mutex x_classCache;
    std::unordered_map<std::string, jclass> m_classCache;

public:
    jclass findClass(JNIEnv* env, const std::string& _fullClassName);
};

#endif
