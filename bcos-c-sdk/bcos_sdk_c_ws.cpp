#include "bcos-c-sdk/bcos_sdk_c_ws.h"
#include <bcos-boostssl/websocket/WsService.h>

// ------------ websocket interface interface begin -------------------------

int32_t bcos_ws_connection_count(void *ws) {
  if (ws) {
    auto service = (bcos::boostssl::ws::WsService *)ws;
    return service->sessions().size();
  }

  return -1;
}

// ------------ websocket interface interface end ---------------------------