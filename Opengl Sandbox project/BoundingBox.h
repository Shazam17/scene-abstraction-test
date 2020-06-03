#pragma once
#include "Drawable.h"
#include "Model.h"

#include <glm/glm.hpp>

using namespace glm;
using namespace std;



class BoundingBox :public Drawable {

	unsigned vbo;
	unsigned vao;
	unsigned ib;

	
	unsigned count;
	mat4 modelMat;
public:
	vec3 localBounds[2];
	vec4 globalCoords[2];

	float len(vec3 v) {
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	BoundingBox() {

	}

	void setModelMat(const mat4& mMat) {
		modelMat = mMat;
		globalCoords[0] = modelMat * vec4(localBounds[0].x, localBounds[0].y, localBounds[0].z, 1.0f);
		globalCoords[1] = modelMat * vec4(localBounds[1].x, localBounds[1].y, localBounds[1].z, 1.0f);
	}

	BoundingBox(Vertex min,  Vertex max) {

		float xDim = abs(min.pos.x - max.pos.x);
		float yDim = abs(min.pos.y - max.pos.y);
		float zDim = abs(min.pos.z - max.pos.z);

		vector<Vertex> resultVertices;
		resultVertices.push_back(max);

		max.pos.x -= xDim;
		resultVertices.push_back(max);

		max.pos.z -= zDim;
		resultVertices.push_back(max);

		max.pos.x += xDim;
		resultVertices.push_back(max);


		resultVertices.push_back(min);

		min.pos.x += xDim;
		resultVertices.push_back(min);

		min.pos.z += zDim;
		resultVertices.push_back(min);

		min.pos.x -= xDim;
		resultVertices.push_back(min);




		std::vector<unsigned> i = vector<unsigned>{
			0,1, 1,2, 2,3 ,3,0 ,
			4,5, 5,6, 6,7, 7,4,
			2,4, 3,5, 0,6, 1,7 };

		localBounds[0] = resultVertices[0].pos;
		localBounds[1] = resultVertices[4].pos;
		count = i.size();
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * resultVertices.size(), &resultVertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * i.size(), &i[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	BoundingBox(std::vector<Vertex> v, std::vector<unsigned> i) {

		localBounds[0] = v[0].pos;
		localBounds[1] = v[4].pos;
		count = i.size();
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * v.size(), &v[0], GL_STATIC_DRAW);

		glGenBuffers(1, &ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * i.size(), &i[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	static BoundingBox loadObj(const std::string& path) {

		std::vector<Vertex> v;
		std::vector<unsigned> i;

		std::ifstream file;
		try {
			file.open(path);
		}
		catch (const std::exception& e) {
			std::cout << "failed to load model" << std::endl;
		}

		std::string buff;
		while (std::getline(file, buff)) {
			std::stringstream ss;
			ss << buff;
			std::string type;
			ss >> type;
			if (type.compare("v") == 0) {
				float cord[3];
				for (int i = 0; i < 3; i++) {
					ss >> cord[i];
				}
				v.push_back(Vertex(cord));
			}
		}

		//find max Point xyz

		Vertex max(0, 0, 0);
		Vertex min(0, 0, 0);

		for (int i = 0; i < v.size(); i++) {

			if (v[i].pos.x > max.pos.x) {
				max.pos.x = v[i].pos.x;
			}
			if (v[i].pos.y > max.pos.y) {
				max.pos.y = v[i].pos.y;
			}
			if (v[i].pos.z > max.pos.z) {
				max.pos.z = v[i].pos.z;
			}
		}

		//find min Point xyz
		for (int i = 0; i < v.size(); i++) {
			if (v[i].pos.x < min.pos.x) {
				min.pos.x = v[i].pos.x;
			}
			if (v[i].pos.y < min.pos.y) {
				min.pos.y = v[i].pos.y;
			}
			if (v[i].pos.z < min.pos.z) {
				min.pos.z = v[i].pos.z;
			}
		}

		//measure dimensions

		float xDim = abs(min.pos.x - max.pos.x);
		float yDim = abs(min.pos.y - max.pos.y);
		float zDim = abs(min.pos.z - max.pos.z);

		vector<Vertex> resultVertices;
		resultVertices.push_back(max);

		max.pos.x -= xDim;
		resultVertices.push_back(max);

		max.pos.z -= zDim;
		resultVertices.push_back(max);

		max.pos.x += xDim;
		resultVertices.push_back(max);


		resultVertices.push_back(min);

		min.pos.x += xDim;
		resultVertices.push_back(min);

		min.pos.z += zDim;
		resultVertices.push_back(min);

		min.pos.x -= xDim;
		resultVertices.push_back(min);




		i = vector<unsigned>{
			0,1, 1,2, 2,3 ,3,0 ,
			4,5, 5,6, 6,7, 7,4,
			2,4, 3,5, 0,6, 1,7 };

		return BoundingBox(resultVertices, i);
	}

	void draw(const Shader& shad) const {
		shad.bind();
		glBindVertexArray(vao);
		glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, 0);
	}


};
