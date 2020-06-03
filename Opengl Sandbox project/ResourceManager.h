#pragma once
#include <string>
#include <map>

#include "Shader.h"
#include "Model.h"
#include "Mesh.h"

using namespace std;

class ResourceManager
{
	string shadersPath;
	string modelsPath;
	string texturePath;

	map<string,Shader> cachedShaders;

public:

	ResourceManager(const string& sPath, const string& mPath) {
		shadersPath = sPath;
		modelsPath = mPath;
	}

	static ResourceManager* GetInstance() {
		static ResourceManager* manager = nullptr;
		if (manager == nullptr) {
			manager = new ResourceManager("assets/shaders/","assets/models/");
		}
		return manager;
	}


	Shader& LoadShader(const string& name) {
		cachedShaders.insert({ name, Shader(shadersPath + name) });
		return cachedShaders[name];
	}

	Shader& GetCachedShader(const string& name) {
		return cachedShaders[name];
	}

	Model LoadModel(const string& name) {
		return Model::loadObj(modelsPath + name);
	}

	Mesh LoadMesh(const string& name){
		Shader bb = GetCachedShader("whiteBasicShader.shader");
		return Mesh(modelsPath + name,bb);
	}

	
	void terminate() {
		auto* instance = GetInstance();
		delete instance;
	}

};

