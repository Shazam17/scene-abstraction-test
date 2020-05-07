#pragma once
#include <string>

#include "Shader.h"
#include "Model.h"

using namespace std;

class ResourceManager
{
	string shadersPath;
	string modelsPath;
	string texturePath;

public:

	ResourceManager(const string& sPath, const string& mPath) {
		shadersPath = sPath;
		modelsPath = mPath;
	}

	Shader LoadShader(const string& name) {
		return Shader(shadersPath + name);
	}

	Model LoadModel(const string& name) {
		return Model::loadObj(modelsPath + name);
	}

};

