#include "Application.h"
#include "Model.h"
#include "OrbitCamera.h"
#include "Renderer.h"
#include "ResourceManager.h"


class GameObject {
public:
	int id;
	Model model;
	Shader shader;
	Transform transform;

	string name;
	GameObject(const string& n, const Model& m,const Shader& s, const Transform& t): name(n) {
		model = m;
		shader = s;
		transform = t;
	}


	string toString() const {
		stringstream ss;

		ss << "GAMEOBJECT" << endl;
		ss << "MESH: " << model.name << endl;
		ss << "SHADER: " << shader.name << endl;
		ss << "POSITION: " << transform.pos.x << ' ' << transform.pos.y << ' ' << transform.pos.z << endl;
		ss << "SCALE: " << transform.scaleVec.x << ' ' << transform.scaleVec.y << ' ' << transform.scaleVec.z << endl;


		return ss.str();
	}



};

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

	Scene(const vector<GameObject> & objs, const string& existSceneName) {
		name = existSceneName;
		objects = objs;

	}
	void addGameObject(const GameObject& gm) {
		objects.push_back(gm);
	}

	void draw(mat4 view,mat4 proj) {
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
					Transform(posVec.pos, scaleVec.pos,vec3(0)
					)));
			}
		}
		

		return Scene(objs, name);
	}




};

int main(void)
{
	Application app(1280, 720, "model interaction");
	OrbitCamera camera;
	ResourceManager resourceManager("assets/shaders/", "assets/models/");

	Model model = resourceManager.LoadModel("cyl1.obj");
	//Shader shader = resourceManager.LoadShader("model.shader");

	//shader.setUniform4m("u_proj", camera.getProjectionMat(app));

	Transform transform(vec3(0), vec3(0.8), vec3(0));


	/*model.name = "cyl1.obj";
	shader.name = "model.shader";

	Scene testScene1("test1");
	GameObject cylinder("cylinder",model,shader,transform);
	testScene1.addGameObject(cylinder);

	Scene::SaveToFile(testScene1);*/

	Scene temp;// = Scene::LoadFromFile("test1");
	bool toRender = false;

	string sceneName = "";
	while (app.isRunning()) {
		app.clear();
		camera.processMouseMove(app);
		camera.processMouseKeys(app);

		//shader.setUniform4m("u_view", camera.getViewMat());
		//Renderer::DrawModelTransformed(model, shader, transform);
		ImGui::InputText("sceneName", &sceneName[0], 100);
		if (ImGui::Button("LoadScene")) {
			toRender = true;
			temp = Scene::LoadFromFile("test1");
		}


		if (toRender) {
	
			temp.draw(camera.getViewMat(), camera.getProjectionMat(app));
		}
		


		app.swapBuffers();
	}

	return 0;
}