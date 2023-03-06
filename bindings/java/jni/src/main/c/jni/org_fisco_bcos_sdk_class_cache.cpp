
#include "org_fisco_bcos_sdk_class_cache.h"
#include <bcos-utilities/BoostLog.h>
#include <iostream>

jclass JClassCache::findClass(JNIEnv* _env, const std::string& _fullClassName)
{
    // try to fetch jclass from cache
    {
        jclass classId = NULL;
        boost::shared_lock<boost::shared_mutex> lock(x_classCache);
        auto it = m_classCache.find(_fullClassName);
        if (it != m_classCache.end())
        {
            classId = it->second;
            return classId;
        }
    }

    // BCOS_LOG(DEBUG) << LOG_BADGE("findClass") << LOG_DESC("load class")
    //                 << LOG_KV("fullClassName", _fullClassName);

    // find class
    jclass classId = _env->FindClass(_fullClassName.c_str());
    if (classId == NULL)
    {
        _env->FatalError(("No such class, className: " + _fullClassName).c_str());
    }

    classId = (jclass)_env->NewGlobalRef(classId);
    if (classId == NULL)
    {
        _env->FatalError(("Unable to NewGlobalRef, className: " + _fullClassName).c_str());
    }

    // add jclass to cache
    {
        boost::unique_lock<boost::shared_mutex> lock(x_classCache);
        m_classCache[_fullClassName] = classId;
    }

    BCOS_LOG(INFO) << LOG_BADGE("findClass") << LOG_DESC("add class to cache")
                   << LOG_KV("fullClassName", _fullClassName);

    return classId;
}