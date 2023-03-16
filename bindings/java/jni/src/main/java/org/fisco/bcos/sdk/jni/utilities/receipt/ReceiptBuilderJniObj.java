package org.fisco.bcos.sdk.jni.utilities.receipt;

import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.JniLibLoader;

public class ReceiptBuilderJniObj {
  static {
    JniLibLoader.loadJniLibrary();
  }

  /**
   * @param gasUsed gas used
   * @param contractAddress contract address
   * @param output receipt output data
   * @param blockNumber block number
   * @return Receipt Data pointer
   */
  public static native long createReceiptData(
      String gasUsed, String contractAddress, String output, long blockNumber) throws JniException;

  /**
   * Create a Receipt Data object with json
   *
   * @param json version: number gasUsed: string contractAddress: string status: number output: hex
   *     string logEntries: array<LogEntry> blockNumber: number logEntry: address: string topic:
   *     array<hex string> data: hex string
   * @return Receipt Data pointer
   */
  public static native long createReceiptDataWithJson(String json) throws JniException;

  /** @param ReceiptData Receipt Data pointer */
  public static native void destroyReceiptData(long ReceiptData) throws JniException;

  /**
   * @param ReceiptData Receipt Data pointer
   * @return encoded string
   */
  public static native String encodeReceiptData(long ReceiptData) throws JniException;

  /**
   * @param ReceiptDataHex ReceiptData hex string
   * @return ReceiptData json string
   * @throws JniException throw when cpp sdk exception
   */
  public static native String decodeReceiptDataToJsonObj(String ReceiptDataHex) throws JniException;

  /**
   * @param cryptoType crypto type
   * @param ReceiptData Receipt Data pointer
   * @return Hash hex string
   */
  public static native String calcReceiptDataHash(int cryptoType, long ReceiptData)
      throws JniException;
}
