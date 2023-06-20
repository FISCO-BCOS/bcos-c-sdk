package org.fisco.bcos.sdk.jni.utilities.tx;

public class TransactionBytes {
    private byte[] buffer;
    private int length;

    public byte[] getBuffer() {
        return buffer;
    }

    public void setBuffer(byte[] buffer) {
        this.buffer = buffer;
    }

    public int getLength() {
        return length;
    }

    public void setLength(int length) {
        this.length = length;
    }
}
