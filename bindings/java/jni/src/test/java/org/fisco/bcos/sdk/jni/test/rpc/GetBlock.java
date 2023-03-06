package org.fisco.bcos.sdk.jni.test.rpc;

import java.util.Arrays;
import org.fisco.bcos.sdk.jni.BcosSDKJniObj;
import org.fisco.bcos.sdk.jni.common.JniConfig;
import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.Response;
import org.fisco.bcos.sdk.jni.rpc.RpcCallback;
import org.fisco.bcos.sdk.jni.rpc.RpcJniObj;
import org.fisco.bcos.sdk.jni.test.Utility;

public class GetBlock {

  public static void usage() {
    System.out.println("\tUsage: ");
    System.out.println(
        "\t\t\"java -cp \"conf/:lib/*:apps/*\"  org.fisco.bcos.sdk.jni.test.amop.GetBlock ip:port group");
    System.out.println("\tExample:");
    System.out.println(
        "\t\t\"java -cp \"conf/:lib/*:apps/*\"  org.fisco.bcos.sdk.jni.test.amop.GetBlock 127.0.0.1:20201 group0");
    System.exit(0);
  }

  public static void main(String[] args) throws JniException {
    if (args.length < 2) {
      System.out.println("args length = " + args.length);
      usage();
    }

    String endpoint = args[0];
    String group = args[1];
    String node = "";
    JniConfig jniConfig = Utility.newJniConfig(Arrays.asList(endpoint));
    jniConfig.setDisableSsl(true);
    BcosSDKJniObj bcosSDKJni = BcosSDKJniObj.build(jniConfig);
    RpcJniObj rpcJniObj = RpcJniObj.build(bcosSDKJni.getNativePointer());
    System.out.println("build Rpc");
    rpcJniObj.start();

    while (true) {
      rpcJniObj.getBlockNumber(
          group,
          node,
          new RpcCallback() {
            @Override
            public void onResponse(Response response) {
              System.out.println("getBlockNumber ==>>>" + response);
            }
          });

      rpcJniObj.getGroupInfo(
          group,
          new RpcCallback() {
            @Override
            public void onResponse(Response response) {
              System.out.println("getBlockNumber ==>>>" + response);
            }
          });

      try {
        Thread.sleep(1000);
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
    }
  }
}
