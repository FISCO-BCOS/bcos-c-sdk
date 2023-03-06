/*
 * Copyright 2014-2020  [fisco-dev]
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 *
 */

package org.fisco.bcos.sdk.jni.amop;

import java.util.Set;
import org.fisco.bcos.sdk.jni.common.JniException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class AmopJniObj {
  private static final Logger logger = LoggerFactory.getLogger(AmopJniObj.class);

  /**
   * @param nativePointer
   * @return
   * @throws JniException
   */
  public static AmopJniObj build(long nativePointer) {
    AmopJniObj amop = new AmopJniObj();
    amop.setNativePointer(nativePointer);

    logger.info(" nativePointer: {}", nativePointer);
    return amop;
  }

  private AmopJniObj() {}

  private long nativePointer = 0L;

  public long getNativePointer() {
    return nativePointer;
  }

  private void setNativePointer(long nativePointer) {
    this.nativePointer = nativePointer;
  }

  // ----------------------------- Amop interface begin --------------------------------------

  public native void start();

  public native void stop();

  public native void subscribeTopic(Set<String> topicsName);

  public native void subscribeTopic(String topicName, AmopRequestCallback callback);

  public native void unsubscribeTopic(Set<String> topicsName);

  public native void setCallback(AmopRequestCallback cb);

  public native void sendAmopMsg(
      String topic, byte[] data, int timeout, AmopResponseCallback callback);

  public native void broadcastAmopMsg(String topic, byte[] data);

  public native void sendResponse(String endPoint, String seq, byte[] data);

  public native Set<String> getSubTopics();

  // ----------------------------- Amop interface end --------------------------------------
}
