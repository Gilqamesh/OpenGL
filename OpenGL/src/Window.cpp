#include "Window.hpp"
#include <iostream>

Window::Window(const std::string& name, int width, int height)
	: window(glfwCreateWindow(960, 540, "Hello World", NULL, NULL))
{
	if (window == nullptr)
	{
		std::cerr << "glfwCreateWindow() failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { glViewport(0, 0, width, height); });

	std::cout << glGetString(GL_VERSION) << std::endl;
}

Window::~Window()
{
	glfwTerminate();
}


void Window::processInput(void) const
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}