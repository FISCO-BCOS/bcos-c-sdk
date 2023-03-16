set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTATICLIB")

file(GLOB SRC_LIST "*.cpp" "*.c")
file(GLOB HEADERS "*.h")

add_executable(get-block-number get-block-number.c)
target_link_libraries(get-block-number PRIVATE ${FISCO_BCOS_JNI_STATIC_TARGET})

add_executable(pub pub.c)
target_link_libraries(pub PRIVATE ${FISCO_BCOS_JNI_STATIC_TARGET})

add_executable(broadcast broadcast.c)
target_link_libraries(broadcast PRIVATE ${FISCO_BCOS_JNI_STATIC_TARGET})

add_executable(sub sub.c)
target_link_libraries(sub PRIVATE ${FISCO_BCOS_JNI_STATIC_TARGET})

add_executable(sdk-start sdk-start.c)
target_link_libraries(sdk-start PRIVATE ${FISCO_BCOS_JNI_STATIC_TARGET})