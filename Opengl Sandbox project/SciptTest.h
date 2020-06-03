#pragma once
#include "Component.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Mesh.h"

class MyScript : public Component {



public:
	void onStart() override {


	}

	void onUpdate() override {
		auto& scale = GetComponent<Transform>()->getScale();
		
		static float scaleFactor = 1.0001f;
		if (scale.x > 4) {
			scaleFactor = 0.009;
			scale *= scaleFactor;
		}else
		if (scale.x < 0.5) {
			scaleFactor = 1.001f;
			scale *= scaleFactor;
		}
		else {
			scale *= scaleFactor;
		}

		GetComponent<Transform>()->recalculateMat();
	}
};

class MeshRender : public Component {
private:
	Mesh model;
	std::string modelName;
	Shader shad;
public:
	MeshRender(const std::string& str) {
		modelName = str;
	}

	void onStart() override {
		model = ResourceManager::GetInstance()->LoadMesh(modelName);
		shad = ResourceManager::GetInstance()->GetCachedShader("model.shader");
	}

	void onUpdate() override {
		model.box.setModelMat(GetComponent<Transform>()->getModelMatrix());
		Renderer::DrawModelWithBoundingBox(&model, shad, *GetComponent<Transform>());
		if (model.getSelected()) {
			//GizmoDrawer::GetInstance()->drawGizmo(gizmoShader, *obj.GetComponent<Transform>("tranform"));
		}
	}

	Mesh& getMesh() {
		return model;
	}

	BoundingBox& getBoundingBox() {
		return model.box;
	}

};
