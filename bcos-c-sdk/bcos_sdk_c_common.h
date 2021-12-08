#ifndef __INCLUDE_BCOS_SDK_CONFIG__
#define __INCLUDE_BCOS_SDK_CONFIG__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// #define _BCOS_SDK_JNI_DEBUG_

#ifdef __cplusplus
extern "C" {
#endif
//--------------- config items begin ----------------

// connect endpoint
struct bcos_sdk_c_endpoint
{
    char* host;  // c-style string，end with '\0', support: ipv4 / ipv6 / domain name
    uint16_t port;
};

// ssl config
struct bcos_sdk_c_cert_config
{
    char* ca_cert;  // Note: buffer of cert should be in pem format
    char* node_key;
    char* node_cert;
};

// sm ssl config
struct bcos_sdk_c_sm_cert_config
{
    char* ca_cert;
    char* node_key;
    char* node_cert;
    char* en_node_key;
    char* en_node_cert;
};

// config for bcos-c-sdk
struct bcos_sdk_c_config
{
    // common config
    int thread_pool_size;
    int message_timeout_ms;
    int reconnect_period_ms;
    int heartbeat_period_ms;

    // connected peers
    struct bcos_sdk_c_endpoint* peers;
    size_t peers_count;

    // the switch for disable ssl connection
    int disableSsl;

    // ssl or sm_ssl
    char* ssl_type;
    struct bcos_sdk_c_cert_config* cert_config;
    struct bcos_sdk_c_sm_cert_config* sm_cert_config;
};

struct bcos_sdk_c_config* bcos_sdk_c_config_create_empty();
struct bcos_sdk_c_config* bcos_sdk_c_config_create_with_config(const char* configFile);

void bcos_sdk_c_config_destroy(void* p);

void bcos_sdk_c_cert_config_destroy(void* p);
void bcos_sdk_c_sm_cert_config_destroy(void* p);

//--------------- config items end ----------------

//--------------- callback begin------------
// response
struct bcos_sdk_c_struct_response
{
    int error;   // code
    char* desc;  // code describe message

    void* data;   // response data pointer
    size_t size;  // response data size

    void* context;  // callback context
};

typedef void (*bcos_sdk_c_struct_response_cb)(struct bcos_sdk_c_struct_response*);
//--------------- callback end------------

// handle callback
void bcos_sdk_c_handle_response(
    void* error, void* data, size_t size, bcos_sdk_c_struct_response_cb callback, void* context);

#ifdef __cplusplus
}
#endif
#endif
