#pragma once

#include "GameObject.h"

class Scene {

	int id;
	string name;
	vector<GameObject> objects;


public:
	Scene() {

	}
	Scene(const string& creationName) : name(creationName) {
		ofstream sceneFile(name);
	}

	Scene(const vector<GameObject>& objs, const string& existSceneName) {
		name = existSceneName;
		objects = objs;

	}
	void addGameObject(const GameObject& gm) {
		objects.push_back(gm);
	}

	void draw(mat4 view, mat4 proj) {
		for (const auto& object : objects) {

			object.shader.setUniform4m("u_view", view);
			object.shader.setUniform4m("u_proj", proj);
			Renderer::DrawModelTransformed(object.model, object.shader, object.transform);
		}
	}

	static void SaveToFile(const Scene& scene) {
		ofstream sceneFile(scene.name);

		sceneFile << "SCENE" << endl;
		sceneFile << "NAME: " << scene.name << endl;

		sceneFile << "OBJECTS" << endl;
		sceneFile << "COUNT:" << scene.objects.size() << endl;
		for (const auto& object : scene.objects) {
			sceneFile << object.toString() << endl;
		}


	}

	static Scene LoadFromFile(const string& fileName) {
		ifstream sceneFile(fileName);
		ResourceManager resourceManager("assets/shaders/", "assets/models/");

		vector<GameObject> objs;

		string buff;
		string name;
		getline(sceneFile, buff);
		if (buff.compare("SCENE") == 0) {
			cout << "SCENE LOADING OK" << endl;
		}
		getline(sceneFile, buff);

		stringstream extractName(buff);
		extractName >> buff;
		extractName >> name;

		cout << "SCENE NAME: " << name << endl;

		while (!sceneFile.eof()) {
			getline(sceneFile, buff);
			if (buff.compare("GAMEOBJECT") == 0) {
				getline(sceneFile, buff);
				stringstream extractObject(buff);
				string meshName;
				extractObject >> buff;
				extractObject >> meshName;

				getline(sceneFile, buff);
				extractObject = stringstream(buff);
				string shaderName;
				extractObject >> buff;
				extractObject >> shaderName;

				getline(sceneFile, buff);
				extractObject = stringstream(buff);
				extractObject >> buff;
				float arr[3];
				extractObject >> arr[0];
				extractObject >> arr[1];
				extractObject >> arr[2];

				Vertex posVec(arr);

				getline(sceneFile, buff);
				extractObject = stringstream(buff);
				extractObject >> buff;

				extractObject >> arr[0];
				extractObject >> arr[1];
				extractObject >> arr[2];

				Vertex scaleVec(arr);

				objs.push_back(GameObject("temp",
					resourceManager.LoadModel(meshName),
					resourceManager.LoadShader(shaderName),
					Transform(posVec.pos, scaleVec.pos, vec3(0)
					)));
			}
		}


		return Scene(objs, name);
	}




};
