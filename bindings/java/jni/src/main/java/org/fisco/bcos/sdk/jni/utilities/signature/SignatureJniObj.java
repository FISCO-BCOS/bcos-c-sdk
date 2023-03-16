package org.fisco.bcos.sdk.jni.utilities.signature;

import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.JniLibLoader;

public class SignatureJniObj {

  static {
    JniLibLoader.loadJniLibrary();
  }

  public static native byte[] sign(long keyPair, String transactionDataHash, String hsmLibPath)
      throws JniException;

  public static native boolean verify(
      int cryptoType,
      byte[] priKeyBytes,
      String transactionDataHash,
      String transactionDataHashSignedData,
      String hsmLibPath)
      throws JniException;
}
