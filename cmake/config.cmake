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

hunter_config(bcos-utilities VERSION 1.0.0-rc2-local
    URL https://${URL_BASE}/FISCO-BCOS/bcos-utilities/archive/55d6bf357719aa030e3898b060b4a3ff2476434b.tar.gz
	SHA1 d0aeb11cf064a5e9793c50dd7bb8487e2d251f02
    CMAKE_ARGS HUNTER_PACKAGE_LOG_BUILD=ON HUNTER_PACKAGE_LOG_INSTALL=ON
)

hunter_config(bcos-crypto VERSION 1.0.0-rc2-local
    URL https://${URL_BASE}/FISCO-BCOS/bcos-crypto/archive/86fdc960741bd26cea3c9f04e61f138f508c7add.tar.gz
    SHA1 53dcac50016f5e1bd517ae8d595b586e358ad118
    CMAKE_ARGS HUNTER_PACKAGE_LOG_BUILD=ON HUNTER_PACKAGE_LOG_INSTALL=ON SM2_OPTIMIZE=ON
)

hunter_config(bcos-boostssl VERSION 1.0.0-rc3-local
	URL https://github.com/LucasLi1024/bcos-boostssl/archive/7fbf9e3b182a0d7140628ebf145566779466e262.tar.gz
	SHA1 9e321a215028ae665ad2302a0a7a29532f38a6e1
)

hunter_config(bcos-cpp-sdk
	VERSION 3.0.0-rc4-local
	URL https://github.com/LucasLi1024/bcos-cpp-sdk/archive/e6ec662ffdd0974dd1ee8df7c17dd80f52daf65e.tar.gz
	SHA1 33a7d6fa0e62d57c60836998dc46d68bef60ada4
)
