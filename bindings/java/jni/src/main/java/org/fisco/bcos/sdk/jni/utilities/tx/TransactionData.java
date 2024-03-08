package org.fisco.bcos.sdk.jni.utilities.tx;

import org.fisco.bcos.sdk.jni.common.JniException;

public class TransactionData {
    protected int version;
    protected long blockLimit;
    protected String chainId;
    protected String groupId;
    protected String nonce;
    protected String to;
    protected String abi;
    protected byte[] input;

    public int getVersion() {
        return version;
    }

    public void setVersion(int version) {
        this.version = version;
    }

    public TransactionData buildVersion(int version) {
        this.version = version;
        return this;
    }

    public long getBlockLimit() {
        return blockLimit;
    }

    public void setBlockLimit(long blockLimit) {
        this.blockLimit = blockLimit;
    }

    public TransactionData buildBlockLimit(long blockLimit) {
        this.blockLimit = blockLimit;
        return this;
    }

    public String getChainId() {
        return chainId;
    }

    public void setChainId(String chainId) {
        this.chainId = chainId;
    }

    public TransactionData buildChainId(String chainId) {
        this.chainId = chainId;
        return this;
    }

    public String getGroupId() {
        return groupId;
    }

    public void setGroupId(String groupId) {
        this.groupId = groupId;
    }

    public TransactionData buildGroupId(String groupId) {
        this.groupId = groupId;
        return this;
    }

    public String getNonce() {
        return nonce;
    }

    public void setNonce(String nonce) {
        this.nonce = nonce;
    }

    public TransactionData buildNonce(String nonce) {
        this.nonce = nonce;
        return this;
    }

    public String getTo() {
        return to;
    }

    public void setTo(String to) {
        this.to = to;
    }

    public TransactionData buildTo(String to) {
        this.to = to;
        return this;
    }

    public String getAbi() {
        return abi;
    }

    public void setAbi(String abi) {
        this.abi = abi;
    }

    public TransactionData buildAbi(String abi) {
        this.abi = abi;
        return this;
    }

    public byte[] getInput() {
        return input;
    }

    public void setInput(byte[] input) {
        this.input = input;
    }

    public TransactionData buildInput(byte[] input) {
        this.input = input;
        return this;
    }

    public String calculateHash(int cryptoType) throws JniException {
        return TransactionStructBuilderJniObj.calcTransactionDataStructHash(cryptoType, this);
    }
}
