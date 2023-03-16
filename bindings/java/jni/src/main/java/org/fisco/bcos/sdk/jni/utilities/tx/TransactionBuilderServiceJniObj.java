package org.fisco.bcos.sdk.jni.utilities.tx;

import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.JniLibLoader;

public class TransactionBuilderServiceJniObj {

  static {
    JniLibLoader.loadJniLibrary();
  }

  /**
   * @param sdk
   * @param groupID
   * @return
   * @throws JniException
   */
  public static native long createTransactionBuilderService(long sdk, String groupID)
      throws JniException;

  /**
   * @param txBuilderService
   * @throws JniException
   */
  public static native void destroyTransactionBuilderService(long txBuilderService)
      throws JniException;

  /**
   * @param txBuilderService
   * @param to
   * @param data
   * @param abi
   * @return
   * @throws JniException
   */
  public static native long createTransactionData(
      long txBuilderService, String to, String data, String abi) throws JniException;

  /**
   * @param txBuilderService
   * @param keyPair
   * @param to
   * @param data
   * @param abi
   * @param attribute
   * @return
   * @throws JniException
   */
  public static native TxPair createSignedTransaction(
      long txBuilderService, long keyPair, String to, String data, String abi, int attribute)
      throws JniException;
}
