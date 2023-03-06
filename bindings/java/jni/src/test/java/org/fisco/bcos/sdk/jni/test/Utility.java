package org.fisco.bcos.sdk.jni.test;

import java.util.List;
import java.util.Objects;
import org.fisco.bcos.sdk.jni.common.JniConfig;
import org.fisco.bcos.sdk.jni.common.JniException;
import org.fisco.bcos.sdk.jni.common.JniLibLoader;
import org.fisco.bcos.sdk.jni.utilities.receipt.ReceiptBuilderJniObj;
import org.fisco.bcos.sdk.jni.utilities.tx.TransactionBuilderJniObj;

public class Utility {
  public static JniConfig newJniConfig(List<String> peers) {
    JniConfig jniConfig = new JniConfig();
    jniConfig.setPeers(peers);
    jniConfig.setCertConfig(new JniConfig.CertConfig());
    jniConfig.setSmCertConfig(new JniConfig.SMCertConfig());

    return jniConfig;
  }

  public static void main(String[] args) throws JniException {
    JniLibLoader.loadJniLibrary();

    String rawJson =
        "{\"abi\":\"\",\"blockLimit\":501,\"chainID\":\"chain0\",\"from\":\"0x3d20a4e26f41b57c2061e520c825fbfa5f321f22\",\"groupID\":\"group0\",\"hash\":\"0x0359a5588c5e9c9dcfd2f4ece850d6f4c41bc88e2c27cc051890f26ef0ef118f\",\"importTime\":1670467885565,\"input\":\"0x2fe99bdc000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000e0000000000000000000000000000000000000000000000000000000000000000574657374310000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005746573743200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000057465737433000000000000000000000000000000000000000000000000000000\",\"nonce\":\"82456994196425209985513938878262806289094428076435270713862099506789849517246\",\"signature\":\"0xd752fbe3218d6f0d4bdcbaf45875bba4a52f9af263badabc0ef93fa31f28d79b0779728b594c1212613a38daea8bdd36670777100b159ac537dfbd83e700d20901\",\"to\":\"0x6849f21d1e455e9f0712b1e99fa4fcd23758e8f1\",\"version\":0}\n";
    long transactionDataWithJson = TransactionBuilderJniObj.createTransactionDataWithJson(rawJson);
    String hash = TransactionBuilderJniObj.calcTransactionDataHash(0, transactionDataWithJson);
    if (!Objects.equals(
        hash, "0x0359a5588c5e9c9dcfd2f4ece850d6f4c41bc88e2c27cc051890f26ef0ef118f")) {
      throw new JniException("errorHash");
    }

    String receiptJson =
        "{\"blockNumber\":2,\"checksumContractAddress\":\"\",\"contractAddress\":\"\",\"from\":\"0x3d20a4e26f41b57c2061e520c825fbfa5f321f22\",\"gasUsed\":\"19413\",\"hash\":\"0xb59cfe6ef607b72a6bab515042e0882213d179bd421afba353e2259b2a6396e4\",\"input\":\"0x2fe99bdc000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000e0000000000000000000000000000000000000000000000000000000000000000574657374310000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005746573743200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000057465737433000000000000000000000000000000000000000000000000000000\",\"logEntries\":[{\"address\":\"6849f21d1e455e9f0712b1e99fa4fcd23758e8f1\",\"data\":\"0x0000000000000000000000000000000000000000000000000000000000000001\",\"topics\":[\"0xc57b01fa77f41df77eaab79a0e2623fab2e7ae3e9530d9b1cab225ad65f2b7ce\"]}],\"message\":\"\",\"output\":\"0x0000000000000000000000000000000000000000000000000000000000000001\",\"status\":0,\"to\":\"0x6849f21d1e455e9f0712b1e99fa4fcd23758e8f1\",\"transactionHash\":\"0x0359a5588c5e9c9dcfd2f4ece850d6f4c41bc88e2c27cc051890f26ef0ef118f\",\"transactionProof\":null,\"version\":0}\n";
    long receiptDataWithJson = ReceiptBuilderJniObj.createReceiptDataWithJson(receiptJson);
    String hash1 = ReceiptBuilderJniObj.calcReceiptDataHash(0, receiptDataWithJson);
    if (!Objects.equals(
        hash1, "0xb59cfe6ef607b72a6bab515042e0882213d179bd421afba353e2259b2a6396e4")) {
      throw new JniException("errorHash1");
    }

    String encodeTransactionData =
        TransactionBuilderJniObj.encodeTransactionData(transactionDataWithJson);
    String jsonObj = TransactionBuilderJniObj.decodeTransactionDataToJsonObj(encodeTransactionData);
    long transactionDataWithJson1 = TransactionBuilderJniObj.createTransactionDataWithJson(jsonObj);
    String hash2 = TransactionBuilderJniObj.calcTransactionDataHash(0, transactionDataWithJson1);
    if (!Objects.equals(
        hash2, "0x0359a5588c5e9c9dcfd2f4ece850d6f4c41bc88e2c27cc051890f26ef0ef118f")) {
      throw new JniException("errorHash2");
    }

    String encodeReceiptData = ReceiptBuilderJniObj.encodeReceiptData(receiptDataWithJson);
    String receiptDataToJsonObj =
        ReceiptBuilderJniObj.decodeReceiptDataToJsonObj(encodeReceiptData);
    long receiptDataWithJson1 =
        ReceiptBuilderJniObj.createReceiptDataWithJson(receiptDataToJsonObj);
    String hash3 = ReceiptBuilderJniObj.calcReceiptDataHash(0, receiptDataWithJson1);
    if (!Objects.equals(
        hash3, "0xb59cfe6ef607b72a6bab515042e0882213d179bd421afba353e2259b2a6396e4")) {
      throw new JniException("errorHash3");
    }

    TransactionBuilderJniObj.destroyTransactionData(transactionDataWithJson);
    TransactionBuilderJniObj.destroyTransactionData(transactionDataWithJson1);
    ReceiptBuilderJniObj.destroyReceiptData(receiptDataWithJson);
    ReceiptBuilderJniObj.destroyReceiptData(receiptDataWithJson1);
    System.out.println("All good!");
  }
}
