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
#include "tests/TestTexture2D.hpp"
#include "tests/TestCamera.hpp"
#include "tests/TestScene.hpp"
#include "tests/TestFps.hpp"

#include "Window.hpp"

int main(void)
{
    TRACE();
    Window window("Hello World", 1340, 680);
    GLint tmp;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &tmp);
    LOG("GL_MAX_TEXTURE_IMAGE_UNITS (texture map access from the fragment shader): " << tmp);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &tmp);
    LOG("GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS (texture map access from the vertex shader): " << tmp);

    {

        // To enable blending
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window.getWindow(), false);
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
        testMenu->RegisterTest<test::TestCamera>("Camera", window);
        testMenu->RegisterTest<test::TestScene>("Scene", window);
        testMenu->RegisterTest<test::TestFps>("Fps", window);

        float lastTime = static_cast<float>(glfwGetTime());
        while (!glfwWindowShouldClose(window.getWindow()))
        {
            ImGui_ImplGlfwGL3_NewFrame();
            if (currentTest)
            {
                float timeNow = static_cast<float>(glfwGetTime());
                float deltaTime = timeNow - lastTime;
                lastTime = timeNow;

                currentTest->OnUpdate(deltaTime);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && window.getKeys()[GLFW_KEY_BACKSPACE])
                {
                    delete currentTest;
                    currentTest = testMenu;
                    window.setDrawDefaultMode();
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            GLCall(glfwPollEvents());
            GLCall(glfwSwapBuffers(window.getWindow()));
        }
        if (currentTest != testMenu)
            delete testMenu;
        delete currentTest;
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    return 0;
}
