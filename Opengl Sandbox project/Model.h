#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include "Shader.h"
#include "Drawable.h"


struct Vertex {
	glm::vec3 pos;

	Vertex(float arr[]) {
		pos.x = arr[0];
		pos.y = arr[1];
		pos.z = arr[2];
	}
	Vertex(float x, float y, float z) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
	}
};
struct Triangle {
	unsigned points[3];
	Triangle(unsigned v1, unsigned v2, unsigned v3) {
		points[0] = v1;
		points[1] = v2;
		points[2] = v3;
	}
};
class Model : public Drawable{

	unsigned vbo, vao, ib;
	unsigned count;
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Triangle> faces;

public:
	std::string name;
	Model() {

	}
	Model(const std::vector<Vertex>& v, const std::vector<unsigned>& i,const std::vector<Triangle>& faces);
	static Model loadObj(const std::string& path);
	void draw (const Shader& shad) const;

};
