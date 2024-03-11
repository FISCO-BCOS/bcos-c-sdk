package org.fisco.bcos.sdk.jni.utilities.tx;

public class TransactionDataV2 extends TransactionDataV1 {
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
