#pragma once
#include <glad/glad.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
public:
	std::string name;
	unsigned int id;
	Shader();
	Shader(const std::string filepath);
	void setUniformVec3(const GLchar* name, const glm::vec3& color) const;
	void setUniform4m(const GLchar* name, const glm::mat4& mat) const;
	void setUniform1f(const GLchar* name, GLfloat var) const;

	void setUniform1i(const GLchar* name, GLint var) const;

	void bind() const;
	void unBind() const;
};
