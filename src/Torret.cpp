#include "Torret.h"
#include "Torret.h"
#include <iostream>


Torret::Torret(glm::vec3 pos, glm::vec3 color)
{
    this->position = pos;
    this->direction = glm::vec3(1.0f, 0.0f, 0.0f);
    this->rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    this->color = color;
    this->shot = false;

    this->missile = new Missile(pos, glm::vec3(0.0, 1.0, 0.001), glm::vec3(0.0));
    // Berechnung der Rotationsachse und des Rotationswinkels
    glm::vec3 standardDirection(1.0f, 0.0f, 0.0f);
    this->rotationAngle = glm::acos(glm::dot(standardDirection, this->direction));
}

void Torret::update(float deltaTime)
{
    glm::vec3 standardDirection(1.0f, 0.0f, 0.0f);
    this->rotationAngle = glm::acos(glm::dot(standardDirection, glm::normalize(this->direction)));
    if (this->missile != nullptr) {
        this->missile->update(deltaTime);
    }

}

glm::vec3 Torret::getPosition()
{
    return this->position;
}


glm::vec3 Torret::getColor()
{
    return this->color;
}

glm::vec3 Torret::getDirection()
{
    return this->direction;
}

bool Torret::getShot()
{
    return this->shot;
}

float Torret::getRotationAngle()
{
    return this->rotationAngle;
}

glm::vec3 Torret::getRotationAxis()
{
    return this->rotationAxis;
}

void Torret::setDirection(glm::vec3 dir)
{
    this->direction = glm::normalize(dir);
}

void Torret::setPosition(glm::vec3 pos)
{
    this->position = pos;
}

void Torret::setShot(bool isShot)
{
    this->shot = isShot;
    this->missile->setShot(isShot);
    this->missile->setVelocity(glm::vec3(120));
}

Missile* Torret::getMissile()
{
    if (this->missile != nullptr) {
        return this->missile;
    }
    else 
        return nullptr;
}

void Torret::deleteMissile()
{
    delete(this->missile);
}
