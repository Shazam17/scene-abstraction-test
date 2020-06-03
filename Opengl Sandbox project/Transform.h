#pragma once


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include "Component.h"

using namespace glm;

class Transform: public Component
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

		mat = rotate(mat, radians(rot.x), vec3(1, 0, 0));
		mat = rotate(mat, radians(rot.y), vec3(0, 1, 0));
		mat = rotate(mat, radians(rot.z), vec3(0, 0, 1));

		mat = scale(mat, scaleVec);
	}
	Transform(const vec3& p, const vec3& sc, const vec3& rot, std::map<std::string,Component*>*cmp): Component(cmp), pos(p), scaleVec(sc), rotation(rot) {
		
		mat = mat4(1.0f);

		mat = translate(mat, pos);

		mat = rotate(mat, radians(rot.x), vec3(1, 0, 0));
		mat = rotate(mat, radians(rot.y), vec3(0, 1, 0));
		mat = rotate(mat, radians(rot.z), vec3(0, 0, 1));

		mat = scale(mat, scaleVec);
	}
		
	void addRotate(const vec3& ad) {
		mat = mat4(1.0f);

		mat = translate(mat, pos);

		mat = rotate(mat,radians(rotation.x), vec3(1, 0, 0));
		mat = rotate(mat, radians(rotation.y), vec3(0, 1, 0));
		mat = rotate(mat, radians(rotation.z), vec3(0, 0, 1));
		
		mat = rotate(mat, radians(ad.x), vec3(1, 0, 0));
		mat = rotate(mat, radians(ad.y), vec3(0, 1, 0));
		mat = rotate(mat, radians(ad.z), vec3(0, 0, 1));

		mat = scale(mat, vec3(0.3f));
	}
		

	vec3& getPos() {
		return pos;
	}

	void setPos(const vec3& p) {

		pos = p;

		mat = mat4(1.0f);
	
		mat = scale(mat, scaleVec);

		mat = translate(mat, pos);

		mat = rotate(mat, rotation.x, vec3(1, 0, 0));
		mat = rotate(mat, rotation.y, vec3(0, 1, 0));
		mat = rotate(mat, rotation.z, vec3(0, 0, 1));

		
	}

	void recalculateMat() {
		mat = mat4(1.0f);

		mat = translate(mat, pos);

	
		mat = rotate(mat, radians(rotation.x), vec3(1, 0, 0));
		mat = rotate(mat, radians(rotation.y), vec3(0, 1, 0));
		mat = rotate(mat, radians(rotation.z), vec3(0, 0, 1));
		mat = scale(mat, scaleVec);

	}

	vec3& getRot() {
		return rotation;
	}

	vec3& getScale() {
		return scaleVec;
	}

	void setRot(const vec3& rot) {
		rotation = rot;

		mat = mat4(1.0f);

		mat = translate(mat, pos);
		mat = scale(mat, scaleVec);

		mat = rotate(mat, rotation.x, vec3(1, 0, 0));
		mat = rotate(mat, rotation.y, vec3(0, 1, 0));
		mat = rotate(mat, rotation.z, vec3(0, 0, 1));


	}

	void setScale(const vec3& scl) {

		scaleVec = scl;

		mat = mat4(1.0f);
	
		mat = translate(mat, pos);
		mat = scale(mat, scaleVec);

		mat = rotate(mat, rotation.x, vec3(1, 0, 0));
		mat = rotate(mat, rotation.y, vec3(0, 1, 0));
		mat = rotate(mat, rotation.z, vec3(0, 0, 1));

		
	}

	mat4 getModelMatrix() const {
		return mat;
	}

	void onStart() override {

	}

	void onUpdate() override {

	}


};

