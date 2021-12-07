set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTATICLIB")

file(GLOB SRC_LIST "*.cpp" "*.c")
file(GLOB HEADERS "*.h")

add_executable(get-block-number get-block-number.c)
target_link_libraries(get-block-number PRIVATE fisco-bcos-c-jni-static)

add_executable(pub pub.c)
target_link_libraries(pub PRIVATE fisco-bcos-c-jni-static)

add_executable(broadcast broadcast.c)
target_link_libraries(broadcast PRIVATE fisco-bcos-c-jni-static)

add_executable(sub sub.c)
target_link_libraries(sub PRIVATE fisco-bcos-c-jni-static)