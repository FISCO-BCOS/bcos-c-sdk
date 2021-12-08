
#include "bcos_sdk_c_event_sub.h"
#include "bcos_sdk_c_common.h"
#include <bcos-cpp-sdk/Sdk.h>
#include <bcos-cpp-sdk/event/EventSub.h>
#include <bcos-cpp-sdk/event/EventSubInterface.h>
#include <tuple>

using namespace bcos;
using namespace bcos::boostssl;
using namespace bcos::boostssl::utilities;
using namespace bcos::boostssl::utilities::protocol;

// ------------------------------event interface begin ----------------------

const char* bcos_event_sub_subscribe_event(void* sdk, const char* group, const char* params,
    bcos_sdk_c_struct_response_cb callback, void* context)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto eventSub = sdkPointer->eventSub();
    auto eventSubId = eventSub->subscribeEvent(
        group, params, [callback, context](Error::Ptr error, const std::string& resp) {
            bcos_sdk_c_handle_response(
                error ? error.get() : NULL, (void*)resp.data(), resp.size(), callback, context);
        });
    return strdup(eventSubId.c_str());
}

void bcos_event_sub_unsubscribe_event(void* sdk, const char* event_sub_id)
{
    auto sdkPointer = (bcos::cppsdk::Sdk*)sdk;
    auto eventSub = sdkPointer->eventSub();
    eventSub->unsubscribeEvent(event_sub_id);
}

// ------------------------------event interface end -------------------------