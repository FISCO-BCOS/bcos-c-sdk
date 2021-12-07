#ifndef __INCLUDE_BCOS_SDK_EVENT__
#define __INCLUDE_BCOS_SDK_EVENT__

#include "bcos-c-sdk/bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------event interface begin ----------------------

void bcos_event_sub_subscribe_event(void *event, const char *group,
                                    const char *params,
                                    bcos_sdk_struct_response_cb callback,
                                    void *context);

void bcos_event_sub_unsubscribe_event(void *event, const char *event_sub_id,
                                      bcos_sdk_struct_response_cb callback,
                                      void *context);

// ------------------------------event interface end -------------------------

void *bcos_event_sub_get_ws(void *event);

#ifdef __cplusplus
}
#endif
#endif
