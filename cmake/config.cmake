# hunter_config(
# OpenSSL VERSION "tassl_1.1.1-local"
# URL https://osp-1257653870.cos.ap-guangzhou.myqcloud.com/FISCO-BCOS/deps/tassl/tassl_1.1.1b_v1.4-fe885b93.tar.gz
# SHA1 3f207bbd209bec67a5aca27c0ffafe9c57ad1884
# )

hunter_config(
    Boost VERSION "1.76.0-local"
    URL
    "https://osp-1257653870.cos.ap-guangzhou.myqcloud.com/FISCO-BCOS/FISCO-BCOS/deps/boost_1_76_0.tar.bz2
    https://downloads.sourceforge.net/project/boost/boost/1.76.0/source/boost_1_76_0.tar.bz2
    https://nchc.dl.sourceforge.net/project/boost/boost/1.76.0/boost_1_76_0.tar.bz2"
    SHA1
    8064156508312dde1d834fec3dca9b11006555b6
    CMAKE_ARGS
    CONFIG_MACRO=BOOST_UUID_RANDOM_PROVIDER_FORCE_POSIX
)

hunter_config(
    jsoncpp VERSION "1.8.0-local"
    URL
    "https://osp-1257653870.cos.ap-guangzhou.myqcloud.com/FISCO-BCOS/deps/jsoncpp/1.8.0.tar.gz
    https://${URL_BASE}/open-source-parsers/jsoncpp/archive/1.8.0.tar.gz"
    SHA1
    40f7f34551012f68e822664a0b179e7e6cac5a97
)

hunter_config(bcos-utilities VERSION 1.0.0-rc3-local
    URL https://${URL_BASE}/FISCO-BCOS/bcos-utilities/archive/f6a20a05548da98bf99da68d1ee6413d5186156c.tar.gz
    SHA1 59c6cb92aee7ca6685a4c96528193519c373da0c
    CMAKE_ARGS HUNTER_PACKAGE_LOG_BUILD=ON HUNTER_PACKAGE_LOG_INSTALL=ON
)

hunter_config(wedpr-crypto VERSION 1.0.0
    URL https://${URL_BASE}/WeBankBlockchain/WeDPR-Lab-Crypto/archive/caeea48d7fa6c879fef52063cf107873744629b2.tar.gz
    SHA1 03e76f0759a0da0f55cad5d49af510657bb6f343
)

hunter_config(bcos-crypto VERSION 1.0.0-rc3-local
    URL https://${URL_BASE}/FISCO-BCOS/bcos-crypto/archive/92e09c9edac2bc6be8099ac34dce7233755eb8ee.tar.gz
    SHA1 b8f1e66cf4ed7ef7ce2755bf5711bf33ec7b344c
    CMAKE_ARGS HUNTER_PACKAGE_LOG_BUILD=ON HUNTER_PACKAGE_LOG_INSTALL=ON
)

hunter_config(bcos-boostssl
    VERSION 1.0.0-local
    URL https://${URL_BASE}/FISCO-BCOS/bcos-boostssl/archive/e8e912b15dc7ea769427a761362f285e9a6611e7.tar.gz
    SHA1 91638e350b922d10a8d573b5b9a214ac447dfbb1
    CMAKE_ARGS HUNTER_PACKAGE_LOG_BUILD=ON HUNTER_PACKAGE_LOG_INSTALL=ON ARCH_NATIVE=${ARCH_NATIVE} URL_BASE=${URL_BASE}
)

hunter_config(bcos-cpp-sdk
    VERSION 3.2.0-local
    URL https://${URL_BASE}/FISCO-BCOS/bcos-cpp-sdk/archive/54d2516460a8171eaf21c06de06c7a17982902c7.tar.gz
    SHA1 d79ade46a3b118b0efff0d43eb1783ba439b845c
)

hunter_config(range-v3 VERSION 1.0.0
    URL https://${URL_BASE}/ericniebler/range-v3/archive/a81477931a8aa2ad025c6bda0609f38e09e4d7ec.tar.gz
    SHA1 2d84b639bfd42bb1cbe2fd88f85ff0d1cca25952
)
