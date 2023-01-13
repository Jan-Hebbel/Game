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

    // "turn on vsync"
    //glfwSwapInterval(1);

    // glfw callback functions
    glfwSetKeyCallback(window, key_callback);

    // load all OpenGL functions using the glfw loader function
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

    // create an OpenGL Viewport
    // apparently you dont need to do this
    /*int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);*/

    // set up blending for transparent textures ---------
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    // --------------------------------------------------

    // triangle data
    /*float rectangle[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f
    };*/

    float rectangle[] = {
        200.0f, 200.0f, 0.0f, 0.0f,
        200.0f, 300.0f, 0.0f, 1.0f,
        300.0f, 300.0f, 1.0f, 1.0f,
        300.0f, 200.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

    // create vertex buffer, automatically bound
    VertexBuffer vbo(rectangle, sizeof(float) * 4 * 4);

    // create vertex array
    VertexArray vao;
    VertexBufferLayout layout;
    // 2 calls are necessary because 2 attributes are defined, else offset would be off
    layout.Push(GL_FLOAT, 2);
    layout.Push(GL_FLOAT, 2);
    vao.AddBuffer(vbo, layout);

    // create element buffer for rectangle
    IndexBuffer ibo(indices, 6);

    // create matrices to move the world and its objects to create the illusion of a camera ----
    //glm::mat4 projection = glm::ortho(-8.0f, 8.0f, -4.5f, 4.5f, -1.0f, 1.0f); // perspective or ortho
    glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(-100.0f, 0.0f, 0.0f));
    // -----------------------------------------------------------------------------------------

    // create shader
    // this relative path is only correct for running in visual studio debug mode
    // check properties -> debugging -> working directory
    Shader shader("res/shaders/shader.vert", "res/shaders/shader.frag");
    shader.Bind();

    Texture texture("res/textures/4x.png");
    texture.Bind();
    // first 0 is the id that is specified in the shader by layout(location = 0)
    // second 0 is specified by the GL_TEXTURE0 Enum set in the Bind function
    shader.SetUniform1i(0, 0);

    /*shader.Unbind();
    vao.Unbind();
    vbo.Unbind();
    ibo.Unbind();*/

    Renderer renderer;

    // setting up imgui -----------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450 core"); // "version 460 core" seems to work as well

    // ImGui window variables
    float f = 0.5f;
    glm::vec3 translation(200.0f, 200.0f, 0.0f);
    // ----------------------------------

    // window stays open loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and process events
        glfwPollEvents();

        // rendering
        renderer.Clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // bind and uniforms
        shader.Bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, translation);
        glm::mat4 mvp = projection * view * model;
        shader.SetUniformMat4f(2, mvp);

        // draw
        renderer.Draw(vao, ibo, shader);

        // Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::SliderFloat2("Translation", &translation[0], 0.0f, 960.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // imgui render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // swap front and back buffers
        glfwSwapBuffers(window);
    }

    // ImGui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // delete things
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
