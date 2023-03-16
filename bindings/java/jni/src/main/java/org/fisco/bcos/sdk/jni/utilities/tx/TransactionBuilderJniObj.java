package org.fisco.bcos.sdk.jni.utilities.tx;

import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.JniLibLoader;

public class TransactionBuilderJniObj {

  static {
    JniLibLoader.loadJniLibrary();
  }

  /**
   * @param groupID group id
   * @param chainID chain id
   * @param to contract address, which calling to
   * @param data tx input data
   * @param abi ABI json string, should add it when deploy
   * @param blockLimit block limit
   * @return Transaction Data pointer
   */
  public static native long createTransactionData(
      String groupID, String chainID, String to, String data, String abi, long blockLimit)
      throws JniException;

  /**
   * Create a Transaction Data object with json
   *
   * @param json version:number groupID:string chainID:string to:string data:hex string abi:string
   *     blockLimit:number nonce:string
   * @return Transaction Data pointer
   */
  public static native long createTransactionDataWithJson(String json) throws JniException;

  /** @param transactionData Transaction Data pointer */
  public static native void destroyTransactionData(long transactionData) throws JniException;

  /**
   * @param transactionData Transaction Data pointer
   * @return encoded string
   */
  public static native String encodeTransactionData(long transactionData) throws JniException;

  /**
   * @param transactionDataHex transactionData hex string
   * @return transactionData json string
   * @throws JniException throw when cpp sdk exception
   */
  public static native String decodeTransactionDataToJsonObj(String transactionDataHex)
      throws JniException;

  /**
   * @param cryptoType crypto type
   * @param transactionData Transaction Data pointer
   * @return Hash hex string
   */
  public static native String calcTransactionDataHash(int cryptoType, long transactionData)
      throws JniException;

  /**
   * @param keyPair KeyPair pointer
   * @param transactionDataHash transaction data hash hex string
   * @return signed tx string
   */
  public static native String signTransactionDataHash(long keyPair, String transactionDataHash)
      throws JniException;

  /**
   * @param transactionData Transaction Data pointer
   * @param transactionDataHashSignedData signed tx data string
   * @param transactionDataHash transaction data hash hex string
   * @param attribute TransactionAttribute
   * @return signed tx string
   */
  public static native String createSignedTransaction(
      long transactionData,
      String transactionDataHashSignedData,
      String transactionDataHash,
      int attribute)
      throws JniException;

  /**
   * @param transactionData
   * @param transactionDataHashSignedData
   * @param transactionDataHash
   * @param attribute
   * @param extraData
   * @return
   */
  public static native String createSignedTransaction(
      long transactionData,
      String transactionDataHashSignedData,
      String transactionDataHash,
      int attribute,
      String extraData)
      throws JniException;

  /**
   * @param keyPair
   * @param groupID
   * @param chainID
   * @param to
   * @param data
   * @param blockLimit
   * @param attribute
   * @return
   * @throws JniException
   */
  public static native TxPair createSignedTransaction(
      long keyPair,
      String groupID,
      String chainID,
      String to,
      String data,
      String abi,
      long blockLimit,
      int attribute)
      throws JniException;

  /**
   * @param keyPair
   * @param groupID
   * @param chainID
   * @param to
   * @param data
   * @param blockLimit
   * @param attribute
   * @param extraData
   * @return
   * @throws JniException
   */
  public static native TxPair createSignedTransaction(
      long keyPair,
      String groupID,
      String chainID,
      String to,
      String data,
      String abi,
      long blockLimit,
      int attribute,
      String extraData)
      throws JniException;
}
