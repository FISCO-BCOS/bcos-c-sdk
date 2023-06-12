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
