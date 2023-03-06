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

package org.fisco.bcos.sdk.jni.event;

import java.util.Set;
import org.fisco.bcos.sdk.jni.common.JniException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class EventSubJniObj {

  private static final Logger logger = LoggerFactory.getLogger(EventSubJniObj.class);

  /**
   * @param nativePointer
   * @return
   * @throws JniException
   */
  public static EventSubJniObj build(long nativePointer) throws JniException {
    EventSubJniObj subscribe = new EventSubJniObj();
    subscribe.setNativePointer(nativePointer);

    logger.info("nativePointer: {}", nativePointer);
    return subscribe;
  }

  private EventSubJniObj() {}

  private long nativePointer = 0L;

  public long getNativePointer() {
    return nativePointer;
  }

  private void setNativePointer(long nativePointer) {
    this.nativePointer = nativePointer;
  }

  // ----------------------------- EventSub interface begin --------------------------------------

  public native void start();

  public native void stop();

  public native String subscribeEvent(String group, String params, EventSubCallback callback);

  public native void unsubscribeEvent(String eventID);

  public native Set<String> getAllSubscribedEvents();

  // ----------------------------- EventSub interface begin --------------------------------------
}
