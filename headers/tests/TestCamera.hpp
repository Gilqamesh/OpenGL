#ifndef TESTCAMERA_HPP
# define TESTCAMERA_HPP

# include "GL/glew.h"
# include "Tests.hpp"
# include "Shader.hpp"
# include "VertexArray.hpp"
# include "IndexBuffer.hpp"
# include "VertexBuffer.hpp"
# include "math/Matrix.hpp"
# include "Window.hpp"
# include <memory>
# include "Camera.hpp"

namespace test
{
	class TestCamera : public Test
	{
	public:
		TestCamera(Window& window);
		~TestCamera();

		struct Vertex
		{
			Vertex() { }
			Vertex(float a, float b, float c)
			{
				Position[0] = a;
				Position[1] = b;
				Position[2] = c;
			}
			float                Position[3];
		};

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		TestCamera();

		Window&				m_window;
		Matrix<float, 4, 4> m_Model;
		Matrix<float, 4, 4> m_View;
		Matrix<float, 4, 4> m_Proj;
		Matrix<float, 4, 4> m_MVP;
		Camera				m_Camera;
		float				m_deltaTime;
		float				m_lastTime;

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
	};
}

#endif
