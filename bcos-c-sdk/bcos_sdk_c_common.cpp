#include "bcos-c-sdk/bcos_sdk_c_common.h"
#include <bcos-framework/interfaces/protocol/CommonError.h>
#include <bcos-framework/libutilities/Common.h>
#include <bcos-framework/libutilities/Error.h>

void bcos_sdk_c_handle_response(void *error, void *data, size_t size,
                                bcos_sdk_struct_response_cb callback,
                                void *context) {
  // auto resp = new bcos_sdk_struct_response();
  bcos_sdk_struct_response temp_resp;
  auto resp = &temp_resp;
  resp->context = context;

  auto errorPtr = (bcos::Error *)error;
  if (errorPtr &&
      errorPtr->errorCode() != bcos::protocol::CommonError::SUCCESS) {
    resp->error = errorPtr->errorCode();
    resp->desc = (char *)errorPtr->errorMessage().data();
  } else {
    resp->error = bcos::protocol::CommonError::SUCCESS;
    resp->data = (bcos::byte *)data;
    resp->size = size;
  }

  callback(resp);
}

// release bcos_sdk_struct_response object
void bcos_sdk_c_release_response(void *p) {
  (void)p;
  return;
  // delete (bcos_sdk_struct_response *)p;
}
