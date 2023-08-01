#pragma once
#include <glm/glm.hpp>
#include <Camera/camera.h>
#include <Shader/Shader.h>


class RangeRenderer
{
public:
	RangeRenderer(float radius);

	void update(float radius);

	void render(glm::mat4 projection, Camera& camera);

private:
	float radiusShoot;

	unsigned int VAO;
	unsigned int VBO;

	Shader shader;

	std::vector<glm::vec3> vertices;

	void initVertices();
	void initBuffer();
};

