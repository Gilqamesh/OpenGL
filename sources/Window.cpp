#include "Window.hpp"
#include "Renderer.hpp"
#include <iostream>

static void GLFWErrorCallback(int errCode, const char *msg)
{
    std::cout << "Error code: " << errCode << ", message: " << msg << std::endl;
}

Window::Window(const std::string& name, int width, int height)
	: width(width), height(height), window(nullptr),
	lastX(width / 2), lastY(height / 2), xChange(0), yChange(0), zoom(45.0f)
{
    TRACE();
	for (unsigned int i = 0; i < 1024; ++i)
		keys[i] = 0;

    glfwSetErrorCallback(&GLFWErrorCallback);

	if (!glfwInit())
	{
		std::cerr << "glfwInit() failed" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (window == nullptr)
	{
		std::cerr << "glfwCreateWindow() failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		std::cout << "glewInit() error" << std::endl;

	//glfwSwapInterval(1);

	// callbacks
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) { (void)window; glViewport(0, 0, width, height); });
	glfwSetKeyCallback(window, handleKeys);
	glfwSetCursorPosCallback(window, handleMouse);
	glfwSetScrollCallback(window, handleScroll);
	glfwSetWindowUserPointer(window, this);

	std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
}

Window::~Window()
{
    TRACE();
	glfwTerminate();
}

GLfloat Window::getXChange(void)
{
    TRACE();
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return (theChange);
}

GLfloat Window::getYChange(void)
{
    TRACE();
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return (theChange);
}

void Window::setDrawWireframeMode(void) const
{
    TRACE();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Window::setDrawDefaultMode(void) const
{
    TRACE();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    TRACE();
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	(void)code;
	(void)mode;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			theWindow->getKeys()[key] = true;
		else if (action == GLFW_RELEASE)
			theWindow->getKeys()[key] = false;
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    TRACE();
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	theWindow->xChange = static_cast<GLfloat>(xPos) - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - static_cast<GLfloat>(yPos);

	theWindow->lastX = static_cast<GLfloat>(xPos);
	theWindow->lastY = static_cast<GLfloat>(yPos);
}

void Window::handleScroll(GLFWwindow* window, double xOffset, double yOffset)
{
    TRACE();
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	(void)xOffset;

	theWindow->zoom -= static_cast<GLfloat>(yOffset);
	if (theWindow->zoom < 1.0f)
		theWindow->zoom = 1.0f;
	if (theWindow->zoom > 45.0f)
		theWindow->zoom = 45.0f;
}
