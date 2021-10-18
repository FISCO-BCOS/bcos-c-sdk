#ifndef __INCLUDE_BCOS_SDK_CONFIG__
#define __INCLUDE_BCOS_SDK_CONFIG__

#include <stdint.h>
#include <stdio.h>

//--------------- config items begin ----------------

// connect endpoint
struct bcos_sdk_struct_endpoint
{
    char* host;  // c-style string，end with '\0'
    uint16_t port;
};

// ssl config
struct bcos_sdk_struct_cert_config
{
    char* ca_cert;  // Note: buffer of cert should be in der format

    char* node_key;
    char* node_cert;
};

// sm ssl config
struct bcos_sdk_struct_sm_cert_config
{
    char* ca_cert;

    char* node_key;
    char* node_cert;

    char* en_node_key;
    char* en_node_cert;
};

// config for bcos-c-sdk
struct bcos_sdk_struct_config
{
    // common config
    int thread_pool_size;
    int reconnect_period_ms;
    int heartbeat_period_ms;
    int message_timeout_ms;

    // ssl type: ssl or sm
    int ssl_type;
    struct bcos_sdk_struct_cert_config cert_config;
    struct bcos_sdk_struct_sm_cert_config sm_cert_config;

    // connected peers info
    struct bcos_sdk_struct_endpoint* peers;
    size_t peers_count;
};
//--------------- config items end ----------------

//--------------- callback begin------------
// response
struct bcos_sdk_struct_response
{
    int error;   // status code
    char* desc;  // status describe

    void* data;
    size_t size;

    void* context;  // callback context
};

typedef void (*bcos_sdk_struct_response_cb)(struct bcos_sdk_struct_response*);
//--------------- callback end------------

// handle callback
void bcos_sdk_c_handle_response(
    void* error, void* data, size_t size, bcos_sdk_struct_response_cb callback, void* context);
// release bcos_sdk_struct_response object
void bcos_sdk_c_release_response(void* p);

#endif
