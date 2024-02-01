package org.fisco.bcos.sdk.jni.utilities.tx;

public class TransactionDataV1 extends TransactionData {
  protected String value;
  protected String gasPrice;
  protected long gasLimit;
  protected String maxFeePerGas;
  protected String maxPriorityFeePerGas;

  public String getValue() {
    return value;
  }

  public void setValue(String value) {
    this.value = value;
  }

  public String getGasPrice() {
    return gasPrice;
  }

  public void setGasPrice(String gasPrice) {
    this.gasPrice = gasPrice;
  }

  public long getGasLimit() {
    return gasLimit;
  }

  public void setGasLimit(long gasLimit) {
    this.gasLimit = gasLimit;
  }

  public String getMaxFeePerGas() {
    return maxFeePerGas;
  }

  public void setMaxFeePerGas(String maxFeePerGas) {
    this.maxFeePerGas = maxFeePerGas;
  }

  public String getMaxPriorityFeePerGas() {
    return maxPriorityFeePerGas;
  }

  public void setMaxPriorityFeePerGas(String maxPriorityFeePerGas) {
    this.maxPriorityFeePerGas = maxPriorityFeePerGas;
  }
}
