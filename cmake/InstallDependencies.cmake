hunter_add_package(jsoncpp)
find_package(jsoncpp CONFIG REQUIRED)

# find OpenSSL
hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)

# Boost
hunter_add_package(Boost COMPONENTS all)
find_package(Boost CONFIG REQUIRED log chrono system filesystem thread iostreams)

# TASSL
hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)

# wedpr-crypto
hunter_add_package(wedpr-crypto)
find_package(wedpr-crypto CONFIG REQUIRED)

# bcos-utilities
hunter_add_package(bcos-utilities)
find_package(bcos-utilities CONFIG REQUIRED)

# hsm-crypto
include(InstallSDF)

# find bcos-crypto
hunter_add_package(bcos-crypto)
find_package(bcos-crypto CONFIG REQUIRED)

# find bcos-boostssl
hunter_add_package(bcos-boostssl)
find_package(bcos-boostssl CONFIG REQUIRED)

# find bcos-cpp-sdk
hunter_add_package(bcos-cpp-sdk)
find_package(bcos-cpp-sdk REQUIRED)
get_target_property(BCOS_CPP_SDK_INCLUDE bcos-cpp-sdk::bcos-cpp-sdk INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${BCOS_CPP_SDK_INCLUDE}/bcos-cpp-sdk)