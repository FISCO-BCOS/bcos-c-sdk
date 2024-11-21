package org.fisco.bcos.sdk.jni.utilities.keypair;

import java.util.Arrays;

public class JniKeyPair {
    private byte[] privateKey;
    private byte[] publicKey;
    // 0: ECDSA, 1: SM2
    private int cryptoType = 0;

    public JniKeyPair() {}

    public JniKeyPair(byte[] privateKey, byte[] publicKey, int cryptoType) {
        this.privateKey = privateKey;
        this.publicKey = publicKey;
        this.cryptoType = cryptoType;
    }

    public byte[] getPrivateKey() {
        return privateKey;
    }

    public void setPrivateKey(byte[] privateKey) {
        this.privateKey = privateKey;
    }

    public byte[] getPublicKey() {
        return publicKey;
    }

    public void setPublicKey(byte[] publicKey) {
        this.publicKey = publicKey;
    }

    public int getCryptoType() {
        return cryptoType;
    }

    public void setCryptoType(int cryptoType) {
        this.cryptoType = cryptoType;
    }

    @Override
    public String toString() {
        return "JniKeyPair{"
                + "publicKey="
                + Arrays.toString(publicKey)
                + ", cryptoType="
                + cryptoType
                + '}';
    }
}
