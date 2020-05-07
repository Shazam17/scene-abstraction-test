#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include "Shader.h"


struct Vertex {
	glm::vec3 pos;

	Vertex(float arr[]) {
		pos.x = arr[0];
		pos.y = arr[1];
		pos.z = arr[2];
	}

};

class Model {

	unsigned vbo, vao, ib;
	unsigned count;
public:
	std::string name;
	Model() {

	}
	Model(const std::vector<Vertex>& v, const std::vector<unsigned>& i);
	static Model loadObj(const std::string& path);
	void draw(const Shader& shad) const;

};