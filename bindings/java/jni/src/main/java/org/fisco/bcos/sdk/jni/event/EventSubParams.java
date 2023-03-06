package org.fisco.bcos.sdk.jni.event;

import java.util.ArrayList;
import java.util.List;

public class EventSubParams {
  private long fromBlock;
  private long toBlock;
  private List<String> addresses = new ArrayList<>();
  private List<List<String>> topics = new ArrayList<>();

  public long getFromBlock() {
    return fromBlock;
  }

  public void setFromBlock(long fromBlock) {
    this.fromBlock = fromBlock;
  }

  public long getToBlock() {
    return toBlock;
  }

  public void setToBlock(long toBlock) {
    this.toBlock = toBlock;
  }

  public List<String> getAddresses() {
    return addresses;
  }

  public void setAddresses(List<String> addresses) {
    this.addresses = addresses;
  }

  public List<List<String>> getTopics() {
    return topics;
  }

  public void setTopics(List<List<String>> topics) {
    this.topics = topics;
  }
}
