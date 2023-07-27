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

class ParticleRenderer
{
public:
	ParticleRenderer();
	void render(std::unordered_map<ParticleTextureHandler, std::vector<Particle*>>& particles, glm::mat4 projection, Camera& camera);

private:
	float* quadVertices;

	unsigned int VAO;
	unsigned int VBO;

	Shader tailShader;
	Shader explosionShader;


	glm::mat4 model;

	void init();
	void updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 view);
};

