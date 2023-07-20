#include "Missile.h"
#include "Missile.h"
#include <iostream>


Missile::Missile(glm::vec3 startPos, glm::vec3 startDirection, float startVelocity, glm::vec3 color)
{
    this->position = startPos;
    this->startPosition = startPos;
    this->startDirection = startDirection;
    this->direction = glm::normalize(startDirection);
    this->velocity = startVelocity;
    this->color = color;

    // Berechnung der Rotationsachse und des Rotationswinkels
    glm::vec3 standardDirection(0.0f, 1.0f, 0.0f);
    this->rotationAxis = glm::cross(standardDirection, this->direction);
    this->rotationAngle = glm::acos(glm::dot(standardDirection, this->direction));
}

void Missile::update(float deltaTime)
{
    this->position += this->velocity * this->direction * deltaTime;
    glm::vec3 standardDirection(0.0f, 1.0f, 0.0f);
    this->rotationAxis = glm::cross(standardDirection, glm::normalize(this->direction));
    this->rotationAngle = glm::acos(glm::dot(standardDirection, glm::normalize(this->direction)));
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

float Missile::getVelocity()
{
    return this->velocity;
}

glm::vec3 Missile::getColor()
{
    return this->color;
}

glm::vec3 Missile::getDirection()
{
    return this->direction;
}

float Missile::getRotationAngle()
{
    return this->rotationAngle;
}

glm::vec3 Missile::getRotationAxis()
{
    return this->rotationAxis;
}

void Missile::setVelocity(float vel)
{
    this->velocity = vel;
}

void Missile::setDirection(glm::vec3 dir)
{
    this->direction = glm::normalize(dir);
}

void Missile::setPosition(glm::vec3 pos)
{
    this->position = pos;
}
