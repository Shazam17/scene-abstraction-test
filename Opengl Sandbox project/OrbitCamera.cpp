#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(float fov, float th, float f) : theta(th), fi(f), fov(glm::radians(fov)) {

}

void OrbitCamera::processMouseKeys(const Application& app) {
	if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		pressed = true;
		posLastPressed = glm::vec2(x, y);
	}

	if (glfwGetMouseButton(app.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		pressed = false;
	}
}

void OrbitCamera::processMouseMove(const Application& app) {
	double xpos, ypos;
	glfwGetCursorPos(app.window, &xpos, &ypos);

	x = xpos;
	y = ypos;

	if (pressed) {
		//deltaX = -deltaX;
		glm::vec2 pos = glm::vec2(xpos, ypos) - posLastPressed;
		pos /= pos.length();


		theta -= pos.x * 0.01;
		fi += pos.y * 0.01;

		if (fi < 0) {
			fi = 0.01;
		}


		if (fi > 3.14) {
			fi = 3.13999;
		}
	}

}

glm::mat4 OrbitCamera::getViewMat() {

	float X = -radius * cos(theta) * sin(fi);
	float Y = radius * cos(fi);
	float Z = radius * sin(theta) * sin(fi);
	view = glm::lookAt(glm::vec3(X, Y, Z), glm::vec3(0), glm::vec3(0, 1, 0));
	return view;
}

glm::mat4 OrbitCamera::getProjectionMat(const Application& app) {
	return glm::perspective(fov, app.aspect, 0.1f, 100.0f);
}
