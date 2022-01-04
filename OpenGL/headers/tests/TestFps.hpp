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

namespace test
{
	class TestFps : public Test
	{
	public:
		TestFps(Window& window);
		~TestFps();

		struct Vertex
		{
			float                Position[3];
		};

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		TestFps();

		Window& m_window;
		Matrix<float, 4, 4> m_View;
		Matrix<float, 4, 4> m_Proj;
		Matrix<float, 4, 4> m_MVP;
		Camera				m_Camera;
		float				m_deltaTime;
		float				m_lastTime;

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<VertexArray> m_VAOGround;
	};
}

#endif