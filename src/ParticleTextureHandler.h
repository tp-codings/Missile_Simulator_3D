#pragma once
class ParticleTextureHandler
{
public:
	ParticleTextureHandler(int textureID, int numberOfRows);

	int getTextureID();
	int getNumberOfRows();

private:
	int ID;
	int rows;
};

