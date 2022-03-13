#include "tests/TestCamera.hpp"
#include "VertexBufferLayout.hpp"
#include "Utils.hpp"

namespace test
{
	TestCamera::TestCamera(Window& window)
		: m_window(window),
		m_Model(identity_matrix<float, 4, 4>()),
		m_View(translation_matrix(Vector<float, 3>(0.0f, 0.0f, 0.0f))),
		//m_Proj(projection_matrix_ortho(0.0f, 960.0f, 0.0f, 540.0f, 0.1f, 100.0f)),
		m_Proj(projection_matrix_perspective(Utils::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f)),
		m_MVP(),
		m_Camera(
			Vector<GLfloat, 3>(0.0f, 0.0f, 5.0f),
			Vector<GLfloat, 3>(0.0f, 1.0f, 0.0f),
			-90.0f, 0.0f, 5.0f, 0.5f),
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
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		unsigned int indices[] = {
			0, 1, 2,
			0, 1, 3,
			0, 1, 4,
			1, 2, 3,
			1, 2, 4,
			2, 3, 4
		};
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6 * 3);

		m_Shader = std::make_unique<Shader>("res/shaders/camera.shader");

		m_VAO->Unbind();
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();
	}

	TestCamera::~TestCamera()
	{
		glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestCamera::OnUpdate(float deltaTime)
	{
        (void)deltaTime;
		float now = static_cast<float>(glfwGetTime());
		m_deltaTime = now - m_lastTime;
		m_lastTime = now;

		m_Camera.mouseControl(m_window.getXChange(), m_window.getYChange());
		m_Camera.keyControl(m_window.getKeys(), m_deltaTime);
		m_View = m_Camera.calculateViewMatrix();

		m_MVP = m_Model * m_View * m_Proj;
	}

	void TestCamera::OnRender()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Vertex initial[] = {
			Vertex(-0.5f,  0.0f,   0.5f),
			Vertex(0.5f,  0.0f,   0.5f),
			Vertex(0.5f,  0.0f,  -0.5f),
			Vertex(  -0.5f,  0.0f,  -0.5f),
			Vertex(0.0f,  2.0f,   0.0f)
		};
		m_VAO->Bind();
		m_VertexBuffer->Bind();
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(initial), initial));

		m_Shader->Bind();
		m_Shader->SetUniformMat4fv("u_MVP", 1, m_MVP);

		GLCall(glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, nullptr));

		m_Shader->Unbind();
		m_VAO->Unbind();
	}

	void TestCamera::OnImGuiRender()
	{

	}
}
