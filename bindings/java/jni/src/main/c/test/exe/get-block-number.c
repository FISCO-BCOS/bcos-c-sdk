#include "bcos-c-sdk/bcos_sdk_c.h"
#include "bcos-c-sdk/bcos_sdk_c_rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage()
{
    printf("Usage: get-block-number <host> <port> <node>(optional)\n");
    printf("Example:\n");
    printf("    ./get-block-number 127.0.0.1 20200\n");
    printf("    ./get-block-number 127.0.0.1 20200 node0\n");
    exit(0);
}

void callback(struct bcos_sdk_c_struct_response* resp)
{
    printf("status = %d \n", resp->error);
    printf("desc = %s \n", resp->desc);
    printf("data = %s \n", (char*)resp->data);
    printf("size = %zu \n", resp->size);
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        usage();
    }

    char* host = argv[1];
    uint16_t port = atoi(argv[2]);

    char* node = argc > 3 ? argv[3] : "";

    // ini bcos_sdk_c_config
    struct bcos_sdk_c_config config;

    config.heartbeat_period_ms = 20000;
    config.reconnect_period_ms = 20000;
    config.message_timeout_ms = 20000;
    config.thread_pool_size = 4;

    struct bcos_sdk_c_endpoint ep;
    ep.host = host;
    ep.port = port;

    config.peers = &ep;
    config.peers_count = 1;

    void* sdk = bcos_sdk_create(&config);
    if (!sdk)
    {
        printf("bcos_sdk_create failed.\n");
        return 0;
    }

    bcos_sdk_start(sdk);

    printf("create sdk service.\n");
    const char* group = "group";

    while (1)
    {
        sleep(5);
        bcos_rpc_get_block_number(sdk, group, node, callback, sdk);
    }

    return 0;
}
