#ifndef __INCLUDE_BCOS_SDK__
#define __INCLUDE_BCOS_SDK__

#include "bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// create bcos sdk object by config
void* bcos_sdk_create(struct bcos_sdk_c_config* config);
// start the bcos sdk
void bcos_sdk_start(void* sdk);
// stop the bcos sdk object
void bcos_sdk_stop(void* sdk);
// destroy the bcos sdk object
void bcos_sdk_destroy(void* sdk);

#ifdef __cplusplus
}
#endif
#endif
