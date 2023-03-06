package org.fisco.bcos.sdk.jni.amop;

import org.fisco.bcos.sdk.jni.common.Response;

public interface AmopResponseCallback {
  void onResponse(Response response);
}
