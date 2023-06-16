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
     * @param data       tx input data
     * @param abi        ABI json string, should add it when deploy
     * @param blockLimit block limit
     * @return Transaction Data struct
     */
    public static native TxData createTransactionDataStruct(
            String groupID, String chainID, String to, String data, String abi, long blockLimit)
            throws JniException;

    /**
     * @param transactionData Transaction Data struct
     */
    public static native void destroyTransactionDataStruct(TxData transactionData)
            throws JniException;

    /**
     * @param transactionData Transaction Data struct
     * @return Hexed Transaction Data
     */
    public static native String encodeTransactionDataStruct(TxData transactionData)
            throws JniException;

    /**
     * @param transactionData Transaction Data struct
     * @return Json Transaction Data
     */
    public static native String encodeTransactionDataStructToJson(TxData transactionData)
            throws JniException;

    /**
     * @param transactionDataHex transactionData hex string
     * @return TxData
     */
    public static native TxData decodeTransactionDataStruct(String transactionDataHex)
            throws JniException;

    /**
     * @param transactionDataJson transactionData json string
     * @return TxData
     */
    public static native TxData decodeTransactionDataStructWithJson(String transactionDataJson)
            throws JniException;

    /**
     * @param cryptoType      crypto type
     * @param transactionData Transaction Data struct
     * @return Hash hex string
     */
    public static native String calcTransactionDataStructHash(int cryptoType, TxData transactionData)
            throws JniException;

    /**
     * @param transactionData
     * @param signature
     * @param transactionDataHash
     * @param attribute
     * @param extraData
     * @return Transaction Struct
     */
    public static native Tx createTransactionStruct(
            TxData transactionData,
            String signature,
            String transactionDataHash,
            int attribute,
            String extraData)
            throws JniException;

    /**
     * @param transaction Transaction struct
     */
    public static native void destroyTransactionStruct(Tx transaction) throws JniException;

    /**
     * @param transactionData
     * @param signature
     * @param transactionDataHash
     * @param attribute
     * @param extraData
     * @return signedTransaction string
     */
    public static native String createTransaction(
            TxData transactionData,
            String signature,
            String transactionDataHash,
            int attribute,
            String extraData)
            throws JniException;

    /**
     * @param transaction Transaction struct
     * @return Hexed Transaction
     */
    public static native String encodeTransactionStruct(Tx transaction) throws JniException;

    /**
     * @param transaction Transaction struct
     * @return Json Transaction
     */
    public static native String encodeTransactionStructToJson(Tx transaction) throws JniException;

    /**
     * @param transactionHex transaction hex string
     * @return Tx
     */
    public static native Tx decodeTransactionStruct(String transactionHex) throws JniException;

    /**
     * @param transactionJson transaction json string
     * @return Tx
     */
    public static native Tx decodeTransactionStructWithJson(String transactionJson)
            throws JniException;
}
