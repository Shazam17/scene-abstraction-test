#pragma once

#include "ResourceManager.h"
#include "Transform.h"

class GameObject {
public:
	int id;
	Model model;
	Shader shader;
	Transform transform;

	string name;
	GameObject(const string& n, const Model& m, const Shader& s, const Transform& t) : name(n) {
		model = m;
		shader = s;
		transform = t;
	}


	string toString() const {
		stringstream ss;

		ss << "GAMEOBJECT" << endl;
		ss << "MESH: " << model.name << endl;
		ss << "SHADER: " << shader.name << endl;
		ss << "POSITION: " << transform.pos.x << ' ' << transform.pos.y << ' ' << transform.pos.z << endl;
		ss << "SCALE: " << transform.scaleVec.x << ' ' << transform.scaleVec.y << ' ' << transform.scaleVec.z << endl;


		return ss.str();
	}



};
