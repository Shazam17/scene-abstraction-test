#include "Application.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class OrbitCamera {

	float theta;
	float fi;
	float fov;

	float radius = 2.0f;
	
	float x;
	float y;

	bool pressed = false;
	glm::vec2 posLastPressed;
	glm::mat4 view;

public:

	OrbitCamera(float fov = 45.0f, float th = 3.14f / 4, float f = 3.14f / 4);
	void processMouseKeys(const Application& app);
	void processMouseMove(const Application& app);

	glm::mat4 getViewMat();
	glm::mat4 getProjectionMat(const Application& app);

};