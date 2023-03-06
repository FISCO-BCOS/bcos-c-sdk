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

package org.fisco.bcos.sdk.jni.rpc;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class RpcJniObj {
  private static final Logger logger = LoggerFactory.getLogger(RpcJniObj.class);

  public static RpcJniObj build(long nativePointer) {
    RpcJniObj rpc = new RpcJniObj();
    rpc.setNativePointer(nativePointer);
    logger.info("nativePointer: {}", nativePointer);
    return rpc;
  }

  private RpcJniObj() {}

  private long nativePointer = 0L;

  public long getNativePointer() {
    return nativePointer;
  }

  private void setNativePointer(long nativePointer) {
    this.nativePointer = nativePointer;
  }

  // ----------------------------- RPC interface begin --------------------------------------

  public native void start();

  public native void stop();

  // -----------------------------------------------------------------------------------------
  public native void genericMethod(String data, RpcCallback callback);

  public native void genericMethod(String group, String data, RpcCallback callback);

  public native void genericMethod(String group, String node, String data, RpcCallback callback);
  // -----------------------------------------------------------------------------------------

  public native void call(String group, String node, String to, String data, RpcCallback callback);

  public native void sendTransaction(
      String group, String node, String data, boolean requireProof, RpcCallback callback);

  public native void getTransaction(
      String group, String node, String txHash, boolean requireProof, RpcCallback callback);

  public native void getTransactionReceipt(
      String group, String node, String txHash, boolean requireProof, RpcCallback callback);

  public native void getBlockByHash(
      String group,
      String node,
      String blockHash,
      boolean onlyHeader,
      boolean onlyTxHash,
      RpcCallback callback);

  public native void getBlockByNumber(
      String group,
      String node,
      long blockNumber,
      boolean onlyHeader,
      boolean onlyTxHash,
      RpcCallback callback);

  public native void getBlockHashByNumber(
      String group, String node, long blockNumber, RpcCallback callback);

  public native long getBlockLimit(String group);

  public native void getBlockNumber(String group, String node, RpcCallback callback);

  public native void getCode(
      String group, String node, String contractAddress, RpcCallback callback);

  public native void getSealerList(String group, String node, RpcCallback callback);

  public native void getObserverList(String group, String node, RpcCallback callback);

  public native void getPbftView(String group, String node, RpcCallback callback);

  public native void getPendingTxSize(String group, String node, RpcCallback callback);

  public native void getSyncStatus(String group, String node, RpcCallback callback);

  public native void getSystemConfigByKey(
      String group, String node, String keyValue, RpcCallback callback);

  public native void getTotalTransactionCount(String group, String node, RpcCallback callback);

  public native void getGroupPeers(String group, RpcCallback callback);

  public native void getPeers(RpcCallback callback);

  public native void getGroupList(RpcCallback callback);

  public native void getGroupInfo(String group, RpcCallback callback);

  public native void getGroupInfoList(RpcCallback callback);

  public native void getGroupNodeInfo(String group, String node, RpcCallback callback);

  // ----------------------------- RPC interface end --------------------------------------
}
