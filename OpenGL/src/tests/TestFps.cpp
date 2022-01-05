#include "tests/TestFps.hpp"
#include "VertexBufferLayout.hpp"

namespace test
{
	TestFps::TestFps(Window& window)
		: m_window(window),
		m_View(translation_matrix(Vector<float, 3>(0.0f, 0.0f, 5.0f))),
		m_Proj(),
		m_MVP(),
		m_Camera(
			Vector<GLfloat, 3>(0.0f, 5.0f, 00.0f),
			Vector<GLfloat, 3>(0.0f, 1.0f, 0.0f),
			-90.0f, 0.0f, 5.0f, 0.25f),
		groundQuads(),
		groundWidth(10),
		groundHeight(10),
		m_CameraMode(Camera::FREE)
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
		std::vector<std::array<unsigned int, 6>> indicesGround;
		for (unsigned int i = 0; i < 10000; ++i)
		{
			indicesGround.push_back({
				i * 4 + 0,
				i * 4 + 1,
				i * 4 + 2,
				i * 4 + 2,
				i * 4 + 3,
				i * 4 + 0
				});
		}
		m_ShaderGround = std::make_unique<Shader>("res/shaders/ground.shader");
		m_VAOGround = std::make_unique<VertexArray>();
		m_VAOGround->Bind();
		m_IndexBufferGround = std::make_unique<IndexBuffer>(&indicesGround[0], 6 * 10000);
		for (unsigned int j = 0; j < 100; ++j)
			for (unsigned int i = 0; i < 100; ++i)
				groundQuads.push_back(Utils::CreateQuad(static_cast<float>(i), 0.0f, static_cast<float>(j), 1.0f, 0.5f, 0.2f, 0.1f, 1.0f));
		m_VertexBufferGround = std::make_unique<VertexBuffer>(groundQuads.size() * sizeof(Utils::QuadColor), &groundQuads[0]);
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
		// Update Members
		bool *keys = m_window.getKeys();
		if (keys[GLFW_KEY_1])
		{
			glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_CameraMode = Camera::FREE;
		}
		if (keys[GLFW_KEY_2])
		{
			glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_CameraMode = Camera::FPS;
		}
		if (keys[GLFW_KEY_3])
		{
			m_CameraMode = Camera::TOPDOWN;
			glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		// Projection Matrix
		m_Proj = projection_matrix_perspective(Utils::radians(m_window.getZoom()), 960.0f / 540.0f, 0.1f, 1000.0f);

		// View Matrix
		m_Camera.mouseControl(m_window.getXChange(), m_window.getYChange());
		m_Camera.setMode(m_CameraMode);
		m_Camera.keyControl(m_window.getKeys(), deltaTime);
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
		m_VAOGround->Bind();
		m_VertexBufferGround->Bind();
		m_IndexBufferGround->Bind();
		// GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quads), &quads));
		m_ShaderGround->Bind();
		m_MVP = m_View * m_Proj;
		m_ShaderGround->SetUniformMat4fv("u_MVP", 1, m_MVP);
		GLCall(glDrawElements(GL_TRIANGLES, 6 * 100 * 100, GL_UNSIGNED_INT, nullptr));
		m_VAOGround->Unbind();
		m_VertexBufferGround->Unbind();
		m_ShaderGround->Unbind();
		m_IndexBufferGround->Unbind();
	}

	void TestFps::OnImGuiRender()
	{
		ImGui::Text("Camera options:\n(1) Free camera\n(2) FPS\n(3) Top Down\n\n");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Current mode: ");
		switch (m_Camera.getMode())
		{
		case Camera::FREE:
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Free");
			break;
		case Camera::FPS:
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "FPS");
			break;
		case Camera::TOPDOWN:
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Top down");
			break;
		}
	}
}
