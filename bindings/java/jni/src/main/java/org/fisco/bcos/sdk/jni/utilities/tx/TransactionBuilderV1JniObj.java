package org.fisco.bcos.sdk.jni.utilities.tx;

import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.JniLibLoader;

public class TransactionBuilderV1JniObj extends TransactionBuilderJniObj {

  static {
    JniLibLoader.loadJniLibrary();
  }

  /**
   * create transaction data with version 1 by default
   *
   * @param groupID group id
   * @param chainID chain id
   * @param to contract address, if it is a contract creation transaction, it can be empty
   * @param nonce random number to avoid duplicate transactions, if it is empty or null, then it
   *     will generate one
   * @param input encoded contract method and params
   * @param abi contract abi, only create contract need
   * @param blockLimit block limit
   * @param value transfer value
   * @param gasPrice gas price
   * @param gasLimit gas limit
   * @return void* transaction data pointer, you should release it after use, check
   *     `destroyTransactionData`
   * @apiNote version 1 transaction only supported in FISCO BCOS 3.6.0 and later
   */
  public static native long createTransactionData(
      String groupID,
      String chainID,
      String to,
      String nonce,
      byte[] input,
      String abi,
      long blockLimit,
      String value,
      String gasPrice,
      long gasLimit)
      throws JniException;

  /**
   * create eip1559 transaction data with version 1 by default
   *
   * @param groupID group id
   * @param chainID chain id
   * @param to contract address, if it is a contract creation transaction, it can be empty
   * @param nonce random number to avoid duplicate transactions, if it is empty or null, then it
   *     will generate one
   * @param input encoded contract method and params
   * @param abi contract abi, only create contract need
   * @param blockLimit block limit
   * @param value transfer value
   * @param gasLimit gas limit
   * @param maxFeePerGas max fee per gas
   * @param maxPriorityFeePerGas max priority fee per gas
   * @return void* transaction data pointer, you should release it after use, check
   *     `destroyTransactionData`
   * @apiNote version 1 transaction only supported in FISCO BCOS 3.6.0 and later
   */
  public static native long createEIP1559TransactionData(
      String groupID,
      String chainID,
      String to,
      String nonce,
      byte[] input,
      String abi,
      long blockLimit,
      String value,
      long gasLimit,
      String maxFeePerGas,
      String maxPriorityFeePerGas)
      throws JniException;

  /**
   * create transaction data with full fields
   *
   * @param cryptoType 0: keccak256, 1: sm3
   * @param version tx version, if version==1, then enable
   *     (value,gasPrice,gasLimit,maxFeePerGas,maxPriorityFeePerGas) fields
   * @param groupID group id
   * @param chainID chain id
   * @param to contract address, if it is a contract creation transaction, it can be empty
   * @param nonce nonce, random number to avoid duplicate transactions
   * @param input encoded contract method and params
   * @param abi contract abi, only create contract need
   * @param blockLimit block limit
   * @param value transfer value
   * @param gasPrice gas price
   * @param gasLimit gas limit
   * @param maxFeePerGas max fee per gas
   * @param maxPriorityFeePerGas max priority fee per gas
   * @return const char* transaction data hash hex string
   * @apiNote version 1 transaction only supported in FISCO BCOS 3.6.0 and later
   */
  public static native String calcTransactionDataHashWithFullFields(
      int cryptoType,
      TransactionVersion version,
      String groupID,
      String chainID,
      String to,
      String nonce,
      byte[] input,
      String abi,
      long blockLimit,
      String value,
      String gasPrice,
      long gasLimit,
      String maxFeePerGas,
      String maxPriorityFeePerGas)
      throws JniException;

  /**
   * create transaction data with json string
   *
   * @param cryptoType 0: keccak256, 1: sm3
   * @param json transaction data json string
   * @return const char* transaction data hash hex string
   * @throws JniException if lack of some required fields, or some fields are invalid
   * @apiNote version 1 transaction only supported in FISCO BCOS 3.6.0 and later
   */
  public static native String calcTransactionDataHashWithJson(int cryptoType, String json)
      throws JniException;

  /**
   * create encoded transaction data with external signature
   *
   * @param signature signature bytes array, if ECDSA, it is r||s||v, if SM2, it is r||s||pk
   * @param txDataHash transactionData hash hex string
   * @param version tx version, only support 0 and 1 now, if version==1, then enable
   *     (value,gasPrice,gasLimit,maxFeePerGas,maxPriorityFeePerGas) fields
   * @param groupID group id
   * @param chainID chain id
   * @param to contract address, if it is a contract creation transaction, it can be empty
   * @param nonce nonce, random number to avoid duplicate transactions
   * @param input encoded contract method and params
   * @param abi contract abi, only create contract need
   * @param blockLimit block limit
   * @param value transfer value
   * @param gasPrice gas price
   * @param gasLimit gas limit
   * @param maxFeePerGas max fee per gas
   * @param maxPriorityFeePerGas max priority fee per gas
   * @param attribute transaction attribute
   * @param extraData extra data in transaction
   * @return const char* encoded transaction hex string
   * @apiNote version 1 transaction only supported in FISCO BCOS 3.6.0 and later
   */
  public static native String createSignedTransactionWithSignature(
      byte[] signature,
      String txDataHash,
      TransactionVersion version,
      String groupID,
      String chainID,
      String to,
      String nonce,
      byte[] input,
      String abi,
      long blockLimit,
      String value,
      String gasPrice,
      long gasLimit,
      String maxFeePerGas,
      String maxPriorityFeePerGas,
      int attribute,
      String extraData)
      throws JniException;

  /**
   * create transaction with full fields, with version 1 by default
   *
   * @param keyPair key pair pointer
   * @param groupID group id
   * @param chainID chain id
   * @param to contract address, if it is a contract creation transaction, it can be empty
   * @param nonce random number to avoid duplicate transactions, if it is empty or null, then it
   *     will generate one
   * @param input encoded contract method and params
   * @param abi contract abi, only create contract need
   * @param blockLimit block limit
   * @param value transfer value
   * @param gasPrice gas price
   * @param gasLimit gas limit
   * @param attribute transaction attribute
   * @param extraData extra data in transaction
   * @return TxPair <txHash, encodedSignTx>
   * @apiNote version 1 transaction only supported in FISCO BCOS 3.6.0 and later
   */
  public static native TxPair createSignedTransactionWithFullFields(
      long keyPair,
      String groupID,
      String chainID,
      String to,
      String nonce,
      byte[] input,
      String abi,
      long blockLimit,
      String value,
      String gasPrice,
      long gasLimit,
      int attribute,
      String extraData)
      throws JniException;

  /**
   * create eip1559 transaction with full fields, with version 1 by default
   *
   * @param keyPair key pair pointer
   * @param groupID group id
   * @param chainID chain id
   * @param to contract address, if it is a contract creation transaction, it can be empty
   * @param nonce random number to avoid duplicate transactions, if it is empty or null, then it
   *     will generate one
   * @param input encoded contract method and params
   * @param abi contract abi, only create contract need
   * @param blockLimit block limit
   * @param value transfer value
   * @param gasLimit gas limit
   * @param maxFeePerGas max fee per gas
   * @param maxPriorityFeePerGas max priority fee per gas
   * @param attribute transaction attribute
   * @param extraData extra data in transaction
   * @return TxPair <txHash, encodedSignTx>
   * @apiNote version 1 transaction only supported in FISCO BCOS 3.6.0 and later
   */
  public static native TxPair createSignedEIP1559TransactionWithFullFields(
      long keyPair,
      String groupID,
      String chainID,
      String to,
      String nonce,
      byte[] input,
      String abi,
      long blockLimit,
      String value,
      String maxFeePerGas,
      String maxPriorityFeePerGas,
      long gasLimit,
      int attribute,
      String extraData)
      throws JniException;
}
