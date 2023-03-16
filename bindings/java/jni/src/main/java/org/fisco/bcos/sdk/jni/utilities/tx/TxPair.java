package org.fisco.bcos.sdk.jni.utilities.tx;

public class TxPair {
  private String txHash;
  private String signedTx;

  public TxPair() {}

  public TxPair(String txHash, String signedHash) {
    this.txHash = txHash;
    this.signedTx = signedHash;
  }

  public String getTxHash() {
    return txHash;
  }

  public void setTxHash(String txHash) {
    this.txHash = txHash;
  }

  public String getSignedTx() {
    return signedTx;
  }

  public void setSignedTx(String signedTx) {
    this.signedTx = signedTx;
  }
}
