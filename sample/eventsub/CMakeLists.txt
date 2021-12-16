set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSTATICLIB")

file(GLOB SRC_LIST "*.c")
file(GLOB HEADERS "*.h")

add_executable(eventsub eventsub.c)
target_link_libraries(eventsub PUBLIC ${BCOS_C_SDK_STATIC_TARGET})