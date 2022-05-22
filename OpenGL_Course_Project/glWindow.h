#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

class glWindow
{
private:
	GLFWwindow* mainWindow;
	GLint width;
	GLint height;

	GLint bufferWidth;
	GLint bufferHeight;

	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat changeX;
	GLfloat changeY;
	bool mouseFirstMoved;

	void createCallbacks();

	static void handleKeys(GLFWwindow* window, const int key, const int code, const int action, const int mode);
	static void handleMouse(GLFWwindow* window, const double xPos, const double yPos);

public:
	glWindow();
	glWindow(GLint windowWidth, GLint windowHeight);

	int initialize();

	void swapBuffers();

	GLint getBufferWidth() const;
	GLint getBufferHeight() const;

	bool* getKeys();

	GLfloat getXChange();
	GLfloat getYChange();

	bool GetShouldClose();

	~glWindow();
};

