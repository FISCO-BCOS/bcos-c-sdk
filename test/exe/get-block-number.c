#include "bcos-c-sdk/bcos_sdk_c.h"
#include "bcos-c-sdk/bcos_sdk_c_rpc.h"
#include "bcos-c-sdk/bcos_sdk_c_ws.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage() {
  printf("Usage: get-block-number <host> <port>\n");
  printf("Example:\n");
  printf("    ./get-block-number 127.0.0.1 20200\n");
  exit(0);
}

void callback(struct bcos_sdk_struct_response *resp) {
  printf("status = %d \n", resp->error);
  printf("desc = %s \n", resp->desc);
  printf("data = %s \n", (char *)resp->data);
  printf("size = %zu \n", resp->size);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    usage();
  }

  char *host = argv[1];
  uint16_t port = atoi(argv[2]);

  // ini bcos_sdk_struct_config
  struct bcos_sdk_struct_config config;

  config.heartbeat_period_ms = 20000;
  config.reconnect_period_ms = 20000;
  config.message_timeout_ms = 20000;
  config.thread_pool_size = 4;

  struct bcos_sdk_struct_endpoint ep;
  ep.host = host;
  ep.port = port;

  config.peers = &ep;
  config.peers_count = 1;

  void *sdk = bcos_sdk_create(&config);
  if (!sdk) {
    printf("bcos_sdk_create failed.\n");
    return 0;
  }

  printf("start sdk service.\n");
  bcos_sdk_start(sdk);

  void *rpc = bcos_sdk_get_rpc(sdk, "group");
  if (!rpc) {
    printf("bcos_sdk_get_rpc failed.\n");
    return 0;
  }

  void *ws = bcos_rpc_get_ws(rpc);
  if (!ws) {
    printf("bcos_rpc_get_ws failed.\n");
    return 0;
  }

  const char *group = "group";

  while (1) {
    sleep(5);
    int con_count = bcos_ws_connection_count(ws);
    printf(" ==> websocket connection count: %d\n", con_count);
    bcos_rpc_get_block_number(rpc, group, callback, rpc);
  }

  return 0;
}
