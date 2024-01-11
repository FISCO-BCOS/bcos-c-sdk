package org.fisco.bcos.sdk.jni.utilities.tx;

import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.JniLibLoader;

public class TransactionStructBuilderV2JniObj {

  static {
    JniLibLoader.loadJniLibrary();
  }

  /**
   * @param TransactionDataV2 Transaction Data struct
   * @return Hexed Transaction Data
   */
  public static native String encodeTransactionDataStructV2(TransactionDataV2 transactionData)
      throws JniException;

  /**
   * @param TransactionDataV2 Transaction Data struct
   * @return Json Transaction Data
   */
  public static native String encodeTransactionDataStructToJsonV2(TransactionDataV2 transactionData)
      throws JniException;

  /**
   * @param transactionDataHex transactionData hex string
   * @return TxData
   */
  public static native TransactionDataV2 decodeTransactionDataStructV2(String transactionDataHex)
      throws JniException;

  /**
   * @param cryptoType crypto type
   * @param transactionData Transaction Data struct
   * @return Hash hex string
   */
  public static native String calcTransactionDataStructHashV2(
      int cryptoType, TransactionDataV2 transactionData) throws JniException;

  /**
   * @param transactionData
   * @param signature
   * @param transactionDataHash
   * @param attribute
   * @param extraData
   * @return signedTransaction string
   */
  public static native String createEncodedTransactionV2(
      TransactionDataV2 transactionData,
      String signature,
      String transactionDataHash,
      int attribute,
      String extraData)
      throws JniException;

  /**
   * @param transaction Transaction struct
   * @return Hexed Transaction
   */
  public static native String encodeTransactionStructV2(TransactionV2 transaction)
      throws JniException;

  /**
   * @param transaction Transaction struct
   * @return Json Transaction
   */
  public static native String encodeTransactionStructToJsonV2(TransactionV2 transaction)
      throws JniException;

  /**
   * @param transactionHex transaction hex string
   * @return Tx
   */
  public static native TransactionV2 decodeTransactionStructV2(String transactionHex)
      throws JniException;
}
