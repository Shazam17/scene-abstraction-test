#pragma once



#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "glm/gtc/matrix_transform.hpp"
#include "GizmoDrawer.h"
#include "Transform.h"




class Renderer {

public:
	
	
	static void submitDraw(Drawable* model, const Shader& shader) {
		mat4 ident(1.0f);
		shader.setUniform4m("u_model", ident);
		model->draw(shader);

	}

	static void DrawModelTransformed( Drawable* model, const Shader& shader , const Transform& transform) {

		mat4 modMat = transform.getModelMatrix();
		shader.setUniform4m("u_model", modMat);
		model->draw(shader);
	}

	static void DrawModelWithBoundingBox(Drawable* model, const Shader& shader, const Transform& transform) {

		mat4 modMat = transform.getModelMatrix();
		shader.setUniform4m("u_model", modMat);
		Shader bbShader = ResourceManager::GetInstance()->GetCachedShader("whiteBasicShader.shader");
		bbShader.setUniform4m("u_model", modMat);
		model->draw(shader);
	}
};

