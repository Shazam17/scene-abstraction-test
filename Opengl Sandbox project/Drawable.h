#pragma once
#include "Shader.h"

class Drawable {

public:
	virtual void draw(const Shader& shad) const = 0;

};

