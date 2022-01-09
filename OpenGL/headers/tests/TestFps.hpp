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
# include "Material.hpp"
# include "LightSource.hpp"
# include "Mesh.hpp"
# include "Model.hpp"

namespace test
{
	class TestFps : public Test
	{
	public:
		TestFps(Window& window);
		~TestFps();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		TestFps();

		Window&									m_window;
		Matrix<float, 4, 4>						m_View;
		Matrix<float, 4, 4>						m_Proj;
		Camera									m_Camera;
		Camera::cameraModeType					m_CameraMode;
		float									m_MoveSpeed;

		std::unique_ptr<Shader>					m_EnvShader;
		Material								m_EnvMaterial;
		std::unique_ptr<Mesh>					m_EnvMesh;

		Texture ambient;
		Texture diffuse;
		Texture specular;
		Texture emission;
		std::unique_ptr<Shader>					m_GroundShader;
		Material								m_GroundMaterial;
		std::unique_ptr<Mesh>					m_GroundMesh;

		std::unique_ptr<VertexArray>			m_LightSourcePointVAO;
		std::unique_ptr<VertexBuffer>			m_LightSourcePointVBO;
		std::unique_ptr<IndexBuffer>			m_LightSourcePointEBO;
		std::unique_ptr<Shader>					m_LightSourcePointShader;
		LightSource								m_LightSourcePoint;

		LightSource								m_LightSourceSpotLight;

		std::vector<std::unique_ptr<Texture>>	m_textures;
		std::vector<Mesh::Vertex>				envCubes;
		std::vector<unsigned int>				indicesEnvCubes;
		std::vector<Mesh::Vertex>				groundQuads;
		std::vector<unsigned int>				indicesGroundQuads;

		Model									backpackModel;
		std::unique_ptr<Shader>					backpackShader;

		void TestFpsInit(void);

		void configureEnvironment(void);
		void configureGround(void);
		void configureLightSourcePoint(void);

		void renderEnvironment(void);
		void renderGround(void);
		void renderLightSourcePoint(void);
		void renderModels(void);
	};
}

#endif
