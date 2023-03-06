#include "jni/org_fisco_bcos_sdk_common.h"
#include "bcos-c-sdk/bcos_sdk_c_common.h"
#include "org_fisco_bcos_sdk_class_cache.h"
#include "org_fisco_bcos_sdk_exception.h"
#include <bcos-boostssl/websocket/WsTools.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <vector>

using namespace bcos;
using namespace bcos::boostssl;

void* bcos_sdk_get_native_pointer(JNIEnv* env, jobject self)
{
    jclass cls = env->GetObjectClass(self);
    if (cls == NULL)
    {
        env->FatalError("No such class, object class is null in acquire native class");
    }

    jfieldID nativeFieldID = env->GetFieldID(cls, "nativePointer", "J");
    if (nativeFieldID == NULL)
    {
        env->FatalError("No such field, native field is null in acquire native field");
    }

    jlong nativePointer = env->GetLongField(self, nativeFieldID);
    void* native = reinterpret_cast<void*>(nativePointer);
    if (native == NULL)
    {  // Note: native obj can be NULL when it is not initialized ???
        env->FatalError("No such long field, object obj is null in acquire native obj");
    }

    return native;
}

static bcos_sdk_c_cert_config* create_bcos_sdk_c_cert_config(
    JNIEnv* env, jclass certConfigClass, jobject jCertConfig)
{
    /*
    public static class CertConfig {
        private String caCert;
        private String nodeKey;
        private String nodeCert;
    }
    */
    std::string className = "org/fisco/bcos/sdk/jni/common/JniConfig$CertConfig";
    jfieldID caCertField = env->GetFieldID(certConfigClass, "caCert", "Ljava/lang/String;");
    if (caCertField == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + ",fieldName: caCert").c_str());
    }

    jstring jCaCert = (jstring)env->GetObjectField(jCertConfig, caCertField);
    if (!jCaCert)
    {
        BOOST_THROW_EXCEPTION(InvalidParameter()
                              << errinfo_comment("caCert has not been initialized, please set it"));
    }

    const char* caCert = env->GetStringUTFChars(jCaCert, NULL);

    jfieldID nodeKeyField = env->GetFieldID(certConfigClass, "nodeKey", "Ljava/lang/String;");
    if (nodeKeyField == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + ",fieldName: nodeKey")
                .c_str());
    }

    jstring jNodeKey = (jstring)env->GetObjectField(jCertConfig, nodeKeyField);
    if (!jNodeKey)
    {
        BOOST_THROW_EXCEPTION(InvalidParameter() << errinfo_comment(
                                  "node key(nodeKey) has not been initialized, please set it"));
    }

    const char* nodeKey = env->GetStringUTFChars(jNodeKey, NULL);

    jfieldID nodeCertField = env->GetFieldID(certConfigClass, "nodeCert", "Ljava/lang/String;");
    if (nodeCertField == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + ",fieldName: nodeCert")
                .c_str());
    }

    jstring jNodeCert = (jstring)env->GetObjectField(jCertConfig, nodeCertField);
    if (!jNodeKey)
    {
        BOOST_THROW_EXCEPTION(InvalidParameter() << errinfo_comment(
                                  "node cert(nodeCert) has not been initialized, please set it"));
    }

    const char* nodeCert = env->GetStringUTFChars(jNodeCert, NULL);
    struct bcos_sdk_c_cert_config* config =
        (struct bcos_sdk_c_cert_config*)malloc(sizeof(struct bcos_sdk_c_cert_config));

    config->ca_cert = strdup(caCert);
    config->node_cert = strdup(nodeCert);
    config->node_key = strdup(nodeKey);

#if defined(_BCOS_SDK_JNI_DEBUG_)
    printf("[create_bcos_sdk_c_cert_config] ==>>ca cert: %s, node cert: %s, node key: %s \n",
        config->ca_cert, config->node_cert, config->node_key);
#endif

    if (caCert)
    {
        env->ReleaseStringUTFChars(jCaCert, caCert);
    }
    if (nodeKey)
    {
        env->ReleaseStringUTFChars(jNodeKey, nodeKey);
    }
    if (nodeCert)
    {
        env->ReleaseStringUTFChars(jNodeCert, nodeCert);
    }

    return config;
}

static bcos_sdk_c_sm_cert_config* create_bcos_sdk_c_sm_cert_config(
    JNIEnv* env, jclass smCertConfigClass, jobject jSmCertConfig)
{
    /*
    public static class SMCertConfig {
        private String caCert;
        private String nodeCert;
        private String nodeKey;
        private String enNodeCert;
        private String enNodeKey;
    */
    const std::string className = "org/fisco/bcos/sdk/jni/common/JniConfig$SMCertConfig";
    // caCert
    jfieldID caCertField = env->GetFieldID(smCertConfigClass, "caCert", "Ljava/lang/String;");
    if (caCertField == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + ",fieldName: caCert").c_str());
    }

    jstring jCaCert = (jstring)env->GetObjectField(jSmCertConfig, caCertField);
    if (!jCaCert)
    {
        BOOST_THROW_EXCEPTION(InvalidParameter()
                              << errinfo_comment("caCert has not been initialized, please set it"));
    }

    const char* caCert = env->GetStringUTFChars(jCaCert, NULL);

    // nodeCert
    jfieldID nodeCertField = env->GetFieldID(smCertConfigClass, "nodeCert", "Ljava/lang/String;");
    if (nodeCertField == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + ",fieldName: nodeCert")
                .c_str());
    }
    jstring jNodeCert = (jstring)env->GetObjectField(jSmCertConfig, nodeCertField);
    if (!jNodeCert)
    {
        BOOST_THROW_EXCEPTION(InvalidParameter() << errinfo_comment(
                                  "nodeCert has not been initialized, please set it"));
    }

    const char* nodeCert = env->GetStringUTFChars(jNodeCert, NULL);

    // nodeKey
    jfieldID nodeKeyField = env->GetFieldID(smCertConfigClass, "nodeKey", "Ljava/lang/String;");
    if (nodeKeyField == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + ",fieldName: nodeKey")
                .c_str());
    }
    jstring jNodeKey = (jstring)env->GetObjectField(jSmCertConfig, nodeKeyField);
    if (!jNodeKey)
    {
        BOOST_THROW_EXCEPTION(InvalidParameter() << errinfo_comment(
                                  "nodeKey has not been initialized, please set it"));
    }

    const char* nodeKey = jNodeKey ? env->GetStringUTFChars(jNodeKey, NULL) : NULL;

    // enNodeCert
    jfieldID enNodeCertField =
        env->GetFieldID(smCertConfigClass, "enNodeCert", "Ljava/lang/String;");
    if (enNodeCertField == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + ",fieldName: enNodeCert")
                .c_str());
    }
    jstring jEnNodeCert = (jstring)env->GetObjectField(jSmCertConfig, enNodeCertField);
    if (!jEnNodeCert)
    {
        BOOST_THROW_EXCEPTION(
            InvalidParameter() << errinfo_comment(
                "encrypt node cert(enNodeCrt) has not been initialized, please set it"));
    }

    const char* enNodeCert = env->GetStringUTFChars(jEnNodeCert, NULL);

    // enNodeKey
    jfieldID enNodeKeyField = env->GetFieldID(smCertConfigClass, "enNodeKey", "Ljava/lang/String;");
    if (enNodeKeyField == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + ",fieldName: enNodeKey")
                .c_str());
    }
    jstring jEnNodeKey = (jstring)env->GetObjectField(jSmCertConfig, enNodeKeyField);
    if (!jEnNodeKey)
    {
        BOOST_THROW_EXCEPTION(
            InvalidParameter() << errinfo_comment(
                "encrypt node key(enNodeKey) has not been initialized, please set it"));
    }

    const char* enNodeKey = env->GetStringUTFChars(jEnNodeKey, NULL);

    struct bcos_sdk_c_sm_cert_config* config =
        (struct bcos_sdk_c_sm_cert_config*)malloc(sizeof(struct bcos_sdk_c_sm_cert_config));

    config->ca_cert = strdup(caCert);
    config->node_cert = strdup(nodeCert);
    config->node_key = strdup(nodeKey);
    config->en_node_cert = strdup(enNodeCert);
    config->en_node_key = strdup(enNodeKey);

#if defined(_BCOS_SDK_JNI_DEBUG_)
    printf(
        "[create_bcos_sdk_c_sm_cert_config] ==>>ca cert: %s, node cert: %s, node key: %s, en node "
        "cert: %s, en node key:%s \n",
        config->ca_cert, config->node_cert, config->node_key, config->en_node_cert,
        config->en_node_key);
#endif


    env->ReleaseStringUTFChars(jCaCert, caCert);
    env->ReleaseStringUTFChars(jNodeKey, nodeKey);
    env->ReleaseStringUTFChars(jNodeCert, nodeCert);
    env->ReleaseStringUTFChars(jEnNodeKey, enNodeKey);
    env->ReleaseStringUTFChars(jEnNodeCert, enNodeCert);

    return config;
}

struct bcos_sdk_c_config* create_config_from_java_obj(JNIEnv* env, jobject jconfig)
{
    std::string className = "org/fisco/bcos/sdk/jni/common/JniConfig";
    jclass configClass = env->GetObjectClass(jconfig);

    jfieldID threadPoolSizeFieldID = env->GetFieldID(configClass, "threadPoolSize", "I");
    int threadPoolSize = (int)env->GetIntField(jconfig, threadPoolSizeFieldID);

    jfieldID reconnectPeriodMsFieldID = env->GetFieldID(configClass, "reconnectPeriodMs", "I");
    int reconnectPeriodMs = (int)env->GetIntField(jconfig, reconnectPeriodMsFieldID);

    jfieldID heartbeatPeriodMsFieldID = env->GetFieldID(configClass, "heartbeatPeriodMs", "I");
    int heartbeatPeriodMs = (int)env->GetIntField(jconfig, heartbeatPeriodMsFieldID);

    jfieldID messageTimeoutMsFieldID = env->GetFieldID(configClass, "messageTimeoutMs", "I");
    int messageTimeoutMs = (int)env->GetIntField(jconfig, messageTimeoutMsFieldID);

    jfieldID peersFieldID = env->GetFieldID(configClass, "peers", "Ljava/util/List;");
    jobject jpeersOjbect = env->GetObjectField(jconfig, peersFieldID);
    if (jpeersOjbect == NULL)
    {
        BOOST_THROW_EXCEPTION(InvalidParameter() << errinfo_comment(
                                  "the connected peers has not been initialized , please set it"));
    }

    // Find "java/util/List" Class (Standard JAVA Class).
    jclass listClass = env->FindClass("java/util/List");
    if (listClass == NULL)
    {
        env->FatalError("No such class, className: java/util/List");
    }

    // Get "java.util.List.get(int location)" MethodID
    jmethodID listGetMethodID = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");
    if (listGetMethodID == NULL)
    {
        env->FatalError("No such method, className: java.util.List<String>, method: get()");
    }

    // Get "int java.util.List.size()" MethodID
    jmethodID listSizeMethodID = env->GetMethodID(listClass, "size", "()I");
    if (listSizeMethodID == NULL)
    {
        env->FatalError("No such method, className: java.util.List<String>, method: size()");
    }

    // String java.util.List.size()
    int listSize = (int)env->CallIntMethod(jpeersOjbect, listSizeMethodID);

    // Note: ep should be free after bcos_sdk_c_config out of use
    struct bcos_sdk_c_endpoint* ep =
        (struct bcos_sdk_c_endpoint*)malloc(listSize * sizeof(struct bcos_sdk_c_endpoint));

    for (int i = 0; i < listSize; ++i)
    {
        // String java.util.List.get
        jstring jpeer = (jstring)env->CallObjectMethod(jpeersOjbect, listGetMethodID, i);
        if (!jpeer)
        {
            BOOST_THROW_EXCEPTION(InvalidParameter() << errinfo_comment(
                                      "this connected peer is null value, it should be in "
                                      "host<ipv4/ipv6/domain name>:port string format"));
        }

        const char* peer = env->GetStringUTFChars(jpeer, NULL);
        bcos::boostssl::NodeIPEndpoint endPoint;

        bcos::boostssl::ws::WsTools::stringToEndPoint(peer ? peer : "", endPoint);
        // if (!result)
        // {
        //     BOOST_THROW_EXCEPTION(InvalidParameter() << errinfo_comment(
        //                               ("the connected peer should be in host<ipv4/ipv6/domain "
        //                                "name>:port:port string format, invalid value: " +
        //                                   std::string(peer))
        //                                   .c_str()));
        //     continue;
        // }
        // else
        // {
        ep[i].host = strdup(endPoint.address().c_str());
        ep[i].port = endPoint.port();
        // }

        // env->ReleaseStringUTFChars(jpeer, peer);
    }

    jfieldID jdisableSsl = env->GetFieldID(configClass, "disableSsl", "Z");
    if (jdisableSsl == NULL)
    {
        env->FatalError(
            ("No such field in the class, className: " + className + " ,field: disableSsl")
                .c_str());
    }

    bool disableSsl = (int)env->GetBooleanField(jconfig, jdisableSsl);

    std::string strSslType;

    bcos_sdk_c_cert_config* cert_config = NULL;
    bcos_sdk_c_sm_cert_config* sm_cert_config = NULL;
    if (!disableSsl)
    {
        jfieldID sslTypeField = env->GetFieldID(configClass, "sslType", "Ljava/lang/String;");
        if (sslTypeField == NULL)
        {
            env->FatalError(
                ("No such field in the class, className: " + className + " ,field: sslType")
                    .c_str());
        }

        jstring jsslType = (jstring)env->GetObjectField(jconfig, sslTypeField);
        if (!jsslType)
        {
            BOOST_THROW_EXCEPTION(
                InvalidParameter() << errinfo_comment(
                    "sslType has not been initialized, it should be \"ssl\" or \"sm_ssl\""));
        }

        const char* sslType = env->GetStringUTFChars(jsslType, NULL);
        strSslType = std::string(sslType);
        env->ReleaseStringUTFChars(jsslType, sslType);
        if (strSslType != "sm_ssl")
        {
            std::string className = "org/fisco/bcos/sdk/jni/common/JniConfig$CertConfig";
            jfieldID certConfigField = env->GetFieldID(
                configClass, "certConfig", "Lorg/fisco/bcos/sdk/jni/common/JniConfig$CertConfig;");
            if (certConfigField == NULL)
            {
                env->FatalError(
                    ("No such field in the class, className: " + className + " ,field: certConfig")
                        .c_str());
            }

            jobject jcertConfig = env->GetObjectField(jconfig, certConfigField);
            if (!jcertConfig)
            {
                BOOST_THROW_EXCEPTION(InvalidParameter()
                                      << errinfo_comment("cert config has not been initialized, it "
                                                         "is used for ssl connection, please "
                                                         "set it"));
            }

            jclass certConfigClass = env->GetObjectClass(jcertConfig);
            cert_config = create_bcos_sdk_c_cert_config(env, certConfigClass, jcertConfig);
        }
        else
        {
            std::string className = "org/fisco/bcos/sdk/jni/common/JniConfig$SMCertConfig";
            jfieldID smCertConfigField = env->GetFieldID(configClass, "smCertConfig",
                "Lorg/fisco/bcos/sdk/jni/common/JniConfig$SMCertConfig;");
            if (smCertConfigField == NULL)
            {
                env->FatalError((
                    "No such field in the class, className: " + className + " ,field: smCertConfig")
                                    .c_str());
            }
            jobject jSmCertConfig = env->GetObjectField(jconfig, smCertConfigField);
            if (!jSmCertConfig)
            {
                BOOST_THROW_EXCEPTION(InvalidParameter() << errinfo_comment(
                                          "sm cert config has not been initialized, it "
                                          "is used for ssl connection, please "
                                          "set it"));
            }

            jclass smCertConfigClass = env->GetObjectClass(jSmCertConfig);
            sm_cert_config =
                create_bcos_sdk_c_sm_cert_config(env, smCertConfigClass, jSmCertConfig);
        }
    }

    struct bcos_sdk_c_config* config = bcos_sdk_c_config_create_empty();
    // init bcos_sdk_c_config
    config->heartbeat_period_ms = heartbeatPeriodMs;
    config->reconnect_period_ms = reconnectPeriodMs;
    config->message_timeout_ms = messageTimeoutMs;
    config->thread_pool_size = threadPoolSize;
    config->peers_count = listSize;
    config->disable_ssl = disableSsl;
    config->is_cert_path = 0;
    config->peers = ep;
    config->ssl_type = strdup(strSslType.c_str());
    config->cert_config = cert_config;
    config->sm_cert_config = sm_cert_config;

    return config;
}

static JClassCache classCache;
jclass bcos_sdk_c_find_jclass(JNIEnv* env, const char* className)
{
    return classCache.findClass(env, className);
}