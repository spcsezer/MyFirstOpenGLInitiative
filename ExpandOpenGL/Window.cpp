#include "Window.hpp"
bool checkMouseHandle = false;

Window::Window() {

	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight) {

	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = 0;
	}
}

int Window::initialize() {

	if (!glfwInit()) {

		cout << "glfw intialization is failed.";
		glfwTerminate();
		return 1;
	}

	//window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	mainWindow = glfwCreateWindow(width, height, "firstopenglproject", NULL, NULL);
	if (!mainWindow) {

		cout << "window creation is failed";
		glfwTerminate();
		return 1;

	}

	//buffer size
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//glew context (you can switch screen for games)
	glfwMakeContextCurrent(mainWindow);

	//input key + mouse
	createCallBacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Mouse hide

	//allow modern extension
	glewExperimental = GLFW_TRUE;

	if (glewInit() != GLEW_OK) {

		cout << "glew init failed";
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;

	}

	glEnable(GL_DEPTH_TEST);

	//set viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallBacks() {

	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);

}

GLfloat Window::getXChange() {

	GLfloat theChange = changeX;
	changeX = 0.0f;
	return theChange;

}

GLfloat Window::getYChange() {

	GLfloat theChange = changeY;
	changeY = 0.0f;
	return theChange;

}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {

	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {

		glfwSetWindowShouldClose(window, GL_TRUE);

	}
	else if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		checkMouseHandle = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		checkMouseHandle = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (key >= 0 && key < 1024) {

		if (action == GLFW_PRESS) {

			theWindow->keys[key] = true;

		}
		else if (action == GLFW_RELEASE) {

			theWindow->keys[key] = false;

		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {

	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (checkMouseHandle == false) {

		if (theWindow->mouseFirstMoved) {

			theWindow->lastX = xPos;
			theWindow->lastY = yPos;
			theWindow->mouseFirstMoved = false;

		}

		theWindow->changeX = xPos - theWindow->lastX;
		theWindow->changeY = theWindow->lastY - yPos;

		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
	}
}

Window::~Window() {

	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
