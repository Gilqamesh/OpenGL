#pragma once
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
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
	};
}

#endif
