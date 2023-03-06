package org.fisco.bcos.sdk.jni.test.amop;

import java.util.Arrays;
import org.fisco.bcos.sdk.jni.BcosSDKJniObj;
import org.fisco.bcos.sdk.jni.amop.AmopJniObj;
import org.fisco.bcos.sdk.jni.common.JniConfig;
import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.test.Utility;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Sub {

  private static final Logger logger = LoggerFactory.getLogger(Sub.class);

  public static void usage() {
    System.out.println("\tUsage: ");
    System.out.println(
        "\t\tjava -cp \"conf/:lib/*:apps/*\"  org.fisco.bcos.sdk.jni.test.amop.Sub ip:port topic");
    System.out.println("\tExample:");
    System.out.println(
        "\t\tjava -cp \"conf/:lib/*:apps/*\"  org.fisco.bcos.sdk.jni.test.amop.Sub 127.0.0.1:20201 topic");
    System.exit(0);
  }

  public static void main(String[] args) throws InterruptedException, JniException {
    if (args.length < 2) {
      usage();
    }

    String peer = args[0];
    String topic = args[1];

    JniConfig jniConfig = Utility.newJniConfig(Arrays.asList(peer));
    jniConfig.setDisableSsl(true);
    BcosSDKJniObj bcosSDKJni = BcosSDKJniObj.build(jniConfig);
    System.out.println("BcosSDK build");
    AmopJniObj amop = AmopJniObj.build(bcosSDKJni.getNativePointer());
    amop.start();

    amop.subscribeTopic(
        topic,
        (endpoint, seq, data) -> {
          System.out.println(" ==> receive message from client");
          System.out.println(" \t==> endpoint: " + endpoint);
          System.out.println(" \t==> seq: " + seq);
          System.out.println(" \t==> data: " + new String(data));

          amop.sendResponse(endpoint, seq, data);
        });

    while (true) {

      Thread.sleep(10000);
    }
  }
}
