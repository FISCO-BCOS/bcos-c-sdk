package org.fisco.bcos.sdk.jni.utilities.tx;

public enum TransactionVersion {
  V0(0),
  V1(1),

  V2(2);

  private int value;

  TransactionVersion(int value) {
    this.value = value;
  }

  public int getValue() {
    return value;
  }

  public static TransactionVersion fromInt(int value) {
    switch (value) {
      case 0:
        return V0;
      case 1:
        return V1;
      case 2:
        return V2;
      default:
        return V0;
    }
  }
}
