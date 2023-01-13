#pragma once
#include <glad/glad.h>

#define ASSERT(x) if (!(x)) __debugbreak();

auto glfw_error_callback(int error, const char* description) -> void;

void GLAPIENTRY opengl_error_callback(
    unsigned int source,
    unsigned int type,
    unsigned int id,
    unsigned int severity,
    int length,
    const char* message,
    const void* userParam
);