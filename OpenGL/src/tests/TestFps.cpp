#include "tests/TestFps.hpp"
#include "VertexBufferLayout.hpp"

namespace test
{
	TestFps::TestFps(Window& window)
		: m_window(window),
		m_View(translation_matrix(Vector<float, 3>(0.0f, 0.0f, 5.0f))),
		m_Proj(),
		m_Camera(
			Vector<GLfloat, 3>(0.0f, 5.0f, 00.0f),
			Vector<GLfloat, 3>(0.0f, 1.0f, 0.0f),
			-90.0f, 0.0f, 5.0f, 0.25f),
		groundQuads(),
		groundWidth(10),
		groundHeight(10),
		m_CameraMode(Camera::cameraModeType::FREE),
		m_MoveSpeed(5.0f),
		m_EnvAmbientStrength(0.2f), m_EnvSpecularStrength(0.5f), m_EnvShininess(32.0f),
		m_GroundAmbientStrength(0.2f), m_GroundSpecularStrength(0.5f), m_GroundShininess(32.0f),
		m_LightSourceColor(1.0f, 1.0f, 1.0f, 1.0f),
		m_LightSourcePosition(20.0f, 20.0f, 20.0f)
	{
		glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		GLCall(glEnable(GL_DEPTH_TEST));

		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

		configureEnvironmentVAO();
		configureGroundVAO();
		configureLightSourceVAO();
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
		if (keys[GLFW_KEY_0])
		{
			glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_CameraMode = Camera::cameraModeType::OFF;
		}
		if (keys[GLFW_KEY_1])
		{
			glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_CameraMode = Camera::cameraModeType::FREE;
		}
		if (keys[GLFW_KEY_2])
		{
			glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_CameraMode = Camera::cameraModeType::FPS;
		}
		if (keys[GLFW_KEY_3])
		{
			glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_CameraMode = Camera::cameraModeType::TOPDOWN;
		}
		float velocity = m_MoveSpeed * deltaTime;
		if (keys[GLFW_KEY_UP])
		{
			m_LightSourcePosition += Vector<float, 3>(0.0f, 0.0f, 1.0f) * velocity;
		}
		if (keys[GLFW_KEY_DOWN])
		{
			m_LightSourcePosition -= Vector<float, 3>(0.0f, 0.0f, 1.0f) * velocity;
		}
		if (keys[GLFW_KEY_LEFT])
		{
			m_LightSourcePosition -= Vector<float, 3>(-1.0f, 0.0f, 0.0f) * velocity;
		}
		if (keys[GLFW_KEY_RIGHT])
		{
			m_LightSourcePosition += Vector<float, 3>(-1.0f, 0.0f, 0.0f) * velocity;
		}
		if (keys[GLFW_KEY_RIGHT_SHIFT])
		{
			m_LightSourcePosition += Vector<float, 3>(0.0f, 1.0f, 0.0f) * velocity;
		}
		if (keys[GLFW_KEY_RIGHT_CONTROL])
		{
			m_LightSourcePosition -= Vector<float, 3>(0.0f, 1.0f, 0.0f) * velocity;
		}

		// Projection Matrix
		m_Proj = projection_matrix_perspective(Utils::radians(m_window.getZoom()), 960.0f / 540.0f, 0.1f, 1000.0f);

		// View Matrix
		m_Camera.setMode(m_CameraMode);
		m_Camera.mouseControl(m_window.getXChange(), m_window.getYChange());
		m_Camera.keyControl(m_window.getKeys(), deltaTime);
		m_View = m_Camera.calculateViewMatrix();
	}

	void TestFps::OnRender()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// Environemnt
		m_EnvVAO->Bind();
		m_EnvShader->Bind();
		Matrix<float, 4, 4> model(identity_matrix<float, 4, 4>());
		m_EnvShader->SetUniformMat4fv("model", 1, model);
		m_EnvShader->SetUniformMat4fv("view", 1, m_View);
		m_EnvShader->SetUniformMat4fv("projection", 1, m_Proj);
		m_EnvShader->SetUniform1i("u_Texture", 0);
		m_EnvShader->SetUniform4f("u_LightColor", m_LightSourceColor);
		m_EnvShader->SetUniform1f("ambientStrength", m_EnvAmbientStrength);
		m_EnvShader->SetUniform3f("lightPosition", m_LightSourcePosition);
		m_EnvShader->SetUniformMat4fv("normalMatrix", 1, normal_matrix(model));
		m_EnvShader->SetUniform3f("viewPos", m_Camera.getPosition());
		m_EnvShader->SetUniform1f("specularStrength", m_EnvSpecularStrength);
		m_EnvShader->SetUniform1f("shininess", m_EnvShininess);
		GLCall(glDrawElements(GL_TRIANGLES, 36 * 10, GL_UNSIGNED_INT, nullptr));
		m_EnvVAO->Unbind();
		m_EnvShader->Unbind();

		// Ground logic
		m_GroundVAO->Bind();
		m_GroundShader->Bind();
		model = identity_matrix<float, 4, 4>();
		m_GroundShader->SetUniformMat4fv("model", 1, model);
		m_GroundShader->SetUniformMat4fv("view", 1, m_View);
		m_GroundShader->SetUniformMat4fv("projection", 1, m_Proj);
		m_GroundShader->SetUniform4f("u_LightColor", m_LightSourceColor);
		m_GroundShader->SetUniform1f("ambientStrength", m_GroundAmbientStrength);
		m_GroundShader->SetUniform3f("lightPosition", m_LightSourcePosition);
		m_GroundShader->SetUniformMat4fv("normalMatrix", 1, normal_matrix(model));
		m_GroundShader->SetUniform3f("viewPos", m_Camera.getPosition());
		m_GroundShader->SetUniform1f("specularStrength", m_GroundSpecularStrength);
		m_GroundShader->SetUniform1f("shininess", m_GroundShininess);
		GLCall(glDrawElements(GL_TRIANGLES, 6 * 100 * 100, GL_UNSIGNED_INT, nullptr));
		m_GroundVAO->Unbind();
		m_GroundShader->Unbind();

		// Light source
		m_LightSourceVAO->Bind();
		m_LightSourceShader->Bind();
		model = scale_matrix(Vector<float, 3>(0.1f, 0.1f, 0.1f)) * translation_matrix(m_LightSourcePosition);
		m_LightSourceShader->SetUniformMat4fv("model", 1, model);
		m_LightSourceShader->SetUniformMat4fv("view", 1, m_View);
		m_LightSourceShader->SetUniformMat4fv("projection", 1, m_Proj);
		m_LightSourceShader->SetUniform4f("lightSourceColor", m_LightSourceColor);
		GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
		m_LightSourceVAO->Unbind();
		m_LightSourceShader->Unbind();
	}

	void TestFps::OnImGuiRender()
	{
		ImGui::Text("Camera options:\n(0) OFF\n(1) Free camera\n(2) FPS\n(3) Top Down\n\n");
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Current mode: ");
		switch (m_Camera.getMode())
		{
		case Camera::cameraModeType::OFF:
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "OFF\n\n");
			break;
		case Camera::cameraModeType::FREE:
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Free\n\n");
			break;
		case Camera::cameraModeType::FPS:
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "FPS\n\n");
			break;
		case Camera::cameraModeType::TOPDOWN:
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Top down\n\n");
			break;
		}
		ImGui::ColorEdit4("Lighting Color\n", &m_LightSourceColor[0]);
		ImGui::SliderFloat("Cube shininess", &m_EnvShininess, 2.0f, 256.0f);
		ImGui::SliderFloat("Ground shininess\n", &m_GroundShininess, 2.0f, 256.0f);
		ImGui::SliderFloat("Cube specular strength", &m_EnvAmbientStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Ground specular strength\n", &m_GroundSpecularStrength, 0.0f, 1.0f);

	}
	void TestFps::configureEnvironmentVAO(void)
	{
		std::vector<Utils::CubeTex_Normal> cubes;
		cubes.push_back(Utils::CreateCube_Normal<Utils::CubeTex_Normal>(0.0f,  0.0f, 0.0f,  1.0f, 0.0f));
		cubes.push_back(Utils::CreateCube_Normal<Utils::CubeTex_Normal>(20.0f, 5.0f, 15.0f, 1.0f, 0.0f));
		cubes.push_back(Utils::CreateCube_Normal<Utils::CubeTex_Normal>(10.5f, 2.2f, 2.5f,  1.0f, 0.0f));
		cubes.push_back(Utils::CreateCube_Normal<Utils::CubeTex_Normal>(30.8f, 2.0f, 12.3f, 1.0f, 0.0f));
		cubes.push_back(Utils::CreateCube_Normal<Utils::CubeTex_Normal>(20.4f, 0.4f, 3.5f,  1.0f, 0.0f));
		cubes.push_back(Utils::CreateCube_Normal<Utils::CubeTex_Normal>(10.7f, 3.0f, 7.5f,  1.0f, 0.0f));
		cubes.push_back(Utils::CreateCube_Normal<Utils::CubeTex_Normal>(10.3f, 2.0f, 30.5f, 1.0f, 0.0f));
		cubes.push_back(Utils::CreateCube_Normal<Utils::CubeTex_Normal>(10.5f, 2.0f, 20.5f, 1.0f, 0.0f));
		cubes.push_back(Utils::CreateCube_Normal<Utils::CubeTex_Normal>(10.5f, 0.2f, 40.5f, 1.0f, 0.0f));
		cubes.push_back(Utils::CreateCube_Normal<Utils::CubeTex_Normal>(10.3f, 1.0f, 50.5f, 1.0f, 0.0f));

		m_EnvVAO = std::make_unique<VertexArray>();
		m_EnvVAO->Bind();

		m_EnvVBO = std::make_unique<VertexBuffer>(sizeof(Utils::CubeTex_Normal) * 10, &cubes[0]);

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);
		layout.Push<float>(1);
		layout.Push<float>(3);
		m_EnvVAO->AddBuffer(*m_EnvVBO, layout);

		std::vector<std::array<unsigned int, 36>> indicesCubes;
		for (unsigned int i = 0; i < 10; ++i)
		{
			indicesCubes.push_back({
				i * 24 + 0,  i * 24 + 1,  i * 24 + 2,  i * 24 + 2,  i * 24 + 3,  i * 24 + 0,
				i * 24 + 4,  i * 24 + 5,  i * 24 + 6,  i * 24 + 6,  i * 24 + 7,  i * 24 + 4,
				i * 24 + 8,  i * 24 + 9,  i * 24 + 10, i * 24 + 10, i * 24 + 11, i * 24 + 8,
				i * 24 + 12, i * 24 + 13, i * 24 + 14, i * 24 + 14, i * 24 + 15, i * 24 + 12,
				i * 24 + 16, i * 24 + 17, i * 24 + 18, i * 24 + 18, i * 24 + 19, i * 24 + 16,
				i * 24 + 20, i * 24 + 21, i * 24 + 22, i * 24 + 22, i * 24 + 23, i * 24 + 20
				});
		}
		m_EnvEBO = std::make_unique<IndexBuffer>(&indicesCubes[0], 36 * 10);

		m_EnvShader = std::make_unique<Shader>("res/shaders/fps.shader");

		m_EnvTexture = std::make_unique<Texture>("res/textures/brick.png");
		m_EnvTexture->Bind();

		m_EnvVAO->Unbind();
		m_EnvVBO->Unbind();
		m_EnvEBO->Unbind();
		m_EnvShader->Unbind();
	}
	void TestFps::configureGroundVAO(void)
	{
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
		m_GroundShader = std::make_unique<Shader>("res/shaders/ground.shader");
		m_GroundVAO = std::make_unique<VertexArray>();
		m_GroundVAO->Bind();
		m_GroundEBO = std::make_unique<IndexBuffer>(&indicesGround[0], 6 * 10000);
		for (unsigned int j = 0; j < 100; ++j)
			for (unsigned int i = 0; i < 100; ++i)
				groundQuads.push_back(Utils::CreateQuad_Normal<Utils::QuadColor_Normal>
					(static_cast<float>(i), 0.0f, static_cast<float>(j), 1.0f, 0.3f, 0.15f, 0.075f, 1.0f));
		m_GroundVBO = std::make_unique<VertexBuffer>(groundQuads.size() * sizeof(Utils::QuadColor_Normal), &groundQuads[0]);
		VertexBufferLayout layoutGround;
		layoutGround.Push<float>(3);
		layoutGround.Push<float>(4);
		layoutGround.Push<float>(3);
		m_GroundVAO->AddBuffer(*m_GroundVBO, layoutGround);

		// Unbind
		m_GroundVAO->Unbind();
		m_GroundVBO->Unbind();
		m_GroundEBO->Unbind();
		m_GroundShader->Unbind();
	}
	void TestFps::configureLightSourceVAO(void)
	{
		// Member variables
		m_LightSourceColor = Vector<float, 4>(1.0f, 1.0f, 1.0f, 1.0f);

		// Shader
		m_LightSourceShader = std::make_unique<Shader>("res/shaders/lightSource.shader");

		// Configure
		m_LightSourceVAO = std::make_unique<VertexArray>();
		m_LightSourceVAO->Bind();
		Utils::CubePosition cube = Utils::CreateCube<Utils::CubePosition>(0.0f, 0.0f, 0.0f, 1.0f);
		m_LightSourceVBO = std::make_unique<VertexBuffer>(sizeof(Utils::CubePosition), &cube[0]);
		VertexBufferLayout layoutLightSource;
		layoutLightSource.Push<float>(3);
		m_LightSourceVAO->AddBuffer(*m_LightSourceVBO, layoutLightSource);

		Utils::CubeIndeces cubeIndeces = Utils::CreateCube_Indices();
		m_LightSourceEBO = std::make_unique<IndexBuffer>(&cubeIndeces[0], 36);

		// Unbind
		m_LightSourceShader->Unbind();
		m_LightSourceVAO->Unbind();
		m_LightSourceVBO->Unbind();
		m_LightSourceEBO->Unbind();
	}
}
