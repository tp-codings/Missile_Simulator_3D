#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Missile.h"

class GunTower
{
public:
	GunTower(glm::vec3 pos, float speed, glm::vec3 color = glm::vec3(0.2, 0.2, 0.2));

	void update(float deltaTime);

	glm::vec3 getPosition();
	glm::vec3 getColor();
	glm::vec3 getDirection();
	bool getShot();

	float getRotationAngle();
	float getSpeed();
	glm::vec3 getRotationAxis();

	void setDirection(glm::vec3 dir);
	void setPosition(glm::vec3 pos);
	void setShot(bool isShot);

	Missile* getTower();

private:
	glm::vec3 direction;
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 rotationAxis;

	float rotationAngle;
	float speed;

	bool shot;

	Missile* barrel;

};


