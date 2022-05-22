#include "glWindow.h"

glWindow::glWindow()
{
	mainWindow = nullptr;

	width = 800;
	height = 600;

	bufferWidth = 0;
	bufferHeight = 0;

	for (int i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}
	
	changeX = 0.0f;
	changeY = 0.0f;
}

void glWindow::createCallbacks() 
{
	glfwSetKeyCallback(mainWindow, handleKeys);

	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void glWindow::handleKeys(GLFWwindow* window, const int key, const int code, const int action, const int mode)
{
	glWindow* usedWindow = static_cast<glWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			usedWindow->keys[key] = true;
		}
		else if(action == GLFW_RELEASE)
		{
			usedWindow->keys[key] = false;
		}
	}

}

void glWindow::handleMouse(GLFWwindow* window, const double xPos, const double yPos) 
{
	glWindow* usedWindow = static_cast<glWindow*>(glfwGetWindowUserPointer(window));

	if (usedWindow->mouseFirstMoved)
	{
		usedWindow->lastX = xPos;
		usedWindow->lastY = yPos;
		usedWindow->mouseFirstMoved = false;
	}

	usedWindow->changeX = xPos - usedWindow->lastX;
	usedWindow->changeY = usedWindow->lastY - yPos;

	usedWindow->lastX = xPos;
	usedWindow->lastY = yPos;
}

glWindow::glWindow(const GLint windowWidth, const GLint windowHeight)
{
	mainWindow = nullptr;

	width = windowWidth;
	height = windowHeight;

	bufferWidth = 0;
	bufferHeight = 0;

	for (int i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}
}

int glWindow::initialize()
{
	if (!glfwInit())
	{
		printf("Error");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

	if (mainWindow == nullptr)
	{
		printf("GFLW Window creation failed!");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(mainWindow);

	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW Initialization Failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);
}

void glWindow::swapBuffers()
{
	glfwSwapBuffers(mainWindow);
}

GLint glWindow::getBufferWidth() const
{
	return bufferWidth;
}

GLint glWindow::getBufferHeight() const
{
	return bufferHeight;
}

bool glWindow::GetShouldClose()
{
	return glfwWindowShouldClose(mainWindow);
}

bool* glWindow::getKeys() 
{
	return keys;
}

GLfloat glWindow::getXChange()
{
	GLfloat temp = changeX;
	changeX = 0;
	return temp;
}
GLfloat glWindow::getYChange()
{
	GLfloat temp = changeY;
	changeY = 0;
	return temp;
}

glWindow::~glWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
