package org.fisco.bcos.sdk.jni.test.amop;

import java.util.Arrays;
import org.fisco.bcos.sdk.jni.BcosSDKJniObj;
import org.fisco.bcos.sdk.jni.amop.AmopJniObj;
import org.fisco.bcos.sdk.jni.common.JniConfig;
import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.test.Utility;

public class Broadcast {
  public static void usage() {
    System.out.println("\tUsage: ");
    System.out.println(
        "\t\tjava -cp \"conf/:lib/*:apps/*\"  org.fisco.bcos.sdk.jni.test.amop.Broadcast ip:port topic msg");
    System.out.println("\tExample:");
    System.out.println(
        "\t\tjava -cp \"conf/:lib/*:apps/*\"  org.fisco.bcos.sdk.jni.test.amop.Broadcast 127.0.0.1:20201 topic HelloWorld");
    System.exit(0);
  }

  public static void main(String[] args) throws InterruptedException, JniException {

    if (args.length < 3) {
      usage();
    }

    String peer = args[0];
    String topic = args[1];
    String msg = args[2];

    JniConfig jniConfig = Utility.newJniConfig(Arrays.asList(peer));
    jniConfig.setDisableSsl(true);

    BcosSDKJniObj bcosSDKJni = BcosSDKJniObj.build(jniConfig);
    AmopJniObj amop = AmopJniObj.build(bcosSDKJni.getNativePointer());
    amop.start();

    while (true) {
      amop.broadcastAmopMsg(topic, msg.getBytes());
      Thread.sleep(10000);
    }
  }
}
