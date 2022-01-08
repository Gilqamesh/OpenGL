#pragma once
#ifndef TESTFPS_HPP
# define TESTFPS_HPP

# include "GL/glew.h"
# include "Tests.hpp"
# include "Shader.hpp"
# include "VertexArray.hpp"
# include "IndexBuffer.hpp"
# include "VertexBuffer.hpp"
# include "math/Matrix.hpp"
# include "Texture.hpp"
# include "Window.hpp"
# include <memory>
# include "Camera.hpp"
# include "Utils.hpp"
# include "MaterialTypes.hpp"
# include "LightSource.hpp"

namespace test
{
	class TestFps : public Test
	{
	public:
		TestFps(Window& window);
		~TestFps();

		struct Vertex
		{
			float Position[3];
		};

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		TestFps();

		Window&					m_window;
		Matrix<float, 4, 4>		m_View;
		Matrix<float, 4, 4>		m_Proj;
		Camera					m_Camera;
		Camera::cameraModeType	m_CameraMode;
		float					m_MoveSpeed;

		std::unique_ptr<Texture>		m_EnvTexture;
		std::unique_ptr<Texture>		m_EnvTextureSpecular;
		std::unique_ptr<Texture>		m_EnvTextureEmission;
		std::unique_ptr<Shader>			m_EnvShader;
		std::unique_ptr<VertexArray>	m_EnvVAO;
		std::unique_ptr<VertexBuffer>	m_EnvVBO;
		std::unique_ptr<IndexBuffer>	m_EnvEBO;
		Material						m_EnvMaterial;

		std::unique_ptr<VertexArray>	m_GroundVAO;
		std::unique_ptr<VertexBuffer>	m_GroundVBO;
		std::unique_ptr<IndexBuffer>	m_GroundEBO;
		std::unique_ptr<Shader>			m_GroundShader;
		Material						m_GroundMaterial;

		std::unique_ptr<VertexArray>	m_LightSourcePointVAO;
		std::unique_ptr<VertexBuffer>	m_LightSourcePointVBO;
		std::unique_ptr<IndexBuffer>	m_LightSourcePointEBO;
		std::unique_ptr<Shader>			m_LightSourcePointShader;
		LightSource						m_LightSourcePoint;

		LightSource						m_LightSourceSpotLight;

		std::vector<Utils::QuadTex_Normal>		groundQuads;
		unsigned int							groundWidth;
		unsigned int							groundHeight;

		void TestFpsInit(void);

		void configureEnvironment(void);
		void configureGround(void);
		void configureLightSourcePoint(void);

		void renderEnvironment(void);
		void renderGround(void);
		void renderLightSourcePoint(void);
	};
}

#endif
