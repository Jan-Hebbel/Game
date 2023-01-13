#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "error.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"
#include "renderer.h"
#include "texture.h"

#include "tests/test_clear_color.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

auto main() -> int {
    // Init GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << '\n';
        return -1;
    }

    // GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwSetErrorCallback(glfw_error_callback);
#endif

    // setup a glfw window and context
    GLFWwindow* window = glfwCreateWindow(960, 540, "Game", NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW window creation failed" << '\n';
        return -1;
    }

    // make the window to current context
    glfwMakeContextCurrent(window);

    //glfwSwapInterval(1); // "turn on vsync"
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << '\n';
        return -1;
    }

    // debugging
    int flags; 
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(opengl_error_callback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    /*int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);*/

    // set up blending for transparent textures ---------
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    // --------------------------------------------------

    // this relative path is only correct for running in visual studio debug mode
    // check properties -> debugging -> working directory
    Shader shader("res/shaders/shader.vert", "res/shaders/shader.frag");
    shader.Bind();

    Texture texture("res/textures/4x.png");
    texture.Bind();
    // first 0 is the id that is specified in the shader by layout(location = 0)
    // second 0 is specified by the GL_TEXTURE0 Enum set in the Bind function
    shader.SetUniform1i(0, 0);

    Renderer renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450 core"); // "version 460 core" seems to work as well

    test::Test* current_test = nullptr;
    test::TestMenu* test_menu = new test::TestMenu(current_test);
    current_test = test_menu;
    
    test_menu->RegisterTest<test::TestClearColor>("Clear Color");
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (current_test)
        {
            current_test->OnUpdate(0.0f);
            current_test->OnRender();
            ImGui::Begin("Test");
            if (current_test != test_menu && ImGui::Button("<-"))
            {
                delete current_test;
                current_test = test_menu;
            }
            current_test->OnImGuiRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    if (current_test != test_menu)
        delete test_menu;
    delete current_test;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
} // vbo and ibo are deleted after opengl context is destroyed... might have to change? ask discord! video: Abstracting OpenGL into Classes

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
