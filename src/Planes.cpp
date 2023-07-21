#include "Planes.h"
#include <iostream>


Planes::Planes(glm::vec3 startPos, glm::vec3 startDirection, float startVelocity, glm::vec3 color)
{
    this->position = startPos;
    this->startPosition = startPos;
    this->startDirection = startDirection;
    this->direction = glm::normalize(startDirection);
    this->velocity = startVelocity;
    this->color = color;

    this->crashRotationSpeed = 1;

    // Berechnung der Rotationsachse und des Rotationswinkels
    glm::vec3 standardDirection(1.0f, 0.0f, 0.0f);
    this->rotationAxis = glm::cross(standardDirection, this->direction);
    this->rotationAngle = glm::acos(glm::dot(standardDirection, this->direction));
}

void Planes::update(float deltaTime)
{
    this->position += this->velocity * this->direction * deltaTime;
    glm::vec3 standardDirection(1.0f, 0.0f, 0.0f);
    this->rotationAxis = glm::cross(standardDirection, glm::normalize(this->direction));
    this->rotationAngle = glm::acos(glm::dot(standardDirection, glm::normalize(this->direction)));


}

glm::vec3 Planes::getPosition()
{
    return this->position;
}

glm::vec3 Planes::getStartPosition()
{
    return this->startPosition;
}

glm::vec3 Planes::getStartDirection()
{
    return this->startDirection;
}


float Planes::getVelocity()
{
    return this->velocity;
}

glm::vec3 Planes::getColor()
{
    return this->color;
}

glm::vec3 Planes::getDirection()
{
    return this->direction;
}

float Planes::getRotationAngle()
{
    return this->rotationAngle;
}

glm::vec3 Planes::getRotationAxis()
{
    return this->rotationAxis;
}

int Planes::getCrahsRotationSpeed()
{
    return this->crashRotationSpeed;
}

void Planes::setVelocity(float vel)
{
    this->velocity = vel;
}

void Planes::setDirection(glm::vec3 dir)
{
    this->direction = glm::normalize(dir);
}

void Planes::setPosition(glm::vec3 pos)
{
    this->position = pos;
}

void Planes::setCrashRotationSpeed(int crashRotationSpeed)
{
    this->crashRotationSpeed = crashRotationSpeed;
}
