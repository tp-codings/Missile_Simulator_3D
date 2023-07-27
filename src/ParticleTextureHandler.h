#pragma once
class ParticleTextureHandler
{
public:
	ParticleTextureHandler(int textureID, int numberOfRows);

	int getTextureID();
	int getNumberOfRows();

	bool operator==(const ParticleTextureHandler other) const {
		return (ID == other.ID) && (rows == other.rows);
	}

private:
	const int ID;
	const int rows;
};

