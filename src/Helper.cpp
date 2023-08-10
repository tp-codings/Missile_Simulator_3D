#include "Helper.h"

static std::random_device rd;
static std::mt19937 gen(rd());

static float getRandomFloat(float min, float max) {
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

glm::vec3 Helper::getRandomPosition(TerrainGenerator* terrain)
{
    float minX = -terrain->getWidth() * 0.5f;
    float maxX = terrain->getWidth() * 0.5f;
    float minY = -terrain->getHeight() * 0.5f;
    float maxY = terrain->getHeight() * 0.5f;

    glm::vec2 randomPos = glm::vec2(getRandomFloat(minX, maxX), getRandomFloat(minY, maxY));
    float height = terrain->getHeightAtPosition(randomPos.x, randomPos.y);
    return glm::vec3(randomPos.y, height, randomPos.x);
}

glm::vec3 Helper::getRandomPlanePosition(TerrainGenerator* terrain)
{
    float minX = (float)-terrain->getWidth();
    float maxX = (float)terrain->getWidth();
    float minY = (float)-terrain->getHeight();
    float maxY = (float)terrain->getHeight();

    glm::vec2 randomPos = glm::vec2(getRandomFloat(minX, maxX), getRandomFloat(minY, maxY));
    float height = getRandomFloat(1000.0f, 1900.0f);
    return glm::vec3(randomPos.x, height, randomPos.y);
}

glm::vec3 Helper::getRandomDirection()
{
    return glm::vec3(glm::linearRand(-1.0f, 1.0f),
        0.0f,
        glm::linearRand(-1.0f, 1.0f));
}
 
