#pragma once



#include <GLFW/glfw3.h>
#include <glad/glad.h>


#include "Model.h"
#include "Transform.h"

class Renderer {


public:
	static void submitDraw() {

	}

	static void DrawModelTransformed(const Model& model, const Shader& shader , const Transform& transform) {

		mat4 modMat = transform.getModelMatrix();
		shader.setUniform4m("u_model", modMat);
		model.draw(shader);
	}

};

