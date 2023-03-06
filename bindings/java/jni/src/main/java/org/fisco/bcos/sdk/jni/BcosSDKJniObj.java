/**
 * Copyright 2014-2020 [fisco-dev]
 *
 * <p>Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the License at
 *
 * <p>http://www.apache.org/licenses/LICENSE-2.0
 *
 * <p>Unless required by applicable law or agreed to in writing, software distributed under the
 * License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.fisco.bcos.sdk.jni;

import org.fisco.bcos.sdk.jni.common.JniConfig;
import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.JniLibLoader;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class BcosSDKJniObj {

  private static final Logger logger = LoggerFactory.getLogger(BcosSDKJniObj.class);

  static {
    JniLibLoader.loadJniLibrary();
  }

  protected BcosSDKJniObj() {}

  /**
   * @param jniConfig
   * @return
   */
  public static BcosSDKJniObj build(JniConfig jniConfig) throws JniException {
    long nativePointer = create(jniConfig);
    BcosSDKJniObj bcosSDKJniObj = new BcosSDKJniObj();
    bcosSDKJniObj.setNativePointer(nativePointer);
    logger.info("newNativePointer, nativePointer: {}, jniConfig: {}", nativePointer, jniConfig);
    return bcosSDKJniObj;
  }

  /**
   * desctroy interface
   *
   * @param nativePointer
   */
  public static void destroy(long nativePointer) {
    if (nativePointer != 0L) {
      BcosSDKJniObj bcosSDKJniObj = new BcosSDKJniObj();
      bcosSDKJniObj.setNativePointer(nativePointer);
      bcosSDKJniObj.destroy();
    }
  }

  private long nativePointer = 0L;

  public long getNativePointer() {
    return nativePointer;
  }

  private void setNativePointer(long nativePointer) {
    this.nativePointer = nativePointer;
  }

  // ------------------------native method list-------------------------------------------

  public static native long create(JniConfig config);

  public native void start();

  public native void stop();

  public native void destroy();

  // -------------------------native method end-------------------------------------------

  // ------------------------ block notifier begin----------------------------------------

  public native void registerBlockNotifier(String group, BlockNotifier callback);

  // ------------------------ block notifier end------------------------------------------
}
