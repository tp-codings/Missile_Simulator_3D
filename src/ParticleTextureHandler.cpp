#include "ParticleTextureHandler.h"

ParticleTextureHandler::ParticleTextureHandler(int textureID, int numberOfRows) :ID(textureID), rows(numberOfRows)
{

}

int ParticleTextureHandler::getTextureID()
{
	return this->ID;
}

int ParticleTextureHandler::getNumberOfRows()
{
	return this->rows;
}
