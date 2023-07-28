#include "ParticleRenderer.h"
#include "ParticleMaster.h"

#define MAX_INSTANCES 100000
#define INSTANCE_DATA_LENGTH 21

ParticleRenderer::ParticleRenderer()
{
	this->init();
	this->shader = Shader("Shader/particle.vs", "Shader/particle.fs");
}

void ParticleRenderer::render(std::unordered_map<int, std::vector<Particle*>>& particles, glm::mat4 projection, Camera& camera)
{
	this->prepareRendering();

	glm::mat4 view = camera.GetViewMatrix();

	for (auto it = particles.begin(); it != particles.end(); it++) {

		int texture = it->first;
		std::vector<Particle*> particleList = it->second;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);


		for (Particle* p : particleList) {

			this->updateModelViewMatrix(p->getPosition(), p->getRotation(), p->getScale(), view);
			this->texOffsets.push_back(glm::vec4(p->getTexOffset1(), p->getTexOffset2()));
			this->blendFactors.push_back(p->getBlendFactor());

			this->shader.use();

			this->shader.setMat4("projection", projection);

			this->shader.setFloat("numberOfRows", p->getTexture().getNumberOfRows());
		}

		glBindBuffer(GL_ARRAY_BUFFER, this->texOffsetVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleList.size() * sizeof(glm::vec4), &this->texOffsets[0]);		
		
		glBindBuffer(GL_ARRAY_BUFFER, this->blendVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleList.size() * sizeof(float), &this->blendFactors[0]);

		glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particleList.size() * sizeof(glm::mat4), &this->modelMatrices[0]);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particleList.size());

		this->modelMatrices.clear();
		this->texOffsets.clear();
		this->blendFactors.clear();
	}


	this->finishRendering();

}

void ParticleRenderer::init()
{
	float qVertices[] = {
		// positions // texCoords
		-0.5f,  0.5f,
		-0.5f, -0.5f,
		 0.5f, -0.5f,

		-0.5f,  0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f
	};
	this->quadVertices = new float[6 * 2];

	std::memcpy(this->quadVertices, qVertices, sizeof(qVertices));

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	//position attribute
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, this->quadVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	//Modelmatrices
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(glm::mat4), 0, GL_DYNAMIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);

	glBindVertexArray(0);


	//texOffsets
	glGenBuffers(1, &this->texOffsetVBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->texOffsetVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(glm::vec4), 0, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glVertexAttribDivisor(5, 1);

	glBindVertexArray(0);

	//texOffsets
	glGenBuffers(1, &this->blendVBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->blendVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(float), 0, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	//this->instanceVBO = loader.createEmptyVBO(INSTANCE_DATA_LENGTH * MAX_INSTANCES);

	//loader.addInstancedAttribute(this->VAO, this->instanceVBO, 1, 4, INSTANCE_DATA_LENGTH, 0);
	//loader.addInstancedAttribute(this->VAO, this->instanceVBO, 2, 4, INSTANCE_DATA_LENGTH, 4);
	//loader.addInstancedAttribute(this->VAO, this->instanceVBO, 3, 4, INSTANCE_DATA_LENGTH, 8);
	//loader.addInstancedAttribute(this->VAO, this->instanceVBO, 4, 4, INSTANCE_DATA_LENGTH, 12);
	//loader.addInstancedAttribute(this->VAO, this->instanceVBO, 5, 4, INSTANCE_DATA_LENGTH, 16);
	//loader.addInstancedAttribute(this->VAO, this->instanceVBO, 6, 4, INSTANCE_DATA_LENGTH, 20);


	glBindBuffer(GL_ARRAY_BUFFER,0);
	

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
	this->modelMatrices.push_back(this->model);
}

void ParticleRenderer::prepareRendering()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);
	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
}

void ParticleRenderer::finishRendering()
{
	//glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthMask(true);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glBindVertexArray(0);
}
