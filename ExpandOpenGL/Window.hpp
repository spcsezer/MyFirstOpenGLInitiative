#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

class Window
{

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat changeX;
	GLfloat changeY;
	bool mouseFirstMoved;

	void createCallBacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);

public:

	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int initialize();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }

	GLfloat getXChange();
	GLfloat getYChange();
	GLFWwindow* getRawWindow() { return mainWindow; }

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

};

