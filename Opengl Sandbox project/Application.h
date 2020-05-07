#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "vendor/imgui.h"
#include "vendor/imgui_impl_glfw.h"
#include "vendor/imgui_impl_opengl3.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Application {
private:
	GLFWwindow* window;
	float aspect;
	float width;
	float height;
	friend class OrbitCamera;

public:
	Application(float w, float h, const std::string& windowName);
	bool isRunning();
	void clear(const glm::vec4& clearColor = glm::vec4(0.0f));
	void swapBuffers();
	~Application();

};