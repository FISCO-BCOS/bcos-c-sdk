add_executable(hello_sample hello_sample.c)
target_link_libraries(hello_sample PUBLIC ${BCOS_C_SDK_STATIC_TARGET})

add_executable(hello_sample_hsm hello_sample_hsm.c)
target_link_libraries(hello_sample_hsm PUBLIC ${BCOS_C_SDK_STATIC_TARGET})

add_executable(tx_struct_test tx_struct_test.c)
target_link_libraries(tx_struct_test PUBLIC ${BCOS_C_SDK_STATIC_TARGET})

add_executable(tx_struct_v2_test tx_struct_v2_test.c)
target_link_libraries(tx_struct_v2_test PUBLIC ${BCOS_C_SDK_STATIC_TARGET})

# add_executable(tx_sign_perf tx_sign_perf.cpp)
# target_link_libraries(tx_sign_perf PUBLIC ${BCOS_C_SDK_STATIC_TARGET})