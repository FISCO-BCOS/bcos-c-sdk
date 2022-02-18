#hunter_config(
#    OpenSSL VERSION "tassl_1.1.1-local"
#    URL https://osp-1257653870.cos.ap-guangzhou.myqcloud.com/FISCO-BCOS/deps/tassl/tassl_1.1.1b_v1.4-fe885b93.tar.gz
#    SHA1 3f207bbd209bec67a5aca27c0ffafe9c57ad1884
#)

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

hunter_config(bcos-utilities VERSION 3.0.0-rc2-local
    URL https://${URL_BASE}/FISCO-BCOS/bcos-utilities/archive/1a1c28d2b87dc6dd9c02d9737d9cb3c434abc664.tar.gz
	SHA1 b8cd65db849be95bf37b3ab3fba0953fd0f59675
    CMAKE_ARGS HUNTER_PACKAGE_LOG_BUILD=ON HUNTER_PACKAGE_LOG_INSTALL=ON
)

hunter_config(bcos-crypto VERSION 3.0.0-local
    URL https://${URL_BASE}/FISCO-BCOS/bcos-crypto/archive/52cf6db2c082ec723f5d150abaae312c8dd293d0.tar.gz
	SHA1 531f31424c3377a9bc8b68a739cfa9ce95f5e8e0
    CMAKE_ARGS HUNTER_PACKAGE_LOG_BUILD=ON HUNTER_PACKAGE_LOG_INSTALL=ON SM2_OPTIMIZE=ON
)

hunter_config(bcos-boostssl
	VERSION 3.0.0-local
	URL https://${URL_BASE}/FISCO-BCOS/bcos-boostssl/archive/f133fbf006b6dfa4205a0bb9fda2ea0b67c5420b.tar.gz
	SHA1 536724943cf0d8bf317bf396cc0e3a1bd7f05423
)

hunter_config(bcos-cpp-sdk
	VERSION 3.0.0-local
	URL https://${URL_BASE}/FISCO-BCOS/bcos-cpp-sdk/archive/d02ccfa5606d683bb9757e7e1890223eebae4280.tar.gz
	SHA1 7ab01453b0f00debe44b0aaecd957ff9331fbf16
)