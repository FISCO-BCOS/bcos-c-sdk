set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSTATICLIB")

file(GLOB SRC_LIST "*.c")
file(GLOB HEADERS "*.h")

add_executable(get-block-number get-block-number.c)
target_link_libraries(get-block-number PRIVATE c-sdk bcos-cpp-sdk::cpp-sdk)

add_executable(pub pub.c)
target_link_libraries(pub PRIVATE c-sdk bcos-cpp-sdk::cpp-sdk)

add_executable(broadcast broadcast.c)
target_link_libraries(broadcast PRIVATE c-sdk bcos-cpp-sdk::cpp-sdk)

add_executable(sub sub.c)
target_link_libraries(sub PRIVATE c-sdk bcos-cpp-sdk::cpp-sdk)