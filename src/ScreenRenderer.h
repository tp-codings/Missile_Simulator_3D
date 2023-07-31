#include <Shader/Shader.h>

#pragma once
class ScreenRenderer
{
public: 
	ScreenRenderer();

	void render(unsigned int texColorBuffer);

private:
	void initBuffer();

	unsigned int VAO;
	unsigned int VBO;

	Shader shader;
};

