package org.fisco.bcos.sdk.jni.utilities.tx;

public class TransactionV2 {
    private TransactionDataV2 transactionData;
    private TransactionBytes dataHash;
    private TransactionBytes signature;
    private TransactionBytes sender;
    private long importTime;
    private int attribute;
    private String extraData;

    public TransactionDataV2 getTransactionData() {
        return transactionData;
    }

    public void setTransactionData(TransactionDataV2 transactionData) {
        this.transactionData = transactionData;
    }

    public TransactionBytes getDataHash() {
        return dataHash;
    }

    public void setDataHash(TransactionBytes dataHash) {
        this.dataHash = dataHash;
    }

    public TransactionBytes getSignature() {
        return signature;
    }

    public void setSignature(TransactionBytes signature) {
        this.signature = signature;
    }

    public TransactionBytes getSender() {
        return sender;
    }

    public void setSender(TransactionBytes sender) {
        this.sender = sender;
    }

    public long getImportTime() {
        return importTime;
    }

    public void setImportTime(long importTime) {
        this.importTime = importTime;
    }

    public int getAttribute() {
        return attribute;
    }

    public void setAttribute(int attribute) {
        this.attribute = attribute;
    }

    public String getExtraData() {
        return extraData;
    }

    public void setExtraData(String extraData) {
        this.extraData = extraData;
    }
}
