#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader/Shader.h>
#include <vector>
#include "Particle.h"
#include <Camera/camera.h>
#include "ParticleTextureHandler.h"
#include "Loader.h"

class ParticleRenderer
{
public:
	ParticleRenderer();

	void render(std::unordered_map<int, std::vector<Particle*>>& particles, glm::mat4 projection, Camera& camera);


private:
	Loader loader;

	float* quadVertices;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int instanceVBO;
	unsigned int texOffsetVBO;
	unsigned int blendVBO;

	Shader shader;

	glm::mat4 model;

	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::vec4> texOffsets;
	std::vector<float> blendFactors;

	void init();
	void updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 view);
	void prepareRendering();
	void finishRendering();
};

