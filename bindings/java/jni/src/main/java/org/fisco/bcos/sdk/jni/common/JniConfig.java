package org.fisco.bcos.sdk.jni.common;

import java.util.List;

/** Config items need by fisco-bcos-sdk-jni module */
public class JniConfig {

  /** thread pool size for network send/recv/handle message */
  private int threadPoolSize = 4;

  private int reconnectPeriodMs = 20000;
  private int heartbeatPeriodMs = 20000;
  private int messageTimeoutMs = 10000;

  /** if disable the ssl connection */
  private boolean disableSsl = false;
  /** ssl or sm_ssl */
  private String sslType;
  /** ssl cert config items */
  private CertConfig certConfig;
  /** sm cert config items */
  private SMCertConfig smCertConfig;
  /** connected peers list */
  private List<String> peers;

  /** cert for sm ssl connection */
  public static class CertConfig {
    private String caCert;
    private String nodeKey;
    private String nodeCert;

    public String getCaCert() {
      return caCert;
    }

    public void setCaCert(String caCert) {
      this.caCert = caCert;
    }

    public String getNodeKey() {
      return nodeKey;
    }

    public void setNodeKey(String nodeKey) {
      this.nodeKey = nodeKey;
    }

    public String getNodeCert() {
      return nodeCert;
    }

    public void setNodeCert(String nodeCert) {
      this.nodeCert = nodeCert;
    }

    @Override
    public String toString() {
      return "CertConfig{"
          + "caCert='"
          + caCert
          + '\''
          + ", nodeKey='"
          + nodeKey
          + '\''
          + ", nodeCert='"
          + nodeCert
          + '\''
          + '}';
    }
  }

  /** cert for sm ssl connection */
  public static class SMCertConfig {
    private String caCert;
    private String nodeCert;
    private String nodeKey;
    private String enNodeCert;
    private String enNodeKey;

    public String getCaCert() {
      return caCert;
    }

    public void setCaCert(String caCert) {
      this.caCert = caCert;
    }

    public String getNodeCert() {
      return nodeCert;
    }

    public void setNodeCert(String nodeCert) {
      this.nodeCert = nodeCert;
    }

    public String getNodeKey() {
      return nodeKey;
    }

    public void setNodeKey(String nodeKey) {
      this.nodeKey = nodeKey;
    }

    public String getEnNodeCert() {
      return enNodeCert;
    }

    public void setEnNodeCert(String enNodeCert) {
      this.enNodeCert = enNodeCert;
    }

    public String getEnNodeKey() {
      return enNodeKey;
    }

    public void setEnNodeKey(String enNodeKey) {
      this.enNodeKey = enNodeKey;
    }

    @Override
    public String toString() {
      return "SMCertConfig{"
          + "caCert='"
          + caCert
          + '\''
          + ", nodeCert='"
          + nodeCert
          + '\''
          + ", nodeKey='"
          + nodeKey
          + '\''
          + ", enNodeCert='"
          + enNodeCert
          + '\''
          + ", enNodeKey='"
          + enNodeKey
          + '\''
          + '}';
    }
  };

  public int getThreadPoolSize() {
    return threadPoolSize;
  }

  public void setThreadPoolSize(int threadPoolSize) {
    this.threadPoolSize = threadPoolSize;
  }

  public int getReconnectPeriodMs() {
    return reconnectPeriodMs;
  }

  public void setReconnectPeriodMs(int reconnectPeriodMs) {
    this.reconnectPeriodMs = reconnectPeriodMs;
  }

  public int getHeartbeatPeriodMs() {
    return heartbeatPeriodMs;
  }

  public void setHeartbeatPeriodMs(int heartbeatPeriodMs) {
    this.heartbeatPeriodMs = heartbeatPeriodMs;
  }

  public int getMessageTimeoutMs() {
    return messageTimeoutMs;
  }

  public void setMessageTimeoutMs(int messageTimeoutMs) {
    this.messageTimeoutMs = messageTimeoutMs;
  }

  public boolean isDisableSsl() {
    return disableSsl;
  }

  public void setDisableSsl(boolean disableSsl) {
    this.disableSsl = disableSsl;
  }

  public String getSslType() {
    return sslType;
  }

  public void setSslType(String sslType) {
    this.sslType = sslType;
  }

  public CertConfig getCertConfig() {
    return certConfig;
  }

  public void setCertConfig(CertConfig certConfig) {
    this.certConfig = certConfig;
  }

  public SMCertConfig getSmCertConfig() {
    return smCertConfig;
  }

  public void setSmCertConfig(SMCertConfig smCertConfig) {
    this.smCertConfig = smCertConfig;
  }

  public List<String> getPeers() {
    return peers;
  }

  public void setPeers(List<String> peers) {
    this.peers = peers;
  }

  @Override
  public String toString() {
    return "JniConfig{"
        + "threadPoolSize="
        + threadPoolSize
        + ", reconnectPeriodMs="
        + reconnectPeriodMs
        + ", heartbeatPeriodMs="
        + heartbeatPeriodMs
        + ", messageTimeoutMs="
        + messageTimeoutMs
        + ", disableSsl="
        + disableSsl
        + ", sslType='"
        + sslType
        + '\''
        + ", peers='"
        + peers
        + '\''
        + '}';
  }
}
