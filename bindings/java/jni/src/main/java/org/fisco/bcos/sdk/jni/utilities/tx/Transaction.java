package org.fisco.bcos.sdk.jni.utilities.tx;

import org.fisco.bcos.sdk.jni.common.JniException;

public class Transaction {
    private TransactionData transactionData;
    private byte[] dataHash;
    private byte[] signature;
    private byte[] sender;
    private long importTime;
    private int attribute;
    private String extraData;

    public TransactionData getTransactionData() {
        return transactionData;
    }

    public void setTransactionData(TransactionData transactionData) {
        this.transactionData = transactionData;
    }

    public Transaction buildTransactionData(TransactionData transactionData) {
        this.transactionData = transactionData;
        return this;
    }

    public byte[] getDataHash() {
        return dataHash;
    }

    public void setDataHash(byte[] dataHash) {
        this.dataHash = dataHash;
    }

    public Transaction buildDataHash(byte[] dataHash) {
        this.dataHash = dataHash;
        return this;
    }

    public byte[] getSignature() {
        return signature;
    }

    public void setSignature(byte[] signature) {
        this.signature = signature;
    }

    public Transaction buildSignature(byte[] signature) {
        this.signature = signature;
        return this;
    }

    public byte[] getSender() {
        return sender;
    }

    public void setSender(byte[] sender) {
        this.sender = sender;
    }

    public Transaction buildSender(byte[] sender) {
        this.sender = sender;
        return this;
    }

    public long getImportTime() {
        return importTime;
    }

    public void setImportTime(long importTime) {
        this.importTime = importTime;
    }

    public Transaction buildImportTime(long importTime) {
        this.importTime = importTime;
        return this;
    }

    public int getAttribute() {
        return attribute;
    }

    public void setAttribute(int attribute) {
        this.attribute = attribute;
    }

    public Transaction buildAttribute(int attribute) {
        this.attribute = attribute;
        return this;
    }

    public String getExtraData() {
        return extraData;
    }

    public void setExtraData(String extraData) {
        this.extraData = extraData;
    }

    public Transaction buildExtraData(String extraData) {
        this.extraData = extraData;
        return this;
    }

    public String encode() throws JniException {
        return TransactionStructBuilderJniObj.encodeTransactionStruct(this);
    }
}
