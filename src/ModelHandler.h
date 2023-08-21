#pragma once
#include <ModelLoader/model.h>
#include "Settings.h"

//Handler for easy transform and render Models but no instanced rendering
class ModelHandler
{
public: 
	ModelHandler(string path){
	this->model = new Model(path);
	this->transformation = glm::mat4(1.0f);
	this->translation = glm::vec3(1.0f);
	this->rotationAxis = glm::vec3(1.0f);
	this->rotationAngle = 0.0f;
	this->scale = 0.0f;
	};

	void Draw(Shader *s, glm::mat4 projection, glm::mat4 view, glm::vec3 color = glm::vec3(1.0f)){
		this->Transform();
		s->use();
		s->setMat4("projection", projection);
		s->setMat4("view", view);
		s->setMat4("model", this->transformation);
		s->setVec3("vColor", color);
		s->setVec3("pointLightPos", Settings::pointLightPos);
		s->setVec3("pointLightColor", glm::vec3(Settings::pointLightColor.x, Settings::pointLightColor.y, Settings::pointLightColor.z));
		s->setVec3("dirLightColor", glm::vec3(Settings::dirLightColor.x, Settings::dirLightColor.y, Settings::dirLightColor.z));
		s->setVec3("dirLightDir", Settings::dirLightDir);
		s->setFloat("shininess", Settings::shininess);
		s->setFloat("ambientStrength", Settings::ambientStrength);

		this->model->Draw(*s);
	};

	void Translate(glm::vec3 direction){
		this->translation = direction;
	};

	void Rotate(float angle, glm::vec3 axis){
		this->rotationAngle = angle;
		this->rotationAxis = axis;
	};
	void Scale(float factor){
		this->scale = factor;
	};

	void setModelMatrix(glm::mat4 model){
		this->transformation = model;
	}

private:

	Model* model;

	glm::mat4 transformation;

	glm::vec3 translation;
	glm::vec3 rotationAxis;

	float rotationAngle;
	float scale;

	void Transform(){
		this->transformation = glm::mat4(1.0f);
		this->transformation = glm::translate(this->transformation, this->translation);
		this->transformation = glm::rotate(this->transformation, this->rotationAngle, this->rotationAxis);
		this->transformation = glm::scale(transformation, glm::vec3(scale));
	};
};

