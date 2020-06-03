#pragma once
#include "Drawable.h"
#include "ResourceManager.h"
#include "Transform.h"

class GizmoDrawer : public Drawable {

	unsigned vao;
	unsigned vbo;
	unsigned ib;

	GizmoDrawer() {

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glGenBuffers(1, &ib);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);

		

		float verts[][3] = {
	{-0.000000, 0.012320, 0.000000},  {-0.000000, 0.012320, 0.974306},
	{0.008711, 0.008711, 0.000000},   {0.008711, 0.008711, 0.974306},
	{0.012320, -0.000000, 0.000000},  {0.012320, -0.000000, 0.974306},
	{0.008711, -0.008711, 0.000000},  {0.008711, -0.008711, 0.974306},
	{-0.000000, -0.012320, 0.000000}, {-0.000000, -0.012320, 0.974306},
	{-0.008711, -0.008711, 0.000000}, {-0.008711, -0.008711, 0.974306},
	{-0.012320, 0.000000, 0.000000},  {-0.012320, 0.000000, 0.974306},
	{-0.008711, 0.008711, 0.000000},  {-0.008711, 0.008711, 0.974306},
	{0.000000, 0.072555, 0.974306},   {0.051304, 0.051304, 0.974306},
	{0.072555, -0.000000, 0.974306},  {0.051304, -0.051304, 0.974306},
	{-0.000000, -0.072555, 0.974306}, {-0.051304, -0.051304, 0.974306},
	{-0.072555, 0.000000, 0.974306},  {-0.051304, 0.051304, 0.974306},
	{0.000000, -0.000000, 1.268098},
		};
		unsigned indices[] = {
1,  3,  2,  3,  5,  4,  5,  7,  6,  7,  9,  8,  9,  11, 10, 11, 13, 12, 5,  18, 19, 15, 1,
0,  13, 15, 14, 6,  10, 14, 11, 21, 22, 7,  19, 20, 13, 22, 23, 3,  17, 18, 9,  20, 21, 15,
23, 16, 1,  16, 17, 23, 22, 24, 21, 20, 24, 19, 18, 24, 17, 16, 24, 16, 23, 24, 22, 21, 24,
20, 19, 24, 18, 17, 24, 0,  1,  2,  2,  3,  4,  4,  5,  6,  6,  7,  8,  8,  9,  10, 10, 11,
12, 7,  5,  19, 14, 15, 0,  12, 13, 14, 14, 0,  2,  2,  4,  6,  6,  8,  10, 10, 12, 14, 14,
2,  6,  13, 11, 22, 9,  7,  20, 15, 13, 23, 5,  3,  18, 11, 9,  21, 1,  15, 16, 3,  1,  17,
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	}
public:
	
	

	static GizmoDrawer* GetInstance() {
		static GizmoDrawer* drawer;
		if (drawer == nullptr) {
			drawer = new GizmoDrawer();
			
		}
		return drawer;
	}

	void drawGizmo(const Shader& shad,  const Transform &tranform) {

		Transform tr = tranform;
		tr.addRotate(vec3(0, 0, 0));
		//std::cout << tr.getRot().z << std::endl;
		drawTranformed(shad,tr.getModelMatrix(), vec3(1, 0, 0));
		tr.addRotate(vec3(0, 90, 0));
		drawTranformed(shad,tr.getModelMatrix(), vec3(0, 1, 0));
		tr.addRotate(vec3(-90, 0, 0));
		drawTranformed(shad,tr.getModelMatrix(), vec3(0, 0, 1));
	}

	void drawTranformed(const Shader& gizmoShader, const mat4& tr, const vec3& color) const {
		glBindVertexArray(vao);
		glDepthFunc(GL_ALWAYS);
		gizmoShader.setUniformVec3("color", color);
		gizmoShader.bind();
		gizmoShader.setUniform4m("u_model", tr);
		gizmoShader.bind();
		glDrawElements(GL_TRIANGLES, 138, GL_UNSIGNED_INT, 0);
		glDepthFunc(GL_LEQUAL);

	}

	void draw(const Shader& shad) const {
		glBindVertexArray(vao);

		shad.setUniformVec3("color", vec3(1, 0, 0));
		shad.bind();

		glDrawElements(GL_TRIANGLES, 138, GL_UNSIGNED_INT, 0);
	}

};