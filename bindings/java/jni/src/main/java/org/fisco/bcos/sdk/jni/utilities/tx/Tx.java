package org.fisco.bcos.sdk.jni.utilities.tx;

public class Tx {
    private TxData transactionData;
    private TxBytes dataHash;
    private TxBytes signature;
    private TxBytes sender;
    private long importTime;
    private int attribute;
    private String extraData;

    public TxData getTransactionData() {
        return transactionData;
    }

    public void setTransactionData(TxData transactionData) {
        this.transactionData = transactionData;
    }

    public TxBytes getDataHash() {
        return dataHash;
    }

    public void setDataHash(TxBytes dataHash) {
        this.dataHash = dataHash;
    }

    public TxBytes getSignature() {
        return signature;
    }

    public void setSignature(TxBytes signature) {
        this.signature = signature;
    }

    public TxBytes getSender() {
        return sender;
    }

    public void setSender(TxBytes sender) {
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

