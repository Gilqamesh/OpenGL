#pragma once
#ifndef WINDOW_HPP
# define WINDOW_HPP

# include <string>
# include <GLFW/glfw3.h>

class Window
{
public:
	Window(const std::string &, int, int);
	~Window();

	GLFWwindow* const& getWindow(void) const { return (window); }
	void processInput(void) const;
private:
	Window();

	GLFWwindow*	window;
};

#endif
