#include "bcos_sdk_c_error.h"

#include <mutex>
#include <string>
#include <thread>

// record c error
thread_local int g_global_error = 0;
thread_local std::string g_global_msg;

int bcos_sdk_get_last_error()
{
    return g_global_error;
}

const char* bcos_sdk_get_last_error_msg()
{
    return g_global_msg.c_str();
}

void bcos_sdk_clear_last_error()
{
    g_global_error = 0;
    g_global_msg.clear();
}

void bcos_sdk_set_last_error_msg(int error, const char* msg)
{
    g_global_error = error;
    g_global_msg = std::string(msg);
}