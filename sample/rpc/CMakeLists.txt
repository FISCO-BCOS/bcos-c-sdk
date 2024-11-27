set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTATICLIB")

file(GLOB SRC_LIST "*.c")
file(GLOB HEADERS "*.h")

add_executable(rpc rpc.c)
target_link_libraries(rpc PUBLIC ${BCOS_C_SDK_STATIC_TARGET})

add_executable(repeat_connect repeat_connect.c)
target_link_libraries(repeat_connect PUBLIC ${BCOS_C_SDK_STATIC_TARGET})

