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
    URL https://${URL_BASE}/FISCO-BCOS/bcos-utilities/archive/b94375d933cc65c348b5c219f2ed821a5f66d9b1.tar.gz
	SHA1 2ae9c579e924ccb67f07c9626d4d748bc60c9cef
    CMAKE_ARGS HUNTER_PACKAGE_LOG_BUILD=ON HUNTER_PACKAGE_LOG_INSTALL=ON
)

hunter_config(bcos-boostssl
	VERSION 3.0.0-local
	URL https://${URL_BASE}/FISCO-BCOS/bcos-boostssl/archive/ef6705e59ea07f1dac8acc62e31cd49731df6a47.tar.gz
	SHA1 8b7da727c4410beaa5ce9310bdcdd08d91687132
)

hunter_config(bcos-cpp-sdk
	VERSION 3.0.0-local
	URL https://${URL_BASE}/FISCO-BCOS/bcos-cpp-sdk/archive/d00ae5d76b7d9b408112093b8c1ace1d211996fe.tar.gz
	SHA1 1a5ea9916c0c3071364032a24f5d1ab837ed9d93
)