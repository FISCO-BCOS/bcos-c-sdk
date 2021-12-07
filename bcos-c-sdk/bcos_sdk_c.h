#ifndef __INCLUDE_BCOS_SDK__
#define __INCLUDE_BCOS_SDK__

#include "bcos-c-sdk/bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------
// bcos sdk function start
//--------------------------------------------------------
// create bcos sdk object by config
void *bcos_sdk_create(struct bcos_sdk_struct_config *config);
// destroy the bcos sdk object
void bcos_sdk_destroy(void *sdk);
// start bcos sdk
void bcos_sdk_start(void *sdk);
// stop bcos sdk
void bcos_sdk_stop(void *sdk);

// get rpc module from sdk object
void *bcos_sdk_get_rpc(void *sdk, const char *group);
// get amop module from sdk object
void *bcos_sdk_get_amop(void *sdk);
// get event sub module from sdk object
void *bcos_sdk_get_event_sub(void *sdk, const char *group);
//--------------------------------------------------------
// bcos sdk function end
//--------------------------------------------------------

//--------------------------------------------------------
// bcos rpc function start
//--------------------------------------------------------
// create bcos sdk rpc object by config
void *bcos_sdk_create_rpc(struct bcos_sdk_struct_config *config);
// destroy the bcos sdk rpc sdk object
void bcos_sdk_destroy_rpc(void *rpc);
// start the rpc service
void bcos_sdk_start_rpc(void *rpc);
// stop the rpc service
void bcos_sdk_stop_rpc(void *rpc);
//--------------------------------------------------------
// bcos rpc function end
//--------------------------------------------------------

//--------------------------------------------------------
// bcos amop function start
//--------------------------------------------------------
// create bcos sdk amop by config
void *bcos_sdk_create_amop(struct bcos_sdk_struct_config *config);
// destroy the bcos sdk amop object
void bcos_sdk_destroy_amop(void *amop);
// start the amop service
void bcos_sdk_start_amop(void *amop);
// stop the amop service
void bcos_sdk_stop_amop(void *amop);
//--------------------------------------------------------
// bcos amop function end
//--------------------------------------------------------

//--------------------------------------------------------
// bcos event sub function start
//--------------------------------------------------------
// create bcos sdk event sub object by config
void *bcos_sdk_create_event_sub(struct bcos_sdk_struct_config *config);
// destroy the bcos sdk event sub object
void bcos_sdk_destroy_event_sub(void *event);
// start the bcos sdk event sub service
void bcos_sdk_start_event_sub(void *event);
// stop the bcos sdk event sub service
void bcos_sdk_stop_event_sub(void *event);
//--------------------------------------------------------
// bcos event sub function end
//--------------------------------------------------------

#ifdef __cplusplus
}
#endif
#endif
