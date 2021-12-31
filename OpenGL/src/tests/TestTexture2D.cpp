#include "tests/TestTexture2D.hpp"

#include "math/Matrix.hpp"

namespace test
{
    TestTexture2D::TestTexture2D()
        : m_TranslationA((Vector<float, 3>(200.0f, 200.0f, 0.0f))),
        m_TranslationB((Vector<float, 3>(400.0f, 200.0f, 0.0f))),
        m_Proj(projection_matrix_ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(translation_matrix(Vector<float, 3>(0.0f, 0.0f, 0.0f)))
	{
        float positions[] = {
           -50.0f, -50.0f, 0.0f, 0.0f,
            50.0f, -50.0f, 1.0f, 0.0f,
            50.0f,  50.0f, 1.0f, 1.0f,
           -50.0f,  50.0f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout  layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/textures/landscape.jpg");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D() {}

	void TestTexture2D::OnUpdate(float deltaTime) {}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        Renderer renderer;
        
        m_Texture->Bind();

        Matrix<float, 4, 4> proj(projection_matrix_ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)); // projection
        Matrix<float, 4, 4> view(translation_matrix(Vector<float, 3>(0.0f, 0.0f, 0.0f)));


        {
            Matrix<float, 4, 4> model(translation_matrix(m_TranslationA));
            Matrix<float, 4, 4> mvp = model * m_View * m_Proj;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }


        {
            Matrix<float, 4, 4> model(translation_matrix(m_TranslationB));
            Matrix<float, 4, 4> mvp = model * m_View * m_Proj;
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
	}

	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_TranslationA, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
