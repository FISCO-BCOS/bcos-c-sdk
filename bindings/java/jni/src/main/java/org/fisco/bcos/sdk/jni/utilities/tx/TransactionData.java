package org.fisco.bcos.sdk.jni.utilities.tx;

public class TransactionData {
    private int version;
    private long blockLimit;
    private String chainId;
    private String groupId;
    private String nonce;
    private String to;
    private String abi;
    private TransactionBytes input;

    public int getVersion() {
        return version;
    }

    public void setVersion(int version) {
        this.version = version;
    }

    public long getBlockLimit() {
        return blockLimit;
    }

    public void setBlockLimit(long blockLimit) {
        this.blockLimit = blockLimit;
    }

    public String getChainId() {
        return chainId;
    }

    public void setChainId(String chainId) {
        this.chainId = chainId;
    }

    public String getGroupId() {
        return groupId;
    }

    public void setGroupId(String groupId) {
        this.groupId = groupId;
    }

    public String getNonce() {
        return nonce;
    }

    public void setNonce(String nonce) {
        this.nonce = nonce;
    }

    public String getTo() {
        return to;
    }

    public void setTo(String to) {
        this.to = to;
    }

    public String getAbi() {
        return abi;
    }

    public void setAbi(String abi) {
        this.abi = abi;
    }

    public TransactionBytes getInput() {
        return input;
    }

    public void setInput(TransactionBytes input) {
        this.input = input;
    }
}
