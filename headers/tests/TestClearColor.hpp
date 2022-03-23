#ifndef TESTCLEARCOLOR_HPP
# define TESTCLEARCOLOR_HPP

# include "Tests.hpp"

# include "Renderer.hpp"

namespace test
{
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_ClearColor[4];
	};
}

#endif