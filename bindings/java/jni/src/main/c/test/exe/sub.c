#include "bcos-c-sdk/bcos_sdk_c.h"
#include "bcos-c-sdk/bcos_sdk_c_amop.h"
#include "bcos-c-sdk/bcos_sdk_c_rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage()
{
    printf("Usage: sub <host> <port> <topic>\n");
    printf("Example:\n");
    printf("    ./sub 127.0.0.1 20200 topic\n");
    exit(0);
}

void sub_cb(const char* endpoint, const char* seq, struct bcos_sdk_c_struct_response* resp)
{
    printf(" ==> receive request, status: %d, data: %s\n", resp->error, (char*)resp->data);
    bcos_amop_send_response(resp->context, endpoint, seq, resp->data, resp->size);
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        usage();
    }

    char* host = argv[1];
    uint16_t port = atoi(argv[2]);
    const char* topic = argv[3];

    // ini bcos_sdk_c_config
    struct bcos_sdk_c_config config;

    config.heartbeat_period_ms = 10000;
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

    printf("start sdk service.\n");

    printf(" ==> subscribe topic topic: %s\n", topic);

    bcos_amop_subscribe_topic_with_cb(sdk, topic, sub_cb, sdk);

    while (1)
    {
        sleep(5);
        printf(" ==> main <== \n");
    }

    return 0;
}
