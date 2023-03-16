#include "org_fisco_bcos_sdk_jni_utilities_signature_SignatureJniObj.h"
#include "bcos-c-sdk/bcos_sdk_c_error.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_keypair.h"
#include "bcos-c-sdk/bcos_sdk_c_uti_signature.h"
#include "org_fisco_bcos_sdk_exception.h"
#include <string.h>

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_signature_SignatureJniObj
 * Method:    sign
 * Signature: (JLjava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jbyteArray JNICALL Java_org_fisco_bcos_sdk_jni_utilities_signature_SignatureJniObj_sign
  (JNIEnv* env, jclass, jlong jkeypair, jstring jtx_data_hash, jstring jhsm_lib_path)
{
     const char* hsm_lib_path = env->GetStringUTFChars(jhsm_lib_path, NULL);
     const char* tx_data_hash = env->GetStringUTFChars(jtx_data_hash, NULL);
     void* keypair = reinterpret_cast<void*>(jkeypair);
     struct bcos_sdk_c_signature_result sign_result = bcos_sdk_sign(keypair, tx_data_hash, hsm_lib_path);

     env->ReleaseStringUTFChars(jhsm_lib_path, hsm_lib_path);
     env->ReleaseStringUTFChars(jtx_data_hash, tx_data_hash);

     int operation_status = bcos_sdk_get_last_error();
     if (operation_status == -1)
     {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
     }

    jbyte *signed_data_r = (jbyte*)sign_result.r;
    jbyte *signed_data_s = (jbyte*)sign_result.s;
    jbyte *signed_data_v = (jbyte*)sign_result.v;
    jbyteArray jsigned_data = env->NewByteArray(576);
    env->SetByteArrayRegion(jsigned_data, 0, 32, signed_data_r);
    env->SetByteArrayRegion(jsigned_data, 32, 32, signed_data_s);
    env->SetByteArrayRegion(jsigned_data, 64, 512, signed_data_v);

    return jsigned_data;
}

/*
 * Class:     org_fisco_bcos_sdk_jni_utilities_signature_SignatureJniObj
 * Method:    verify
 * Signature: (I[BLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_fisco_bcos_sdk_jni_utilities_signature_SignatureJniObj_verify
  (JNIEnv* env, jclass, jint jcrypto_type, jbyteArray jpub_key, jstring jtx_data_hash, jstring jsigned_data,
  jstring jhsm_lib_path)
{
    int crypto_type = (int)jcrypto_type;
    jbyte* public_key = (jbyte*)env->GetByteArrayElements(jpub_key, 0);
    jsize pubkey_len = env->GetArrayLength(jpub_key);
    const char* tx_data_hash = env->GetStringUTFChars(jtx_data_hash, NULL);
    const char* signed_data = env->GetStringUTFChars(jsigned_data, NULL);
    jsize signed_data_len = strlen(signed_data);
    const char* hsm_lib_path = env->GetStringUTFChars(jhsm_lib_path, NULL);

    bool is_verify_success =  bcos_sdk_verify(crypto_type, (void*)public_key, pubkey_len, tx_data_hash,
                                        (void*)signed_data, signed_data_len, hsm_lib_path);

    env->ReleaseStringUTFChars(jtx_data_hash, tx_data_hash);
    env->ReleaseStringUTFChars(jsigned_data, signed_data);
    env->ReleaseStringUTFChars(jhsm_lib_path, hsm_lib_path);
    if (!bcos_sdk_is_last_opr_success())
    {
        THROW_JNI_EXCEPTION(env, bcos_sdk_get_last_error_msg());
    }

    return is_verify_success ? JNI_TRUE : JNI_FALSE;
}
