### 3.6.0

(2024-02-08)

**新增**

- 新增 `v1` 版本的交易构造方法，并提供Jni层对应的实现方法。详情见：https://github.com/FISCO-BCOS/bcos-c-sdk/pull/191 ，https://github.com/FISCO-BCOS/bcos-c-sdk/pull/192
- 新增Jni层的 `TransactionData` 以及 `Transaction` 两层的交易类，支持交易编码、计算哈希、签名、发送。详情见：https://github.com/FISCO-BCOS/bcos-c-sdk/pull/163
- 在c sdk层增加获取与节点握手后的协议版本号，

**更新**

- 重构依赖库，使用vcpkg作为包管理，不再使用hunter。依赖FISCO BCOS主仓库的Cpp SDK，不再依赖bcos-cpp-sdk的仓库。

**修复**

- 修复配置文件中hostname解析失败的问题。详情见：https://github.com/FISCO-BCOS/bcos-c-sdk/pull/212

---

### 3.5.0

(2023-10-12)

**修复**
- 修复rpc模块的jvmdetach线程方式导致性能下降问题

#### bcos-go-sdk
**修复**
- 修复go wrapper的NewSDK没有检查c-sdk返回的last error

---

### 3.4.0

(2023-06-12)

#### bcos-sdk-jni

**更新**
- 去掉日志中对证书信息的打印

**修复**
- 修复rpc模块的jvm没有detach线程，导致java-sdk析构失败的问题

### 3.3.0

(2023-04-18)

#### bcos-sdk-jni
**新增**
- 新增密码机支持：配置keyindex使用内置密钥、签名验签
- 添加 `sendTransaction` 接口，支持同时编码/签名/发送交易功能

**更新**
- 更新 `bcos-boostssl` 以支持域名
- 更新 `jackson-databind` 到 `2.14.2` 版本

#### bcos-c-sdk

**新增**
- 新增密码机支持：配置keyindex使用内置密钥、签名验签
- 添加 `sendTransaction` 接口，支持同时编码/签名/发送交易功能

**更新**
- 更新 `jackson-databind` 到 `2.14.2` 版本
- 合并 `bcos-sdk-jni`
- 添加发送RPC请求时是否根据块高发送的开关

### 3.2.0

(2023-01-03)

**更新**

- 添加交易回执编码解析接口

### 3.1.0

(2023-01-03)

**更新**

- 交易支持设置`extraData`字段

### 3.0.1

(2022-09-09)

**更新**

- 添加abi编解码接口
- 更新HelloWorld示例

### 3.0.0

(2022-08-23)

**更新**

- 更新`bcos-boostssl`、`bcos-cpp-sdk`，详细功能更新参考`bcos-boostssl`、`bcos-cpp-sdk` ChangeLog

### 3.0.0-rc4

(2022-06-30)

**更新**

- 更新`bcos-boostssl`、`bcos-cpp-sdk`

### 3.0.0-rc3

(2022-03-31)

**新增**

- 支持交易编码
- 支持交易签名

### 3.0.0-rc2

(2022-02-22)

**新增**

- 支持通过配置文件初始化`SDK`
- 添加区块高度通知注册接口
- 添加`windows ci`，支持通过`ci`上传动态库

**更新**

- 更新`bcos-boostssl`、`bcos-cpp-sdk`依赖

### 3.0.0-rc1

(2021-12-09)

**简介**

`FISCO-BCOS 3.0`的`C`版本的`SDK`实现，通过对`CPP-SDK`的封装实现

**功能**

- `RPC`
- `AMOP`
- 合约事件订阅
