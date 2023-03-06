package org.fisco.bcos.sdk.jni.rpc;

import org.fisco.bcos.sdk.jni.common.Response;

/** receive message callback for modules of java-sdk */
public interface RpcCallback {
  /**
   * on receive response message
   *
   * @param response
   * @return void
   */
  void onResponse(Response response);
}
