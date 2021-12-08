#ifndef __INCLUDE_BCOS_SDK_EVENT__
#define __INCLUDE_BCOS_SDK_EVENT__

#include "bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------event interface begin ----------------------

const char* bcos_event_sub_subscribe_event(void* sdk, const char* group, const char* params,
    bcos_sdk_c_struct_response_cb callback, void* context);

void bcos_event_sub_unsubscribe_event(void* sdk, const char* event_sub_id);

// ------------------------------event interface end -------------------------

#ifdef __cplusplus
}
#endif
#endif
