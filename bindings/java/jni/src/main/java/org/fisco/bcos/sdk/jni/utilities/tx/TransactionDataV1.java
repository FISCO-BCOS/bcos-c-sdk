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

    public TransactionDataV1 buildValue(String value) {
        this.value = value;
        return this;
    }

    public String getGasPrice() {
        return gasPrice;
    }

    public void setGasPrice(String gasPrice) {
        this.gasPrice = gasPrice;
    }

    public TransactionDataV1 buildGasPrice(String gasPrice) {
        this.gasPrice = gasPrice;
        return this;
    }

    public long getGasLimit() {
        return gasLimit;
    }

    public void setGasLimit(long gasLimit) {
        this.gasLimit = gasLimit;
    }

    public TransactionDataV1 buildGasLimit(long gasLimit) {
        this.gasLimit = gasLimit;
        return this;
    }

    public String getMaxFeePerGas() {
        return maxFeePerGas;
    }

    public void setMaxFeePerGas(String maxFeePerGas) {
        this.maxFeePerGas = maxFeePerGas;
    }

    public TransactionDataV1 buildMaxFeePerGas(String maxFeePerGas) {
        this.maxFeePerGas = maxFeePerGas;
        return this;
    }

    public String getMaxPriorityFeePerGas() {
        return maxPriorityFeePerGas;
    }

    public void setMaxPriorityFeePerGas(String maxPriorityFeePerGas) {
        this.maxPriorityFeePerGas = maxPriorityFeePerGas;
    }

    public TransactionDataV1 buildMaxPriorityFeePerGas(String maxPriorityFeePerGas) {
        this.maxPriorityFeePerGas = maxPriorityFeePerGas;
        return this;
    }
}
