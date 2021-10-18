#include "bcos-c-sdk/bcos_sdk_c.h"
#include <bcos-cpp-sdk/SdkFactory.h>
#include <cstdio>
#include <mutex>

// c-style sdk obj
struct Sdk {
  void *service;
  void *rpc;
  void *amop;
  void *event;
};

// construct WsConfig obj by struct Config
static std::shared_ptr<bcos::boostssl::ws::WsConfig>
initWsConfig(struct bcos_sdk_struct_config *config) {
  // init WsConfig by Config
  auto wsConfig = std::make_shared<bcos::boostssl::ws::WsConfig>();
  wsConfig->setModel(bcos::boostssl::ws::WsModel::Client);

  auto peers = std::make_shared<bcos::boostssl::ws::EndPoints>();
  for (size_t i = 0; i < config->peers_count; i++) {
    bcos::boostssl::ws::EndPoint ep;
    ep.host = config->peers[i].host;
    ep.port = config->peers[i].port;
    peers->push_back(ep);
  }

  // TODO: set default value
  wsConfig->setConnectedPeers(peers);
  wsConfig->setThreadPoolSize(config->thread_pool_size);
  wsConfig->setReconnectPeriod(config->reconnect_period_ms);
  wsConfig->setHeartbeatPeriod(config->heartbeat_period_ms);

  return wsConfig;
}

// create bcos sdk object by config
void *bcos_sdk_create(struct bcos_sdk_struct_config *config) {
  // construct sdk object
  auto factory = std::make_shared<bcos::cppsdk::SdkFactory>();
  factory->setConfig(initWsConfig(config));

  auto wsService = factory->buildWsService();

  auto rpc = factory->buildJsonRpc(wsService);
  auto amop = factory->buildAMOP(wsService);
  // TODO:event sub is not being test
  // auto event = factory->buildEventSub(wsService);

  auto rpcPointer = rpc.release();
  auto amopPointer = amop.release();

  auto sdk = new Sdk();
  sdk->service = wsService.get();
  sdk->rpc = rpcPointer;
  sdk->amop = amopPointer;
  // sdk->event = eventPointer;

  return sdk;
}

// destroy the bcos sdk object
void bcos_sdk_destroy(void *sdk) {
  if (sdk) {
    bcos_sdk_destroy_rpc(((Sdk *)sdk)->rpc);
    bcos_sdk_destroy_amop(((Sdk *)sdk)->amop);
    // TODO:
    // bcos_sdk_destroy_event_sub(((Sdk *)sdk)->event);

    // delete Sdk object
    delete (Sdk *)sdk;
  }
}

// start bcos sdk
void bcos_sdk_start(void *sdk) {
  if (sdk) {
    // start websocket service first
    auto service = (bcos::boostssl::ws::WsService *)((Sdk *)sdk)->service;
    service->start();
    bcos_sdk_start_rpc(((Sdk *)sdk)->rpc);
    bcos_sdk_start_amop(((Sdk *)sdk)->amop);
    // TODO:
    // bcos_sdk_start_event_sub(((Sdk *)sdk)->event);
  }
}

// stop bcos sdk
void bcos_sdk_stop(void *sdk) {
  if (sdk) {
    bcos_sdk_stop_rpc(((Sdk *)sdk)->rpc);
    bcos_sdk_stop_amop(((Sdk *)sdk)->amop);
    bcos_sdk_stop_event_sub(((Sdk *)sdk)->event);
  }
}

// get rpc module from sdk object
void *bcos_sdk_get_rpc(void *sdk, const char *group) {
  // TODO: init with group
  (void)group;

  return sdk ? ((Sdk *)sdk)->rpc : NULL;
}

// get event sub module from sdk object
void *bcos_sdk_get_event_sub(void *sdk, const char *group) {
  // TODO: init with group
  (void)group;

  return sdk ? ((Sdk *)sdk)->event : NULL;
}

// get amop module from sdk object
void *bcos_sdk_get_amop(void *sdk) { return sdk ? ((Sdk *)sdk)->amop : NULL; }

void *bcos_sdk_create_rpc(struct bcos_sdk_struct_config *config) {
  auto wsConfig = initWsConfig(config);

  // construct sdk object
  auto factory = std::make_shared<bcos::cppsdk::SdkFactory>();
  factory->setConfig(wsConfig);

  auto wsService = factory->buildWsService();
  auto rpc = factory->buildJsonRpc(wsService);

  auto rpcPointer = rpc.get();
  rpc.reset();

  return rpcPointer;
}

// destroy the bcos sdk rpc sdk object
void bcos_sdk_destroy_rpc(void *rpc) {
  // stop rpc service and delete rpc object
  if (rpc) {
    auto rpcPointer = (bcos::cppsdk::jsonrpc::JsonRpcInterface *)rpc;
    rpcPointer->stop();
    delete rpcPointer;
    rpc = NULL;
  }
}

// start the rpc service
void bcos_sdk_start_rpc(void *rpc) {
  if (rpc) {
    auto rpcPointer = (bcos::cppsdk::jsonrpc::JsonRpcInterface *)rpc;
    rpcPointer->start();
  }
}

// stop the rpc service
void bcos_sdk_stop_rpc(void *rpc) {
  if (rpc) {
    auto rpcPointer = (bcos::cppsdk::jsonrpc::JsonRpcInterface *)rpc;
    rpcPointer->stop();
  }
}

// create bcos sdk amop object by config
void *bcos_sdk_create_amop(struct bcos_sdk_struct_config *config) {
  auto wsConfig = initWsConfig(config);

  // construct sdk object
  auto factory = std::make_shared<bcos::cppsdk::SdkFactory>();
  factory->setConfig(wsConfig);

  auto wsService = factory->buildWsService();
  auto amop = factory->buildAMOP(wsService);

  auto amopPointer = amop.get();
  amop.reset();

  return amopPointer;
}

// destroy the bcos sdk amop object
void bcos_sdk_destroy_amop(void *amop) {
  // stop amop service and delete amop object
  if (amop) {
    auto amopPointer = (bcos::cppsdk::amop::AMOPInterface *)amop;
    amopPointer->stop();
    delete amopPointer;
    amop = NULL;
  }
}

// start the amop service
void bcos_sdk_start_amop(void *amop) {
  if (amop) {
    auto amopPointer = (bcos::cppsdk::amop::AMOPInterface *)amop;
    amopPointer->start();
  }
}

// stop the amop service
void bcos_sdk_stop_amop(void *amop) {
  if (amop) {
    auto amopPointer = (bcos::cppsdk::amop::AMOPInterface *)amop;
    amopPointer->stop();
  }
}

// create bcos sdk event sub object by config
void *bcos_sdk_create_event_sub(struct bcos_sdk_struct_config *config) {
  auto wsConfig = initWsConfig(config);

  // construct sdk object
  auto factory = std::make_shared<bcos::cppsdk::SdkFactory>();
  factory->setConfig(wsConfig);

  auto wsService = factory->buildWsService();
  auto event = factory->buildEventSub(wsService);

  auto eventPointer = event.get();
  event.reset();

  return eventPointer;
}

// destroy the bcos sdk event sub object
void bcos_sdk_destroy_event_sub(void *event) {
  // stop event sub service and delete event sub object
  if (event) {
    auto eventPointer = (bcos::cppsdk::event::EventSubInterface *)event;
    eventPointer->stop();
    delete eventPointer;
    event = NULL;
  }
}

// start the event sub service
void bcos_sdk_start_event_sub(void *event) {
  if (event) {
    auto eventPointer = (bcos::cppsdk::event::EventSubInterface *)event;
    eventPointer->start();
  }
}

// stop the event sub service
void bcos_sdk_stop_event_sub(void *event) {
  if (event) {
    auto eventPointer = (bcos::cppsdk::event::EventSubInterface *)event;
    eventPointer->stop();
  }
}