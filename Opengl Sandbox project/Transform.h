#pragma once


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

class Transform
{
private:
	vec3 pos;
	vec3 scaleVec;
	vec3 rotation;

	mat4 mat;

	friend class GameObject;

public:
	Transform() {
		pos = scaleVec = rotation = vec3(0);
		mat = mat4(1.0f);
	}
	Transform(const vec3& p, const vec3& sc, const vec3& rot): pos(p), scaleVec(sc), rotation(rot) {
		mat = mat4(1.0f);
		mat = translate(mat, pos);
		mat = scale(mat, scaleVec);
	}

	void setPos(const vec3& p) {

		pos = p;

		mat = mat4(1.0f);
		mat = translate(mat, pos);
		mat = scale(mat, scaleVec);
	}

	void setScale(const vec3& scl) {

		scaleVec = scl;

		mat = mat4(1.0f);
		mat = translate(mat, pos);
		mat = scale(mat, scaleVec);
	}

	mat4 getModelMatrix() const {
		return mat;
	}

};

