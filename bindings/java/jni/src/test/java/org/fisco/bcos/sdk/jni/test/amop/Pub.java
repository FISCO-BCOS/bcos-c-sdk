package org.fisco.bcos.sdk.jni.test.amop;

import java.util.Arrays;
import org.fisco.bcos.sdk.jni.BcosSDKJniObj;
import org.fisco.bcos.sdk.jni.amop.AmopJniObj;
import org.fisco.bcos.sdk.jni.common.JniConfig;
import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.test.Utility;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Pub {
  private static final Logger logger = LoggerFactory.getLogger(Sub.class);

  public static void usage() {
    System.out.println("\tUsage: ");
    System.out.println(
        "\t\tjava -cp \"conf/:lib/*:apps/*\"  org.fisco.bcos.sdk.jni.test.amop.Pub ip:port topic msg");
    System.out.println("\tExample:");
    System.out.println(
        "\t\tjava -cp \"conf/:lib/*:apps/*\"  org.fisco.bcos.sdk.jni.test.amop.Pub 127.0.0.1:20201 topic HelloWorld");
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
    System.out.println("BcosSDK build");
    AmopJniObj amop = AmopJniObj.build(bcosSDKJni.getNativePointer());
    amop.start();

    while (true) {
      amop.sendAmopMsg(
          topic,
          msg.getBytes(),
          0,
          response ->
              logger.info(
                  " ==> receive response message from server, errorCode: {}, errorMessage: {}, data: {}",
                  response.getErrorCode(),
                  response.getErrorMessage(),
                  new String(response.getData())));

      Thread.sleep(10000);
    }
  }
}
