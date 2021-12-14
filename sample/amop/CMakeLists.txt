set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTATICLIB")

file(GLOB SRC_LIST "*.c")
file(GLOB HEADERS "*.h")

add_executable(publish publish.c)
target_link_libraries(publish PUBLIC ${BCOS_C_SDK_TARGET})

add_executable(broadcast broadcast.c)
target_link_libraries(broadcast PUBLIC ${BCOS_C_SDK_TARGET})

add_executable(subscribe subscribe.c)
target_link_libraries(subscribe PUBLIC ${BCOS_C_SDK_TARGET})