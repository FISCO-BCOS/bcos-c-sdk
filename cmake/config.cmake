hunter_config(bcos-boostssl
	VERSION 3.0.0-local
	URL "https://${URL_BASE}/FISCO-BCOS/bcos-boostssl/archive/87888f85eb34154166ea30c7b28974d0bfa849f2.tar.gz"
	SHA1 21567a7b4732d424983455e2ac97d5e199a1cf6e
)

hunter_config(bcos-cpp-sdk
	VERSION 3.0.0-local
	URL "https://${URL_BASE}/ywy2090/bcos-cpp-sdk/archive/45ce90850ea056fc803306ffc2353cb4dccc22d8.tar.gz"
	SHA1 bc94d6c3f1ad316b180c3c268326b6b6ff081845
)

hunter_config(bcos-framework VERSION 3.0.0-local
	URL https://${URL_BASE}/FISCO-BCOS/bcos-framework/archive/d4697aa6b984a18e2cab7f91c098480bf5e61ebd.tar.gz
	SHA1 ab2fa84f449aa90b108731734bfd28dc5e78da22
	CMAKE_ARGS HUNTER_PACKAGE_LOG_BUILD=ON HUNTER_PACKAGE_LOG_INSTALL=ON
)