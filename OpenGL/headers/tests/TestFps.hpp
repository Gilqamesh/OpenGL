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
		float							m_EnvAmbientStrength;
		float							m_EnvSpecularStrength;
		float							m_EnvShininess;

		std::unique_ptr<VertexArray>	m_GroundVAO;
		std::unique_ptr<VertexBuffer>	m_GroundVBO;
		std::unique_ptr<IndexBuffer>	m_GroundEBO;
		std::unique_ptr<Shader>			m_GroundShader;
		float							m_GroundAmbientStrength;
		float							m_GroundSpecularStrength;
		float							m_GroundShininess;

		std::unique_ptr<VertexArray>	m_LightSourceVAO;
		std::unique_ptr<VertexBuffer>	m_LightSourceVBO;
		std::unique_ptr<IndexBuffer>	m_LightSourceEBO;
		std::unique_ptr<Shader>			m_LightSourceShader;
		Vector<float, 4>				m_LightSourceColor;
		Vector<float, 3>				m_LightSourcePosition;

		std::vector<Utils::QuadColor_Normal>	groundQuads;
		unsigned int							groundWidth;
		unsigned int							groundHeight;

		void configureEnvironmentVAO(void);
		void configureGroundVAO(void);
		void configureLightSourceVAO(void);
	};
}

#endif
