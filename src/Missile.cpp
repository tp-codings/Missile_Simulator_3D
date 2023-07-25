#include "Missile.h"
#include "Missile.h"
#include <iostream>


Missile::Missile(glm::vec3 startPos, glm::vec3 startDirection, glm::vec3 startVelocity, glm::vec3 color)
{
    this->position = startPos;
    this->startPosition = startPos;
    this->startDirection = startDirection;
    this->direction = glm::normalize(startDirection);
    this->maxVelocity = startVelocity;
    this->velocity = glm::vec3(0.0f);
    this->color = color;
    this->timer = 0.0f;
    this->shot = false;

    // Berechnung der Rotationsachse und des Rotationswinkels
    glm::vec3 standardDirection(0.0f, 1.0f, 0.0f);
    this->rotationAxis = glm::cross(standardDirection, this->direction);
    this->rotationAngle = glm::acos(glm::dot(standardDirection, this->direction));
}

void Missile::update(float deltaTime)
{
    if (this->calcVecLength(this->velocity) < this->calcVecLength(this->maxVelocity)) {
        this->velocity += this->acceleration * deltaTime;
    }
    this->position += this->velocity * this->direction * deltaTime;
    glm::vec3 standardDirection(0.0f, 1.0f, 0.0f);
    this->rotationAxis = glm::cross(standardDirection, glm::normalize(this->direction));
    this->rotationAngle = glm::acos(glm::dot(standardDirection, glm::normalize(this->direction)));
    if (this->shot) {
        this->timer += deltaTime;
    }
}

glm::vec3 Missile::getPosition()
{
    return this->position;
}

glm::vec3 Missile::getStartPosition()
{
    return this->startPosition;
}

glm::vec3 Missile::getStartDirection()
{
    return this->startDirection;
}

glm::vec3 Missile::getTrajectory()
{
    return this->direction;
}

glm::vec3 Missile::getVelocity()
{
    return this->velocity;
}

float Missile::getTimer()
{
    return this->timer;
}

glm::vec3 Missile::getColor()
{
    return this->color;
}

glm::vec3 Missile::getDirection()
{
    return this->direction;
}

glm::vec3 Missile::getAcceleration()
{
    return this->acceleration;
}

glm::vec3 Missile::getMaxVelocity()
{
    return this->maxVelocity;
}

float Missile::getRotationAngle()
{
    return this->rotationAngle;
}

glm::vec3 Missile::getRotationAxis()
{
    return this->rotationAxis;
}

void Missile::setVelocity(glm::vec3 vel)
{
    this->velocity = vel;
}

void Missile::setMaxVelocity(glm::vec3 maxVel)
{
    this->maxVelocity = maxVel;
}

void Missile::setDirection(glm::vec3 dir)
{
    this->direction = glm::normalize(dir);
}

void Missile::setPosition(glm::vec3 pos)
{
    this->position = pos;
}

void Missile::setAcceleration(glm::vec3 acc)
{
    this->acceleration = acc;
}

void Missile::setShot(bool shot)
{
    this->shot = shot;
}

float Missile::calcVecLength(glm::vec3 vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z*vec.z);
}
