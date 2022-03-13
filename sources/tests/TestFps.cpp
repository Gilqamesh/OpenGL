#include "tests/TestFps.hpp"
#include "VertexBufferLayout.hpp"

namespace test
{
	void TestFps::TestFpsInit(void)
	{
        TRACE();
		m_View = translation_matrix(Vector<float, 3>(0.0f, 0.0f, 5.0f));
		m_CameraMode = Camera::cameraModeType::FREE;
		m_MoveSpeed = 5.0f;
		m_EnvMaterial = Material(32.0f);
		m_LightSourcePoint = LightSource(LightSource::LightType::POINT, Vector<float, 3>(20.0f, 20.0f, 20.0f), Vector<float, 4>(1.0f, 1.0f, 1.0f, 1.0f),
			0.1f, 0.5f, 1.0f);
		ambient = Texture(Vector<float, 4>(0.3f, 0.15f, 0.1f, 1.0f));
		diffuse = Texture(Vector<float, 4>(0.3f, 0.15f, 0.1f, 1.0f));
		specular = Texture(Vector<float, 4>(0.5f, 0.5f, 0.5f, 1.0f));
		emission = Texture(Vector<float, 4>(0.0f, 0.0f, 0.0f, 1.0f));
		//m_GroundMaterial = Material(32.0f, Vector<float, 3>(0.3f, 0.15f, 0.1f));
		m_GroundMaterial = Material(32.0f, ambient, diffuse, specular, emission);
	}

	TestFps::TestFps(Window& window)
		: m_window(window),
		m_Camera(Vector<GLfloat, 3>(0.0f, 5.0f, 0.0f), Vector<GLfloat, 3>(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.25f),
		m_LightSourceSpotLight(LightSource::LightType::SPOTLIGHT, Vector<float, 3>(0.0f, 5.0f, 0.0f), Vector<float, 4>(1.0f, 1.0f, 0.0f, 1.0f),
			Vector<float, 3>(0.0f, 0.0f, -1.0f), Utils::radians(10.0f), Utils::radians(12.5f), 0.0f, 0.5f, 1.0f)
	{
        TRACE();
		TestFpsInit();
		glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		GLCall(glEnable(GL_DEPTH_TEST));

		GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

		configureEnvironment();
		configureGround();
		configureLightSourcePoint();
	}

	TestFps::~TestFps()
	{
		glfwSetInputMode(m_window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestFps::OnUpdate(float deltaTime)
	{
        TRACE();
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
			m_LightSourcePoint.setPosition() += Vector<float, 3>(0.0f, 0.0f, 1.0f) * velocity;
		}
		if (keys[GLFW_KEY_DOWN])
		{
			m_LightSourcePoint.setPosition() -= Vector<float, 3>(0.0f, 0.0f, 1.0f) * velocity;
		}
		if (keys[GLFW_KEY_LEFT])
		{
			m_LightSourcePoint.setPosition() -= Vector<float, 3>(-1.0f, 0.0f, 0.0f) * velocity;
		}
		if (keys[GLFW_KEY_RIGHT])
		{
			m_LightSourcePoint.setPosition() += Vector<float, 3>(-1.0f, 0.0f, 0.0f) * velocity;
		}
		if (keys[GLFW_KEY_RIGHT_SHIFT])
		{
			m_LightSourcePoint.setPosition() += Vector<float, 3>(0.0f, 1.0f, 0.0f) * velocity;
		}
		if (keys[GLFW_KEY_RIGHT_CONTROL])
		{
			m_LightSourcePoint.setPosition() -= Vector<float, 3>(0.0f, 1.0f, 0.0f) * velocity;
		}

		// Projection Matrix
		m_Proj = projection_matrix_perspective(
			Utils::radians(m_window.getZoom()),
			static_cast<float>(m_window.getWidth()) / static_cast<float>(m_window.getHeight()), 0.1f, 1000.0f);

		// View Matrix
		m_Camera.setMode(m_CameraMode);
		m_Camera.mouseControl(m_window.getXChange(), m_window.getYChange());
		m_Camera.keyControl(m_window.getKeys(), deltaTime);
		m_LightSourceSpotLight.setPosition() = m_Camera.getPosition();
		m_LightSourceSpotLight.setDirection() = m_Camera.getDirection();
		m_View = m_Camera.calculateViewMatrix();
	}

	void TestFps::OnRender()
	{
        TRACE();
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		renderEnvironment();
		renderGround();
		renderLightSourcePoint();
	}

	void TestFps::OnImGuiRender()
	{
        TRACE();
		enum menuSetting
		{
			MENU,
			POINT_LIGHT,
			SPOT_LIGHT,
			CUBES,
			GROUND
		};

		static menuSetting currentSetting;

		switch (currentSetting)
		{
		case MENU:
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
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Lighting Settings");
			if (ImGui::Button("Point Light"))
				currentSetting = POINT_LIGHT;
			if (ImGui::Button("Spot Light"))
				currentSetting = SPOT_LIGHT;

			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Cube Settings");
			if (ImGui::Button("Cubes"))
				currentSetting = CUBES;

			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Ground Settings");
			if (ImGui::Button("Ground"))
				currentSetting = GROUND;
			break;
		case POINT_LIGHT:
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Point Light Settings\n\n");

			ImGui::ColorEdit4("Color\n", &m_LightSourcePoint.setColor()[0]);

			ImGui::SliderFloat("Ambient Factor\n", &m_LightSourcePoint.setAmbientFactor(), 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse Factor\n", &m_LightSourcePoint.setDiffuseFactor(), 0.0f, 1.0f);
			ImGui::SliderFloat("Specular Factor\n", &m_LightSourcePoint.setSpecularFactor(), 0.0f, 1.0f);

			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Attenuation\n");
			ImGui::SliderFloat("Constant factor\n", &m_LightSourcePoint.setAttenuationFactor_Constant(), 1.0f, 10.0f);
			ImGui::SliderFloat("Linear factor\n", &m_LightSourcePoint.setAttenuationFactor_Linear(), 0.0014f, 0.7f);
			ImGui::SliderFloat("Quadratic factor\n", &m_LightSourcePoint.setAttenuationFactor_Quadratic(), 0.000007f, 1.8f);

			ImGui::SliderFloat3("Position", m_LightSourcePoint.setPosition().data(), 0.0f, 100.0f);

			if (ImGui::Button("back <-"))
				currentSetting = MENU;
			break;
		case SPOT_LIGHT:
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Spot Light Settings\n\n");

			ImGui::ColorEdit4("Color\n", &m_LightSourceSpotLight.setColor()[0]);

			ImGui::SliderFloat("Ambient Factor\n", &m_LightSourceSpotLight.setAmbientFactor(), 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse Factor\n", &m_LightSourceSpotLight.setDiffuseFactor(), 0.0f, 1.0f);
			ImGui::SliderFloat("Specular Factor\n", &m_LightSourceSpotLight.setSpecularFactor(), 0.0f, 1.0f);

			ImGui::SliderFloat("Inner Cutoff Angle\n", &m_LightSourceSpotLight.setInnerCutOffAngle(), 0.0f, Utils::radians(m_window.getZoom()));
			ImGui::SliderFloat("Outer Cutoff Angle\n", &m_LightSourceSpotLight.setOuterCutOffAngle(), 0.0f, Utils::radians(m_window.getZoom()));

			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Attenuation\n");
			ImGui::SliderFloat("Constant factor\n", &m_LightSourceSpotLight.setAttenuationFactor_Constant(), 1.0f, 10.0f);
			ImGui::SliderFloat("Linear factor\n", &m_LightSourceSpotLight.setAttenuationFactor_Linear(), 0.0014f, 0.7f);
			ImGui::SliderFloat("Quadratic factor\n", &m_LightSourceSpotLight.setAttenuationFactor_Quadratic(), 0.000007f, 1.8f);

			if (ImGui::Button("back <-"))
				currentSetting = MENU;
			break;
		case CUBES:
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Cube Settings\n\n");

			ImGui::SliderFloat("Shininess", &m_EnvMaterial.setShininessFactor(), 2.0f, 256.0f);

			if (ImGui::Button("back <-"))
				currentSetting = MENU;
			break;
		case GROUND:
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Ground Settings\n\n");

			ImGui::SliderFloat("Shininess\n", &m_GroundMaterial.setShininessFactor(), 2.0f, 256.0f);

			if (ImGui::Button("back <-"))
				currentSetting = MENU;
			break;
		}
	}
	
	void TestFps::configureEnvironment(void)
	{
        TRACE();
		m_EnvShader = std::make_unique<Shader>("res/shaders/material/material.vert", "res/shaders/material/material.frag", "Environment Shader");

		std::array<Mesh::Vertex, 24> temp = Utils::CreateCube(0.0f, 0.0f, 0.0f, 1.0f);
		for (unsigned int i = 0; i < 24; ++i)
			envCubes.push_back(temp[i]);
		temp = Utils::CreateCube(20.0f, 5.0f, 15.0f, 1.0f);
		for (unsigned int i = 0; i < 24; ++i)
			envCubes.push_back(temp[i]);
		temp = Utils::CreateCube(10.5f, 2.2f, 2.5f, 1.0f);
		for (unsigned int i = 0; i < 24; ++i)
			envCubes.push_back(temp[i]);
		temp = Utils::CreateCube(30.8f, 2.0f, 12.3f, 1.0f);
		for (unsigned int i = 0; i < 24; ++i)
			envCubes.push_back(temp[i]);
		temp = Utils::CreateCube(20.4f, 0.4f, 3.5f, 1.0f);
		for (unsigned int i = 0; i < 24; ++i)
			envCubes.push_back(temp[i]);
		temp = Utils::CreateCube(10.7f, 3.0f, 7.5f, 1.0f);
		for (unsigned int i = 0; i < 24; ++i)
			envCubes.push_back(temp[i]);
		temp = Utils::CreateCube(10.3f, 2.0f, 30.5f, 1.0f);
		for (unsigned int i = 0; i < 24; ++i)
			envCubes.push_back(temp[i]);
		temp = Utils::CreateCube(10.5f, 2.0f, 20.5f, 1.0f);
		for (unsigned int i = 0; i < 24; ++i)
			envCubes.push_back(temp[i]);
		temp = Utils::CreateCube(10.5f, 0.2f, 40.5f, 1.0f);
		for (unsigned int i = 0; i < 24; ++i)
			envCubes.push_back(temp[i]);
		temp = Utils::CreateCube(10.3f, 1.0f, 50.5f, 1.0f);
		for (unsigned int i = 0; i < 24; ++i)
			envCubes.push_back(temp[i]);

		for (unsigned int i = 0; i < 10; ++i)
		{
			indicesEnvCubes.insert(indicesEnvCubes.end(), {
				i * 24 + 0,  i * 24 + 1,  i * 24 + 2,  i * 24 + 2,  i * 24 + 3,  i * 24 + 0,
				i * 24 + 4,  i * 24 + 5,  i * 24 + 6,  i * 24 + 6,  i * 24 + 7,  i * 24 + 4,
				i * 24 + 8,  i * 24 + 9,  i * 24 + 10, i * 24 + 10, i * 24 + 11, i * 24 + 8,
				i * 24 + 12, i * 24 + 13, i * 24 + 14, i * 24 + 14, i * 24 + 15, i * 24 + 12,
				i * 24 + 16, i * 24 + 17, i * 24 + 18, i * 24 + 18, i * 24 + 19, i * 24 + 16,
				i * 24 + 20, i * 24 + 21, i * 24 + 22, i * 24 + 22, i * 24 + 23, i * 24 + 20
				});
		}

		m_textures.push_back(std::make_unique<Texture>("res/textures/brick.png"));
		m_textures.push_back(std::make_unique<Texture>("res/textures/brick_specular.png"));
		m_textures.push_back(std::make_unique<Texture>("res/textures/brick_emission.jpg"));
		m_EnvMaterial.addTexture(*m_textures[0], *m_textures[0], *m_textures[1]);
		m_EnvMesh = std::make_unique<Mesh>(envCubes, indicesEnvCubes, m_EnvMaterial);
	}

	void TestFps::configureGround(void)
	{
        TRACE();
		for (unsigned int i = 0; i < 10000; ++i)
		{
			indicesGroundQuads.insert(indicesGroundQuads.end(), {
				i * 4 + 0,
				i * 4 + 1,
				i * 4 + 2,
				i * 4 + 2,
				i * 4 + 3,
				i * 4 + 0
				});
		}

		m_GroundShader = std::make_unique<Shader>("res/shaders/material/material.vert","res/shaders/material/material.frag", "Ground Shader");
		for (unsigned int j = 0; j < 100; ++j)
		{
			for (unsigned int i = 0; i < 100; ++i)
			{
				auto temp = Utils::CreateQuad(static_cast<float>(i), 0.0f, static_cast<float>(j), 1.0f);
				for (auto vertex : temp)
					groundQuads.push_back(vertex);
			}
		}

		m_GroundMesh = std::make_unique<Mesh>(groundQuads, indicesGroundQuads, m_GroundMaterial);
	}
	void TestFps::configureLightSourcePoint(void)
	{
        TRACE();
		// Shader
		m_LightSourcePointShader = std::make_unique<Shader>("res/shaders/lightSource/lightSource.vert",
			"res/shaders/lightSource/lightSource.frag", "LightSourcePoint Shader");

		// Configure
		m_LightSourcePointVAO = std::make_unique<VertexArray>();
		m_LightSourcePointVAO->Bind();
		auto cube = Utils::CreateCube(0.0f, 0.0f, 0.0f, 1.0f);
		m_LightSourcePointVBO = std::make_unique<VertexBuffer>(sizeof(cube), &cube[0]);
		VertexBufferLayout layoutLightSource;
		layoutLightSource.Push<float>(3);
		layoutLightSource.Push<float>(3);
		layoutLightSource.Push<float>(2);
		m_LightSourcePointVAO->AddBuffer(*m_LightSourcePointVBO, layoutLightSource);

		auto cubeIndeces = Utils::CreateCube_Indices();
		m_LightSourcePointEBO = std::make_unique<IndexBuffer>(&cubeIndeces[0], 36);

		// Unbind
		m_LightSourcePointShader->Unbind();
		m_LightSourcePointVAO->Unbind();
		m_LightSourcePointVBO->Unbind();
		m_LightSourcePointEBO->Unbind();
	}
	
	void TestFps::renderEnvironment(void)
	{
        TRACE();
		m_EnvShader->Bind();
		Matrix<float, 4, 4> model(identity_matrix<float, 4, 4>());
		m_EnvShader->SetUniformMat4fv("model", 1, model);
		m_EnvShader->SetUniformMat4fv("view", 1, m_View);
		m_EnvShader->SetUniformMat4fv("projection", 1, m_Proj);
		m_EnvShader->SetUniformMat4fv("normalMatrix", 1, normal_matrix(model));
		m_EnvShader->SetUniform3f("viewPos", m_Camera.getPosition());
		m_EnvShader->SetUniformLightSource(m_LightSourcePoint, 0);
		m_EnvShader->SetUniformLightSource(m_LightSourceSpotLight, 1);
		m_EnvMesh->drawMesh(*m_EnvShader);
		m_EnvShader->Unbind();
	}
	void TestFps::renderGround(void)
	{
        TRACE();
		m_GroundShader->Bind();
		Matrix<float, 4, 4> model = identity_matrix<float, 4, 4>();
		m_GroundShader->SetUniformMat4fv("model", 1, model);
		m_GroundShader->SetUniformMat4fv("view", 1, m_View);
		m_GroundShader->SetUniformMat4fv("projection", 1, m_Proj);
		m_GroundShader->SetUniformMat4fv("normalMatrix", 1, normal_matrix(model));
		m_GroundShader->SetUniform3f("viewPos", m_Camera.getPosition());
		m_GroundShader->SetUniformLightSource(m_LightSourcePoint, 0);
		m_GroundShader->SetUniformLightSource(m_LightSourceSpotLight, 1);
		m_GroundMesh->drawMesh(*m_GroundShader);
		m_GroundShader->Unbind();
	}
	void TestFps::renderLightSourcePoint(void)
	{
        TRACE();
		m_LightSourcePointVAO->Bind();
		m_LightSourcePointVBO->Bind();
		m_LightSourcePointEBO->Bind();
		m_LightSourcePointShader->Bind();
		Matrix<float, 4, 4> model = scale_matrix(Vector<float, 3>(0.1f, 0.1f, 0.1f)) * translation_matrix(m_LightSourcePoint.getPosition());
		m_LightSourcePointShader->SetUniformMat4fv("model", 1, model);
		m_LightSourcePointShader->SetUniformMat4fv("view", 1, m_View);
		m_LightSourcePointShader->SetUniformMat4fv("projection", 1, m_Proj);
		m_LightSourcePointShader->SetUniformLightSource(m_LightSourcePoint);
		GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
		m_LightSourcePointVAO->Unbind();
		m_LightSourcePointVBO->Unbind();
		m_LightSourcePointEBO->Unbind();
		m_LightSourcePointShader->Unbind();
	}
}
