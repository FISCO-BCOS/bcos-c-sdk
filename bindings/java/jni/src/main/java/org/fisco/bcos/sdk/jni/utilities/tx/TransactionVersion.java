package org.fisco.bcos.sdk.jni.utilities.tx;
public enum TransactionVersion {
  V0(0),
  V1(1);

  private int value;

  TransactionVersion(int value) {
    this.value = value;
  }

  public int getValue() {
    return value;
  }
}