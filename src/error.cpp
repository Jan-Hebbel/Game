#include "error.h"

#include <iostream>
#include <format>

auto glfw_error_callback(int error, const char* description) -> void
{
    std::cout << std::format("Error {}: {}", error, description) << '\n';
}

void GLAPIENTRY opengl_error_callback(
    unsigned int source,
    unsigned int type,
    unsigned int id,
    unsigned int severity,
    int length,
    const char* message,
    const void* userParam
)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
    {
        fprintf(stderr, "ERROR: type = 0x%x, severity: HIGH, message = %s\n", type, message);
        exit(-1);
        break;
    }
    case GL_DEBUG_SEVERITY_MEDIUM:
    {
        fprintf(stderr, "WARNING: type = 0x%x, severity: MEDIUM, message = %s\n", type, message);
        break;
    }
    case GL_DEBUG_SEVERITY_LOW:
    {
        fprintf(stderr, "WARNING: type = 0x%x, severity: LOW, message = %s\n", type, message);
        break;
    }
    case GL_DEBUG_SEVERITY_NOTIFICATION:
    {
        fprintf(stderr, "NOTIFICATION: type = 0x%x, severity: NOTIFICATION, message = %s\n", type, message);
        break;
    }
    }
}