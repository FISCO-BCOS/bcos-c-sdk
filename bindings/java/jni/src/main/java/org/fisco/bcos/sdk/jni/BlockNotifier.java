package org.fisco.bcos.sdk.jni;

import java.math.BigInteger;

public interface BlockNotifier {
  /**
   * receive block notifier
   *
   * @param groupId
   * @param blockNumber
   * @return void
   */
  void onResponse(String groupId, BigInteger blockNumber);
}
