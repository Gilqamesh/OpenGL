#include "tests/TestFps.hpp"
#include "VertexBufferLayout.hpp"
#include "Utils.hpp"

namespace test
{
	TestFps::TestFps(Window& window)
		: m_window(window),
		m_View(translation_matrix(Vector<float, 3>(0.0f, 0.0f, 5.0f))),
		m_Proj(),
		m_MVP(),
		m_Camera(
			Vector<GLfloat, 3>(0.0f, 0.0f, 3.0f),
			Vector<GLfloat, 3>(0.0f, 1.0f, 0.0f),
			-90.0f, 0.0f, 2.5f, 0.25f),
		m_deltaTime(0.0f), m_lastTime(0.0f)
	{
		glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		GLCall(glEnable(GL_DEPTH_TEST));

		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

		m_VAO = std::make_unique<VertexArray>();
		m_VAO->Bind();

		m_VertexBuffer = std::make_unique<VertexBuffer>(sizeof(Vertex) * 1000);

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(1);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		unsigned int indices[] = {
			0,  1,  2,  2,  3,  0,
			4,  5,  6,  6,  7,  4,
			8,  9,  10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20,
		};
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);

		m_Shader = std::make_unique<Shader>("res/shaders/fps.shader");

		m_Texture = std::make_unique<Texture>("res/textures/brick.png");
		m_Texture->Bind();

		m_VAO->Unbind();
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();
		m_Shader->Unbind();

		// Ground Logic
		unsigned int indicesGround[] = {
			0, 1, 2, 2, 3, 0
		};
		m_IndexBufferGround = std::make_unique<IndexBuffer>(indicesGround, 6);
		m_ShaderGround = std::make_unique<Shader>("res/shaders/ground.shader");
		m_VAOGround = std::make_unique<VertexArray>();
		m_VAOGround->Bind();
		m_VertexBufferGround = std::make_unique<VertexBuffer>(sizeof(Utils::VertexColor) * 1000);
		VertexBufferLayout layoutGround;
		layoutGround.Push<float>(3);
		layoutGround.Push<float>(4);
		m_VAOGround->AddBuffer(*m_VertexBufferGround, layoutGround);
		m_VAOGround->Unbind();
		m_VertexBufferGround->Unbind();
		m_IndexBufferGround->Unbind();
	}

	TestFps::~TestFps()
	{
		glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestFps::OnUpdate(float deltaTime)
	{
		float now = static_cast<float>(glfwGetTime());
		m_deltaTime = now - m_lastTime;
		m_lastTime = now;

		m_Proj = projection_matrix_perspective(Utils::radians(m_window.getZoom()), 960.0f / 540.0f, 0.1f, 100.0f);
		m_Camera.mouseControl(m_window.getXChange(), m_window.getYChange());
		m_Camera.keyControl(m_window.getKeys(), m_deltaTime);
		m_View = m_Camera.calculateViewMatrix();
	}

	void TestFps::OnRender()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		auto cube = Utils::CreateCube(0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		m_VAO->Bind();
		m_VertexBuffer->Bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube), &cube));

		m_Shader->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);
		m_MVP = m_View * m_Proj;
		m_Shader->SetUniformMat4fv("u_MVP", 1, m_MVP);
		m_IndexBuffer->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
		m_VAO->Unbind();
		m_Shader->Unbind();
		m_IndexBuffer->Unbind();

		// Ground logic
		m_window.setDrawWireframeMode();
		auto quad = Utils::CreateQuad(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		m_VAOGround->Bind();
		m_VertexBufferGround->Bind();
		m_IndexBufferGround->Bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad), &quad));
		m_ShaderGround->Bind();
		for (unsigned int j = 0; j < 10; ++j)
		{
			for (unsigned int i = 0; i < 10; ++i)
			{
				Matrix<float, 4, 4> model = translation_matrix(Vector<float, 3>(static_cast<float>(i), static_cast<float>(j), 0.0f));
				model *= rotation_matrix(Utils::radians(90.0f), Vector<float, 3>(1.0f, 0.0f, 0.0f));
				m_MVP = model * m_View * m_Proj;
				m_ShaderGround->SetUniformMat4fv("u_MVP", 1, m_MVP);
				GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			}
		}
		m_VAOGround->Unbind();
		m_VertexBufferGround->Unbind();
		m_ShaderGround->Unbind();
		m_IndexBufferGround->Unbind();
		m_window.setDrawDefaultMode();
	}

	void TestFps::OnImGuiRender()
	{

	}
}
