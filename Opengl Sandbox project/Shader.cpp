#include "Shader.h"

Shader::Shader() {

}

Shader::Shader(const std::string filepath) {
	std::ifstream file(filepath);

	if (!file) {
		std::cout << "File isnt opened" << std::endl;
	}
	int mode = -1;
	std::string line;
	std::stringstream ss[3];
	bool gShader = false;
	while (!file.eof()) {
		getline(file, line);
		if (line.find("shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				mode = 0;
			}
			if (line.find("fragment") != std::string::npos) {
				mode = 1;
			}
			if (line.find("geometry") != std::string::npos) {
				mode = 2;
				gShader = true;
			}
		}
		else {
			ss[mode] << line << '\n';
		}
		//std::cout << line << std::endl;
	}
	std::string strCode[3];
	strCode[0] = ss[0].str();
	strCode[1] = ss[1].str();
	if (gShader) {
		strCode[2] = ss[2].str();
	}

	const GLchar* shaderCode[3];
	shaderCode[0] = strCode[0].c_str();
	shaderCode[1] = strCode[1].c_str();;


	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &shaderCode[0], NULL);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, &shaderCode[1], NULL);
	glCompileShader(fragmentShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	id = glCreateProgram();

	if (gShader) {
		shaderCode[2] = strCode[2].c_str();;
		unsigned int geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &shaderCode[2], NULL);
		glCompileShader(geometry);


		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		};
		glAttachShader(id, geometry);
		glDeleteShader(geometry);
	}

	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::setUniformVec3(const GLchar* name, glm::vec3& color) const {
	bind();
	int loc = glGetUniformLocation(id, name);
	if (loc == -1) {
		std::cout << "uniform didnt find" << std::endl;
	}
	glUniform3fv(loc, 1, &color[0]);
	unBind();
}

void Shader::setUniform4m(const GLchar* name, const glm::mat4& mat) const {
	bind();
	int loc = glGetUniformLocation(id, name);
	if (loc == -1) {
		std::cout << "uniform didnt find" << std::endl;;
	}
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
	unBind();
}

void Shader::setUniform1f(const GLchar* name, GLfloat var) const {
	bind();
	//std::cout << "setting " << name << " uniform" << std::endl;
	int loc = glGetUniformLocation(id, name);
	if (loc == -1) {
		std::cout << "uniform didnt find" << std::endl;
	}
	glUniform1f(loc, var);
	unBind();
}

void Shader::setUniform1i(const GLchar* name, GLint var) const {
	bind();
	unsigned int loc = glGetUniformLocation(id, name);
	if (loc == -1) {
		std::cout << "uniform didnt find" << std::endl;
	}
	glUniform1i(loc, var);
	unBind();
}

void Shader::bind() const {
	glUseProgram(id);
}

void Shader::unBind() const {
	glUseProgram(0);
}
