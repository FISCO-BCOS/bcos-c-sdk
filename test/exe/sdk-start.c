#include "bcos_sdk_c.h"
#include "bcos_sdk_c_rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage()
{
    printf("Usage: sdk-start <host> <port>\n");
    printf("Example:\n");
    printf("    ./sdk-start 127.0.0.1 20200\n");
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
    printf("create bcos sdk.\n");

    bcos_sdk_start(sdk);
    printf("start bcos sdk.\n");

    printf("get group list.\n");
    bcos_rpc_get_group_list(sdk, callback, sdk);

    sleep(5);
    bcos_sdk_stop(sdk);
    printf("stop bcos sdk.\n");

    sleep(5);
    bcos_sdk_destroy(sdk);
    printf("destroy bcos sdk.\n");

    return 0;
}
