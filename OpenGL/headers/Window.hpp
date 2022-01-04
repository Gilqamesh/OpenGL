#pragma once
#ifndef WINDOW_HPP
# define WINDOW_HPP

# include <GL/glew.h>
# include <string>
# include <GLFW/glfw3.h>

class Window
{
public:
	Window(const std::string &, int, int);
	~Window();

	GLFWwindow* getWindow(void) { return (window); }

	void setDrawWireframeMode(void) const;
	void setDrawDefaultMode(void)   const;

	bool*		getKeys() { return (keys); }
	GLfloat		getXChange(void);
	GLfloat		getYChange(void);
	GLfloat		getZoom(void) const { return (zoom); }
private:
	Window();

	GLFWwindow*	window;

	// mouse
	GLfloat     lastX;
	GLfloat     lastY;
	GLfloat     xChange;
	GLfloat     yChange;
	GLfloat     zoom;
	
	bool		keys[1024];
	
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
	static void handleScroll(GLFWwindow* window, double xOffset, double yOffset);
};

#endif
