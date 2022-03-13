#include "tests/TestScene.hpp"
#include "VertexBufferLayout.hpp"
#include "Utils.hpp"

namespace test
{
	TestScene::TestScene(Window& window)
		: m_window(window),
		m_View(translation_matrix(Vector<float, 3>(0.0f, 0.0f, 0.0f))),
		m_Proj(),
		m_MVP(),
		m_Camera(
			Vector<GLfloat, 3>(0.0f, 0.0f, 3.0f),
			Vector<GLfloat, 3>(0.0f, 1.0f, 0.0f),
			-90.0f, 0.0f, 2.5f, 0.25f),
		m_deltaTime(0.0f), m_lastTime(0.0f)
	{
		m_window.setDrawWireframeMode();
		glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		GLCall(glEnable(GL_DEPTH_TEST));

		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

		m_VAO = std::make_unique<VertexArray>();
		m_VAO->Bind();

		m_VertexBuffer = std::make_unique<VertexBuffer>(sizeof(Vertex) * 1000);

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_Shader = std::make_unique<Shader>("res/shaders/scene.shader");

		m_Texture = std::make_unique<Texture>("res/textures/brick.png");
		m_Texture->Bind();

		m_VAO->Unbind();
		m_VertexBuffer->Unbind();
	}

	TestScene::~TestScene()
	{
		glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestScene::OnUpdate(float deltaTime)
	{
        (void)deltaTime;
		float now = static_cast<float>(glfwGetTime());
		m_deltaTime = now - m_lastTime;
		m_lastTime = now;

		m_Proj = projection_matrix_perspective(Utils::radians(m_window.getZoom()), 960.0f / 540.0f, 0.1f, 100.0f);
		m_Camera.mouseControl(m_window.getXChange(), m_window.getYChange());
		m_Camera.keyControl(m_window.getKeys(), m_deltaTime);
		m_View = m_Camera.calculateViewMatrix();
		/*m_View = look_at(Vector<float, 3>(10.0f * sin(now), 0.0f, 10.0f * cos(now)),
			Vector<float, 3>(0.0f, 0.0f, 0.0f), Vector<float, 3>(0.0f, 1.0f, 0.0f));*/
	}

	void TestScene::OnRender()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Vector<float, 3> cubePositions[] = {
			Vector<float, 3>(0.0f, 0.0f, 0.0f),
			Vector<float, 3>(2.0f, 5.0f, -15.0f),
			Vector<float, 3>(-1.5f, -2.2f, -2.5f),
			Vector<float, 3>(3.8f, -2.0f, -12.3f),
			Vector<float, 3>(2.4f, -0.4f, -3.5f),
			Vector<float, 3>(-1.7f, 3.0f, -7.5f),
			Vector<float, 3>(1.3f, -2.0f, -2.5f),
			Vector<float, 3>(1.5f, 2.0f, -2.5f),
			Vector<float, 3>(1.5f, 0.2f, -1.5f),
			Vector<float, 3>(-1.3f, 1.0f, -1.5f),
		};

		Vertex pyramid[] = {
		    Vertex(0.0f,  0.0f,   0.0f, 0.0f, 0.0f),
			Vertex(0.5f,  0.0f,   0.0f, 1.0f, 0.0f),
			Vertex(0.5f,  0.5f,   0.0f, 1.0f, 1.0f),

		    Vertex(0.5f,  0.5f,   0.0f, 1.0f, 1.0f),
			Vertex(0.0f,  0.5f,   0.0f, 0.0f, 1.0f),
			Vertex(0.0f,  0.0f,   0.0f, 0.0f, 0.0f),

			Vertex(0.0f,  0.0f,   0.0f, 0.0f, 0.0f),
			Vertex(0.5f,  0.0f,   0.0f, 1.0f, 0.0f),
			Vertex(0.25f, 0.25f,  1.0f, 0.5f, 1.0f),

			Vertex(0.5f,  0.0f,   0.0f, 0.0f, 0.0f),
			Vertex(0.5f,  0.5f,   0.0f, 1.0f, 0.0f),
			Vertex(0.25f, 0.25f,  1.0f, 0.5f, 1.0f),

			Vertex(0.5f,  0.5f,   0.0f, 0.0f, 0.0f),
			Vertex(0.0f,  0.5f,   0.0f, 1.0f, 0.0f),
			Vertex(0.25f, 0.25f,  1.0f, 0.5f, 1.0f),

			Vertex(0.0f,  0.5f,   0.0f, 0.0f, 0.0f),
			Vertex(0.0f,  0.0f,   0.0f, 1.0f, 0.0f),
			Vertex(0.25f, 0.25f,  1.0f, 0.5f, 1.0f)
		};

		m_VAO->Bind();
		m_VertexBuffer->Bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pyramid), pyramid));

		m_Shader->Bind();

		m_Shader->SetUniform1i("u_Texture", 0);
		for (unsigned int i = 0; i < 10; ++i)
		{
			Matrix<float, 4, 4> model(translation_matrix(cubePositions[i]));
			model *= rotation_matrix(Utils::radians(20.0f) * i,
				Vector<float, 3>(1.0f, 0.3f, 0.5f));
			m_MVP = model * m_View * m_Proj;
			m_Shader->SetUniformMat4fv("u_MVP", 1, m_MVP);
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 18));
		}


		m_Shader->Unbind();
		m_VAO->Unbind();
	}

	void TestScene::OnImGuiRender()
	{

	}
}
