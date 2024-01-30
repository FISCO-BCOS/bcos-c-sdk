package org.fisco.bcos.sdk.jni.utilities.tx;

public class Transaction {
  private TransactionData transactionData;
  private byte[] dataHash;
  private byte[] signature;
  private byte[]  sender;
  private long importTime;
  private int attribute;
  private String extraData;

  public TransactionData getTransactionData() {
    return transactionData;
  }

  public void setTransactionData(TransactionData transactionData) {
    this.transactionData = transactionData;
  }

  public byte[] getDataHash() {
    return dataHash;
  }

  public void setDataHash(byte[] dataHash) {
    this.dataHash = dataHash;
  }

  public byte[] getSignature() {
    return signature;
  }

  public void setSignature(byte[] signature) {
    this.signature = signature;
  }

  public byte[] getSender() {
    return sender;
  }

  public void setSender(byte[] sender) {
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
