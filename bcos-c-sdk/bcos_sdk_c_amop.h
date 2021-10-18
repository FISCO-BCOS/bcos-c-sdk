#ifndef __INCLUDE_BCOS_SDK_AMOP__
#define __INCLUDE_BCOS_SDK_AMOP__

#include "bcos-c-sdk/bcos_sdk_c_common.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------- amop callback begin----------------------------
typedef void (*bcos_sdk_c_amop_subscribe_cb)(const char *endpoint,
                                             const char *seq,
                                             struct bcos_sdk_struct_response *);
typedef void (*bcos_sdk_c_amop_publish_cb)(struct bcos_sdk_struct_response *);
//------------------------- amop callback end -----------------------------

// -------------------------amop interface begin --------------------------

void bcos_amop_subscribe_topic(void *amop, const char *topics[],
                               size_t topic_count);

void bcos_amop_subscribe_topic_with_cb(void *amop, const char *topic,
                                       bcos_sdk_c_amop_subscribe_cb cb,
                                       void *context);

void bcos_amop_unsubscribe_topic(void *amop, const char *topics[],
                                 size_t topic_count);

void bcos_amop_publish(void *amop, const char *topic, void *data, size_t size,
                       uint32_t timeout, bcos_sdk_c_amop_publish_cb cb,
                       void *context);

void bcos_amop_broadcast(void *amop, const char *topic, void *data,
                         size_t size);

void bcos_amop_query_subscribed_topics(void *amop);

void bcos_amop_set_subscribe_topic_cb(void *amop,
                                      bcos_sdk_c_amop_subscribe_cb cb,
                                      void *context);

void bcos_amop_send_response(void *amop, const char *endpoint, const char *peer,
                             void *data, size_t size);

void *bcos_amop_get_ws(void *amop);

// -------------------------- amop interface end ----------------------------
#ifdef __cplusplus
}
#endif
#endif
