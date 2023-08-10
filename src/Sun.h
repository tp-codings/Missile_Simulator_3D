#pragma once
#include "ModelHandler.h"
#include <Shader/Shader.h>
class Sun
{
public:
	Sun();
	void render(glm::mat4 projection, glm::mat4 view);

private:
	ModelHandler* model;
	Shader shader;
};

