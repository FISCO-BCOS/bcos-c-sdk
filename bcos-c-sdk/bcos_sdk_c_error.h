#ifndef __INCLUDE_BCOS_SDK_C_ERROR__
#define __INCLUDE_BCOS_SDK_C_ERROR__

#ifdef __cplusplus
extern "C" {
#endif

extern int bcos_sdk_get_last_error();
extern const char* bcos_sdk_get_last_error_msg();

extern void bcos_sdk_clear_last_error();
extern void bcos_sdk_set_last_error_msg(int error, const char* msg);

#ifdef __cplusplus
}
#endif
#endif