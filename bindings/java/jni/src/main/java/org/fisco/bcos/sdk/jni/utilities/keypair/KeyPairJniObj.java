package org.fisco.bcos.sdk.jni.utilities.keypair;

import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.JniLibLoader;

public class KeyPairJniObj {

  static {
    JniLibLoader.loadJniLibrary();
  }

  public static native long createJniKeyPair(int cryptoType) throws JniException;

  public static native long createHsmKeyPair(String hsmLibPath) throws JniException;

  public static native long createJniKeyPair(int cryptoType, byte[] priKeyBytes)
      throws JniException;

  public static native long createHsmKeyPair(byte[] priKeyBytes, String hsmLibPath)
      throws JniException;

  public static native long useHsmKeyPair(int keyIndex, String password, String hsmLibPath)
      throws JniException;

  public static native String getJniKeyPairAddress(long keyPair);

  public static native String getJniKeyPairPubKey(long keyPair);

  public static native String getJniKeyPairPrivateKey(long keyPair);

  public static native int getJniKeyPairCryptoType(long keyPair);

  public static native void destroyJniKeyPair(long keyPair);
}
