add_executable(keypair keypair.c)
target_link_libraries(keypair PUBLIC ${BCOS_C_SDK_STATIC_TARGET})

add_executable(deploy_hello deploy_hello.c)
target_link_libraries(deploy_hello PUBLIC ${BCOS_C_SDK_STATIC_TARGET})

add_executable(call_hello call_hello.c)
target_link_libraries(call_hello PUBLIC ${BCOS_C_SDK_STATIC_TARGET})