package org.fisco.bcos.sdk.jni.utilities.tx;

import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.JniLibLoader;

public class TransactionStructBuilderJniObj {

    static {
        JniLibLoader.loadJniLibrary();
    }

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
}
