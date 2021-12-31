#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.hpp"

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "math/Matrix.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.hpp"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // limit fps
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "glewInit() error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
           -50.0f, -50.0f, 0.0f, 0.0f,   // 0
            50.0f, -50.0f, 1.0f, 0.0f,   // 1
            50.0f,  50.0f, 1.0f, 1.0f,   // 2
           -50.0f,  50.0f, 0.0f, 1.0f    // 3
        };

        // Index buffer
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // To enable blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray     va;
        VertexBuffer    vb(positions, 4 * 4 * sizeof(float));   // 4 vertexes and 4 float each

        VertexBufferLayout  layout;
        // Position coordinates
        layout.Push<float>(2);
        // Texture coordinates
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer     ib(indices, 6);

        Matrix<float, 4, 4> proj(projection_matrix_ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)); // projection
        Matrix<float, 4, 4> view(translation_matrix(Vector<float, 3>(0.0f, 0.0f, 0.0f)));

        Shader  shader("res/shaders/Basic.shader");
        shader.Bind();

        Texture texture("res/textures/landscape.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer    renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        Vector<float, 3> translationA(Vector<float, 3>(200.0f, 200.0f, 0.0f));
        Vector<float, 3> translationB(Vector<float, 3>(400.0f, 200.0f, 0.0f));

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            shader.Bind();

            {
                Matrix<float, 4, 4> model(translation_matrix(translationA));
                Matrix<float, 4, 4> mvp = model * view * proj;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }


            {
                Matrix<float, 4, 4> model(translation_matrix(translationB));
                Matrix<float, 4, 4> mvp = model * view * proj;
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            {
                ImGui::SliderFloat3("Translation A", &translationA, 0.0f, 960.0f);
                ImGui::SliderFloat3("Translation B", &translationB, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
