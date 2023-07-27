#include "ParticleRenderer.h"

ParticleRenderer::ParticleRenderer()
{
	this->init();
	this->tailShader = Shader("Shader/particle.vs", "Shader/particle.fs");
	this->explosionShader = Shader("Shader/explosion.vs", "Shader/explosion.fs");
}

void ParticleRenderer::render(std::unordered_map<ParticleTextureHandler, std::vector<Particle*>>& particles, glm::mat4 projection, Camera& camera)
{
	
	glm::mat4 view = camera.GetViewMatrix();

	for (auto it = particles.begin(); it != particles.end(); ++it) {
		ParticleTextureHandler texture = it->first;
		std::vector<Particle*> particleList = it->second;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.getTextureID());

		for (Particle* p : particleList) {

			this->updateModelViewMatrix(p->getPosition(), p->getRotation(), p->getScale(), view);

			if (p->getType() == "TAIL") {
				//Set uniforms
				this->tailShader.use();
				this->tailShader.setMat4("model", this->model);
				this->tailShader.setMat4("projection", projection);
				this->tailShader.setFloat("elapsedTime", p->getElapsedTime());
				this->tailShader.setFloat("lifeLength", p->getLifeLength());
			}
			else {
				this->explosionShader.use();
				this->explosionShader.setMat4("model", this->model);
				this->explosionShader.setMat4("projection", projection);
				this->explosionShader.setFloat("elapsedTime", p->getElapsedTime());
				this->explosionShader.setFloat("lifeLength", p->getLifeLength());
			}

			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			//Maybe depthmask und blend
		}
	}
}

void ParticleRenderer::init()
{
	float qVertices[] = {
		// positions // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	this->quadVertices = new float[6 * 4];

	std::memcpy(this->quadVertices, qVertices, sizeof(qVertices));

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	//position attribute
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, this->quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	this->model = glm::mat4(1.0f);
}

void ParticleRenderer::updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 view)
{
	this->model = glm::mat4(1.0f);
	this->model = glm::translate(this->model, position);
	this->model[0][0] = view[0][0];
	this->model[0][1] = view[1][0];
	this->model[0][2] = view[2][0];
	this->model[1][0] = view[0][1];
	this->model[2][0] = view[0][2];
	this->model[1][1] = view[1][1];
	this->model[1][2] = view[2][1];
	this->model[2][1] = view[1][2];
	this->model[2][2] = view[2][2];

	this->model = glm::rotate(this->model, glm::radians(rotation), glm::vec3(0, 0, 1));
	this->model = glm::scale(this->model, glm::vec3(scale));

	this->model = view * this->model;
}
