#ifndef TESTTEXTURE2D_HPP
# define TESTTEXTURE2D_HPP

# include "Tests.hpp"

# include "Renderer.hpp"
# include "VertexBufferLayout.hpp"
# include "Texture.hpp"

# include <memory>

namespace test
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		struct Vertex
		{
            Vertex() { }
            Vertex(float a, float b, float c, float d, float e, float f)
            {
                Position[0] = a;
                Position[1] = b;
                Position[2] = c;
                TexCoords[0] = d;
                TexCoords[1] = e;
                TexID = f;
            }
			float                Position[3];
			float                TexCoords[2];
			float                TexID;
		};

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		Vector<float, 3> m_TranslationA;
		Vector<float, 3> m_TranslationB;
		Matrix<float, 4, 4> m_Proj;
		Matrix<float, 4, 4> m_View;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Texture> m_SandTex;
		std::unique_ptr<Texture> m_BrickTex;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
	};
}

#endif
