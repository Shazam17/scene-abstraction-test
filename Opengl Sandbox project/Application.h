#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <functional>

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
	float scrollOffset = 0;
	friend class OrbitCamera;
	ImGuiIO io;
public:

	float mouseX;
	float mouseY;

	enum class KEY_PRESS_STATE {
		KEY_PRESSED = GLFW_PRESS,
		KEY_RELEASED = GLFW_RELEASE,
		NONE
	};

	enum class MOUSE_KEY_STATE {
		LEFT = GLFW_MOUSE_BUTTON_LEFT,
		RIGHT,
		MIDDLE,
		NONE
	};

	enum class MOUSE_MOVE_STATE {
		Stationary,
		Moving
	};

	KEY_PRESS_STATE mouseKeyPressState = KEY_PRESS_STATE::NONE;
	MOUSE_MOVE_STATE mouseMoveState = MOUSE_MOVE_STATE::Moving;
	MOUSE_KEY_STATE mouseKeyTypeState = MOUSE_KEY_STATE::NONE;
	std::function<void(int, int)> mouseMove;
	std::function<void(MOUSE_KEY_STATE, KEY_PRESS_STATE)> clickListener;


	Application(float w, float h, const std::string& windowName);
	bool isRunning();
	void clear(const glm::vec4& clearColor = glm::vec4(0.0f));
	void swapBuffers();

	static void mouseWheelCallback(GLFWwindow* window, double xOff, double yOff);
	static void mouseMoveCallback(GLFWwindow* window, double xPos, double yPos);
	static void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
	static void resizeCallback(GLFWwindow* window,int width, int height);



	

	void calcMouseState(const glm::vec2& tMouseCord);


	void resizeWindow(int width, int height);
	void mouseScrollCalback(double xOff,double yOff);

	void setMouseClickListener(std::function<void(const Application& app, MOUSE_KEY_STATE, KEY_PRESS_STATE)> cb);

	float getWidth() const { return width; }
	float getHeight() const { return height; }

	~Application();

};