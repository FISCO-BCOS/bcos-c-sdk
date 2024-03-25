package org.fisco.bcos.sdk.jni.utilities.tx;

public class TransactionDataV2 extends TransactionDataV1 {

    public TransactionDataV2() {
        super();
    }

    public TransactionDataV2(TransactionDataV1 data) {
        this.version = data.version;
        this.blockLimit = data.blockLimit;
        this.chainId = data.chainId;
        this.groupId = data.groupId;
        this.nonce = data.nonce;
        this.to = data.to;
        this.abi = data.abi;
        this.input = data.input;
        this.value = data.value;
        this.gasPrice = data.gasPrice;
        this.gasLimit = data.gasLimit;
        this.maxFeePerGas = data.maxFeePerGas;
        this.maxPriorityFeePerGas = data.maxPriorityFeePerGas;
    }

    byte[] extension;

    public byte[] getExtension() {
        return extension;
    }

    public void setExtension(byte[] extension) {
        this.extension = extension;
    }

    public TransactionDataV2 buildExtension(byte[] extension) {
        this.extension = extension;
        return this;
    }
}
