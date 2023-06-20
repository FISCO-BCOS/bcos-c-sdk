package org.fisco.bcos.sdk.jni.utilities.tx;

import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.JniLibLoader;

public class TransactionStructBuilderJniObj {

    static {
        JniLibLoader.loadJniLibrary();
    }

    /**
     * @param groupID    group id
     * @param chainID    chain id
     * @param to         contract address, which calling to
     * @param input      tx hex input
     * @param abi        ABI json string, should add it when deploy
     * @param blockLimit block limit
     * @return Transaction Data struct
     */
    public static native TransactionData createTransactionDataStructWithHexInput(
            String groupID, String chainID, String to, String input, String abi, long blockLimit)
            throws JniException;

    /**
     * @param groupID    group id
     * @param chainID    chain id
     * @param to         contract address, which calling to
     * @param bytesInput tx bytes input
     * @param abi        ABI json string, should add it when deploy
     * @param blockLimit block limit
     * @return Transaction Data struct
     */
    public static native TransactionData createTransactionDataStructWithBytes(
            String groupID, String chainID, String to, byte[] bytesInput, String abi, long blockLimit)
            throws JniException;

    /**
     * @param transactionData Transaction Data struct
     */
    public static native void destroyTransactionDataStruct(TransactionData transactionData)
            throws JniException;

    /**
     * @param transactionData Transaction Data struct
     * @return Hexed Transaction Data
     */
    public static native String encodeTransactionDataStruct(TransactionData transactionData)
            throws JniException;

    /**
     * @param transactionData Transaction Data struct
     * @return Json Transaction Data
     */
    public static native String encodeTransactionDataStructToJson(TransactionData transactionData)
            throws JniException;

    /**
     * @param transactionDataHex transactionData hex string
     * @return TxData
     */
    public static native TransactionData decodeTransactionDataStruct(String transactionDataHex)
            throws JniException;

    /**
     * @param transactionDataJson transactionData json string
     * @return TxData
     */
    public static native TransactionData decodeTransactionDataStructWithJson(String transactionDataJson)
            throws JniException;

    /**
     * @param cryptoType      crypto type
     * @param transactionData Transaction Data struct
     * @return Hash hex string
     */
    public static native String calcTransactionDataStructHash(int cryptoType, TransactionData transactionData)
            throws JniException;

    /**
     * @param transactionData
     * @param signature
     * @param transactionDataHash
     * @param attribute
     * @param extraData
     * @return Transaction Struct
     */
    public static native Transaction createTransactionStruct(
            TransactionData transactionData,
            String signature,
            String transactionDataHash,
            int attribute,
            String extraData)
            throws JniException;

    /**
     * @param transaction Transaction struct
     */
    public static native void destroyTransactionStruct(Transaction transaction) throws JniException;

    /**
     * @param transactionData
     * @param signature
     * @param transactionDataHash
     * @param attribute
     * @param extraData
     * @return signedTransaction string
     */
    public static native String createEncodedTransaction(
            TransactionData transactionData,
            String signature,
            String transactionDataHash,
            int attribute,
            String extraData)
            throws JniException;

    /**
     * @param transaction Transaction struct
     * @return Hexed Transaction
     */
    public static native String encodeTransactionStruct(Transaction transaction) throws JniException;

    /**
     * @param transaction Transaction struct
     * @return Json Transaction
     */
    public static native String encodeTransactionStructToJson(Transaction transaction) throws JniException;

    /**
     * @param transactionHex transaction hex string
     * @return Tx
     */
    public static native Transaction decodeTransactionStruct(String transactionHex) throws JniException;

    /**
     * @param transactionJson transaction json string
     * @return Tx
     */
    public static native Transaction decodeTransactionStructWithJson(String transactionJson)
            throws JniException;
}
