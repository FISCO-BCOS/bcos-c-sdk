package org.fisco.bcos.sdk.jni.test.tx;

import java.security.SecureRandom;
import java.util.Arrays;
import org.fisco.bcos.sdk.jni.BcosSDKJniObj;
import org.fisco.bcos.sdk.jni.common.JniConfig;
import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.Response;
import org.fisco.bcos.sdk.jni.rpc.RpcCallback;
import org.fisco.bcos.sdk.jni.rpc.RpcJniObj;
import org.fisco.bcos.sdk.jni.test.Utility;
import org.fisco.bcos.sdk.jni.utilities.keypair.KeyPairJniObj;
import org.fisco.bcos.sdk.jni.utilities.tx.*;
import org.junit.Assert;

public class TestTxStructV2 {

  // ------------------------------------------------------------------------------
  // ------------------------------------------------------------------------------

  // HelloWorld Source Code:
  /**
   * pragma solidity>=0.4.24 <0.6.11;
   *
   * <p>contract HelloWorld { string name;
   *
   * <p>constructor() public { name = "Hello, World!"; }
   *
   * <p>function get() public view returns (string memory) { return name; }
   *
   * <p>function set(string memory n) public { name = n; } }
   */
  private static final String hwBIN =
      "608060405234801561001057600080fd5b506040518060400160405280600d81526020017f48656c6c6f2c20576f726c6421000000000000000000000000000000000000008152506000908051906020019061005c929190610062565b50610107565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f106100a357805160ff19168380011785556100d1565b828001600101855582156100d1579182015b828111156100d05782518255916020019190600101906100b5565b5b5090506100de91906100e2565b5090565b61010491905b808211156101005760008160009055506001016100e8565b5090565b90565b610310806101166000396000f3fe608060405234801561001057600080fd5b50600436106100365760003560e01c80634ed3885e1461003b5780636d4ce63c146100f6575b600080fd5b6100f46004803603602081101561005157600080fd5b810190808035906020019064010000000081111561006e57600080fd5b82018360208201111561008057600080fd5b803590602001918460018302840111640100000000831117156100a257600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600081840152601f19601f820116905080830192505050505050509192919290505050610179565b005b6100fe610193565b6040518080602001828103825283818151815260200191508051906020019080838360005b8381101561013e578082015181840152602081019050610123565b50505050905090810190601f16801561016b5780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b806000908051906020019061018f929190610235565b5050565b606060008054600181600116156101000203166002900480601f01602080910402602001604051908101604052809291908181526020018280546001816001161561010002031660029004801561022b5780601f106102005761010080835404028352916020019161022b565b820191906000526020600020905b81548152906001019060200180831161020e57829003601f168201915b5050505050905090565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f1061027657805160ff19168380011785556102a4565b828001600101855582156102a4579182015b828111156102a3578251825591602001919060010190610288565b5b5090506102b191906102b5565b5090565b6102d791905b808211156102d35760008160009055506001016102bb565b5090565b9056fea2646970667358221220b5943f43c48cc93c6d71cdcf27aee5072566c88755ce9186e32ce83b24e8dc6c64736f6c634300060a0033";

  private static final String hwSmBIN =
      "608060405234801561001057600080fd5b506040518060400160405280600d81526020017f48656c6c6f2c20576f726c6421000000000000000000000000000000000000008152506000908051906020019061005c929190610062565b50610107565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f106100a357805160ff19168380011785556100d1565b828001600101855582156100d1579182015b828111156100d05782518255916020019190600101906100b5565b5b5090506100de91906100e2565b5090565b61010491905b808211156101005760008160009055506001016100e8565b5090565b90565b610310806101166000396000f3fe608060405234801561001057600080fd5b50600436106100365760003560e01c8063299f7f9d1461003b5780633590b49f146100be575b600080fd5b610043610179565b6040518080602001828103825283818151815260200191508051906020019080838360005b83811015610083578082015181840152602081019050610068565b50505050905090810190601f1680156100b05780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b610177600480360360208110156100d457600080fd5b81019080803590602001906401000000008111156100f157600080fd5b82018360208201111561010357600080fd5b8035906020019184600183028401116401000000008311171561012557600080fd5b91908080601f016020809104026020016040519081016040528093929190818152602001838380828437600081840152601f19601f82011690508083019250505050505050919291929050505061021b565b005b606060008054600181600116156101000203166002900480601f0160208091040260200160405190810160405280929190818152602001828054600181600116156101000203166002900480156102115780601f106101e657610100808354040283529160200191610211565b820191906000526020600020905b8154815290600101906020018083116101f457829003601f168201915b5050505050905090565b8060009080519060200190610231929190610235565b5050565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f1061027657805160ff19168380011785556102a4565b828001600101855582156102a4579182015b828111156102a3578251825591602001919060010190610288565b5b5090506102b191906102b5565b5090565b6102d791905b808211156102d35760008160009055506001016102bb565b5090565b9056fea26469706673582212209871cb2bcf390d53645807cbaedfe052d739ef9cff9d84787f74c4f379e1854664736f6c634300060a0033";

  /*
  {
      "6d4ce63c": "get()",
      "4ed3885e": "set(string)"
  }

  {
      "299f7f9d": "get()",
      "3590b49f": "set(string)"
  }
  */

  public static byte[] fromHex(String hexString) {
    if (hexString.startsWith("0x")) {
      hexString = hexString.substring(2);
    }

    if (hexString.length() % 2 != 0) {
      throw new IllegalArgumentException("Invalid hex string: " + hexString);
    }

    int length = hexString.length() / 2;
    byte[] bytes = new byte[length];

    for (int i = 0; i < length; i++) {
      String twoChars = hexString.substring(i * 2, i * 2 + 2);
      bytes[i] = (byte) Integer.parseInt(twoChars, 16);
    }

    return bytes;
  }

  public static String generateNonce() {
    byte[] nonceBytes = new byte[16];
    SecureRandom secureRandom = new SecureRandom();
    secureRandom.nextBytes(nonceBytes);
    StringBuilder hex = new StringBuilder();
    for (byte b : nonceBytes) {
      hex.append(String.format("%02x", b));
    }
    return hex.toString();
  }

  public static String getBinary(boolean isSM) {
    return isSM ? hwSmBIN : hwBIN;
  }

  public static void Usage() {
    System.out.println("Desc: test transaction struct V2 [HelloWorld set]");
    System.out.println(
        "Usage: java -cp 'conf/:lib/*:apps/*' org.fisco.bcos.sdk.jni.test.tx.TestTxStructV2");
    System.exit(0);
  }

  public static void main(String[] args) throws JniException, InterruptedException {
    try {
      if (args.length > 1) {
        Usage();
      }

      String endpoint = "127.0.0.1:20200";
      String group = "group0";
      String node = "";
      JniConfig jniConfig = Utility.newJniConfig(Arrays.asList(endpoint));
      jniConfig.setDisableSsl(true);
      BcosSDKJniObj bcosSDKJni = BcosSDKJniObj.build(jniConfig);
      RpcJniObj rpcJniObj = RpcJniObj.build(bcosSDKJni.getNativePointer());
      System.out.println("build Rpc");
      rpcJniObj.start();

      boolean smCrypto = false;

      long keyPair = KeyPairJniObj.createJniKeyPair(smCrypto ? 1 : 0);
      String jniKeyPairAddress = KeyPairJniObj.getJniKeyPairAddress(keyPair);
      System.out.printf(" [test Tx Struct V2] new account, address: %s\n", jniKeyPairAddress);

      String groupID = "group0";
      String chainID = "chain0";
      String data = getBinary(smCrypto);
      long blockLimit = 1;
      String value = "11";
      String gasPrice = "0";
      long gasLimit = 0;
      String maxFeePerGas = "11";
      String maxPriorityFeePerGas = "22";

      // construct TransactionDataV2
      TransactionDataV2 transactionDataStructV2 = new TransactionDataV2();
      transactionDataStructV2.setGroupId(groupID);
      transactionDataStructV2.setChainId(chainID);
      transactionDataStructV2.setTo("");
      transactionDataStructV2.setAbi("");
      transactionDataStructV2.setVersion(1);
      transactionDataStructV2.setNonce(generateNonce());
      transactionDataStructV2.setBlockLimit(blockLimit);
      transactionDataStructV2.setValue(value);
      transactionDataStructV2.setGasPrice(gasPrice);
      transactionDataStructV2.setGasLimit(gasLimit);
      transactionDataStructV2.setMaxFeePerGas(maxFeePerGas);
      transactionDataStructV2.setMaxPriorityFeePerGas(maxPriorityFeePerGas);
      // input
      TransactionBytes inputBytes = new TransactionBytes();
      byte[] bytesInput = fromHex(data);
      inputBytes.setBuffer(bytesInput);
      inputBytes.setLength(bytesInput.length);
      transactionDataStructV2.setInput(inputBytes);

      // encode TxData to hex tx data
      String txDataHex =
          TransactionStructBuilderV2JniObj.encodeTransactionDataStructV2(transactionDataStructV2);
      // decode hex tx data to TxData
      TransactionDataV2 decodeTransactionDataStructHex =
          TransactionStructBuilderV2JniObj.decodeTransactionDataStructV2(txDataHex);
      //            TransactionDataV2 decodeTransactionDataStructHex2 =
      // TransactionStructBuilderV2JniObj.decodeTransactionDataStructV2(null);
      //            TransactionDataV2 decodeTransactionDataStructHex3 =
      // TransactionStructBuilderV2JniObj.decodeTransactionDataStructV2("");

      // assert
      Assert.assertEquals(
          transactionDataStructV2.getChainId(), decodeTransactionDataStructHex.getChainId());
      Assert.assertEquals(
          transactionDataStructV2.getGroupId(), decodeTransactionDataStructHex.getGroupId());
      Assert.assertEquals(
          transactionDataStructV2.getAbi(), decodeTransactionDataStructHex.getAbi());
      Assert.assertEquals(
          transactionDataStructV2.getBlockLimit(), decodeTransactionDataStructHex.getBlockLimit());
      Assert.assertEquals(
          transactionDataStructV2.getValue(), decodeTransactionDataStructHex.getValue());
      Assert.assertEquals(
          transactionDataStructV2.getGasPrice(), decodeTransactionDataStructHex.getGasPrice());
      Assert.assertEquals(
          transactionDataStructV2.getGasLimit(), decodeTransactionDataStructHex.getGasLimit());
      Assert.assertEquals(
          transactionDataStructV2.getMaxFeePerGas(),
          decodeTransactionDataStructHex.getMaxFeePerGas());
      Assert.assertEquals(
          transactionDataStructV2.getMaxPriorityFeePerGas(),
          decodeTransactionDataStructHex.getMaxPriorityFeePerGas());

      // encode TxData to json tx data
      String txDataJson =
          TransactionStructBuilderV2JniObj.encodeTransactionDataStructToJsonV2(
              transactionDataStructV2);
      System.out.printf(" [test Tx Struct V2] txDataJson: %s\n", txDataJson);

      // calc tx data hash
      String txDataHash =
          TransactionStructBuilderV2JniObj.calcTransactionDataStructHashV2(
              smCrypto ? 1 : 0, decodeTransactionDataStructHex);
      System.out.printf(" [test Tx Struct V2] txDataHash: %s\n", txDataHash);
      // signature tx data hash
      String signature = TransactionBuilderJniObj.signTransactionDataHash(keyPair, txDataHash);
      System.out.printf(" [test Tx Struct V2] signature: %s\n", signature);

      // construct tx
      TransactionV2 transactionStructV2 = new TransactionV2();
      TransactionBytes dataHashBytes = new TransactionBytes();
      dataHashBytes.setBuffer(txDataHash.getBytes());
      dataHashBytes.setLength(txDataHash.getBytes().length);
      TransactionBytes signatureBytes = new TransactionBytes();
      signatureBytes.setBuffer(signature.getBytes());
      signatureBytes.setLength(signature.getBytes().length);
      transactionStructV2.setTransactionData(decodeTransactionDataStructHex);
      transactionStructV2.setDataHash(dataHashBytes);
      transactionStructV2.setSignature(signatureBytes);
      transactionStructV2.setSender(null);
      transactionStructV2.setImportTime(0);
      transactionStructV2.setAttribute(0);
      transactionStructV2.setExtraData("");
      // assert
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getBlockLimit(),
          decodeTransactionDataStructHex.getBlockLimit());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getGroupId(),
          decodeTransactionDataStructHex.getGroupId());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getChainId(),
          decodeTransactionDataStructHex.getChainId());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getChainId(),
          decodeTransactionDataStructHex.getChainId());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getAbi(),
          decodeTransactionDataStructHex.getAbi());
      Assert.assertArrayEquals(
          transactionStructV2.getDataHash().getBuffer(), txDataHash.getBytes());
      Assert.assertArrayEquals(
          transactionStructV2.getSignature().getBuffer(), signature.getBytes());

      // encode Tx to hex tx
      String txHex =
          TransactionStructBuilderV2JniObj.encodeTransactionStructV2(transactionStructV2);
      // decode hex tx to Tx
      TransactionV2 decodeTransactionStructHex =
          TransactionStructBuilderV2JniObj.decodeTransactionStructV2(txHex);

      // assert
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getBlockLimit(),
          decodeTransactionStructHex.getTransactionData().getBlockLimit());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getGroupId(),
          decodeTransactionStructHex.getTransactionData().getGroupId());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getChainId(),
          decodeTransactionStructHex.getTransactionData().getChainId());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getAbi(),
          decodeTransactionStructHex.getTransactionData().getAbi());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getValue(),
          decodeTransactionStructHex.getTransactionData().getValue());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getGasLimit(),
          decodeTransactionStructHex.getTransactionData().getGasLimit());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getGasPrice(),
          decodeTransactionStructHex.getTransactionData().getGasPrice());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getMaxFeePerGas(),
          decodeTransactionStructHex.getTransactionData().getMaxFeePerGas());
      Assert.assertEquals(
          transactionStructV2.getTransactionData().getMaxPriorityFeePerGas(),
          decodeTransactionStructHex.getTransactionData().getMaxPriorityFeePerGas());
      Assert.assertArrayEquals(
          transactionStructV2.getDataHash().getBuffer(),
          decodeTransactionStructHex.getDataHash().getBuffer());
      Assert.assertArrayEquals(
          transactionStructV2.getSignature().getBuffer(),
          decodeTransactionStructHex.getSignature().getBuffer());

      // encode Tx to json tx
      String txJson =
          TransactionStructBuilderV2JniObj.encodeTransactionStructToJsonV2(transactionStructV2);
      System.out.printf(" [test Tx Struct V2] txJson: %s\n", txJson);
      // create tx string
      String txString =
          TransactionStructBuilderV2JniObj.createEncodedTransactionV2(
              decodeTransactionDataStructHex, signature, txDataHash, 0, "");
      //        System.out.printf(" [test Tx Struct V2] txString: %s\n", txString);

      // rpc send tx
      rpcJniObj.sendTransaction(
          group,
          node,
          txString,
          false,
          new RpcCallback() {
            @Override
            public void onResponse(Response response) {
              System.out.println("response error code: ==>>> " + response.getErrorCode());
              String dataStr = new String(response.getData());
              System.out.println("response data: ==>>> " + dataStr);
            }
          });

      Thread.sleep(2000);
      rpcJniObj.stop();
      System.out.printf(" [test Tx Struct V2] finish !! \n");
    } catch (JniException e) {
      System.out.println("test tx struct V2 failed, error: " + e.getMessage());
    }
  }
}
