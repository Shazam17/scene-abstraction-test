#pragma once
#include "Drawable.h"
#include "BoundingBox.h"
#include "ResourceManager.h"
#include "GizmoDrawer.h"




class Mesh : public Drawable {

	Model md;
	Shader bbShader;
	bool selected = false;
public:
	BoundingBox box;

	bool getSelected() {
		return selected;
	}
	
	Mesh() {

	}


	Mesh(const string& path,const Shader& bb) {
		md = Model::loadObj(path);
		box = BoundingBox::loadObj(path);
		bbShader = bb;
	}

	void setSelected(bool value) {
		selected = value;
	}

	void draw(const Shader& shad) const {
		md.draw(shad);
		if (selected) {
			box.draw(bbShader);
		}
	}
};