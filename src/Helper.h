#pragma once
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include "Loader.h"
#include "TerrrainGenerator.h"
#include <random>




class Helper {
public:
	Helper() {
	};

	static glm::vec3 getRandomPosition(TerrainGenerator* terrain);

	static glm::vec3 getRandomPlanePosition(TerrainGenerator* terrain);

	static glm::vec3 getRandomDirection();

private:
	Loader* loader;
};

namespace Colors {
	const glm::vec3 RED = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 GREEN = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 BLUE = glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 WHITE = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 BLACK = glm::vec3(0.0f, 0.0f, 0.0f);

	// Custom Colors
	const glm::vec3 ORANGE = glm::vec3(1.0f, 0.5f, 0.0f);
	const glm::vec3 PURPLE = glm::vec3(0.5f, 0.0f, 0.5f);
	const glm::vec3 YELLOW = glm::vec3(1.0f, 1.0f, 0.0f);
	const glm::vec3 CYAN = glm::vec3(0.0f, 1.0f, 1.0f);
	const glm::vec3 MAGENTA = glm::vec3(1.0f, 0.0f, 1.0f);

	// Pastell Colors
	const glm::vec3 PASTEL_PINK = glm::vec3(0.86f, 0.63f, 0.69f);
	const glm::vec3 PASTEL_YELLOW = glm::vec3(0.95f, 0.95f, 0.58f);
	const glm::vec3 PASTEL_BLUE = glm::vec3(0.62f, 0.77f, 0.87f);
	const glm::vec3 PASTEL_GREEN = glm::vec3(0.64f, 0.87f, 0.68f);
	const glm::vec3 PASTEL_PURPLE = glm::vec3(0.73f, 0.62f, 0.81f);

	// Metallic Colors
	const glm::vec3 GOLD = glm::vec3(1.0f, 0.84f, 0.0f);
	const glm::vec3 SILVER = glm::vec3(0.75f, 0.75f, 0.75f);
	const glm::vec3 BRONZE = glm::vec3(0.8f, 0.5f, 0.2f);
	const glm::vec3 COPPER = glm::vec3(0.85f, 0.55f, 0.4f);
	const glm::vec3 STEEL = glm::vec3(0.6f, 0.6f, 0.67f);

	// Rainbow Colors
	const glm::vec3 VIOLET = glm::vec3(0.5f, 0.0f, 1.0f);
	const glm::vec3 INDIGO = glm::vec3(0.29f, 0.0f, 0.51f);
	const glm::vec3 BLUE_GREEN = glm::vec3(0.0f, 0.5f, 0.5f);
	const glm::vec3 YELLOW_GREEN = glm::vec3(0.6f, 0.8f, 0.2f);
	const glm::vec3 YELLOW_ORANGE = glm::vec3(1.0f, 0.71f, 0.0f);
	const glm::vec3 RED_ORANGE = glm::vec3(1.0f, 0.27f, 0.0f);

	// Earth Tones
	const glm::vec3 BROWN = glm::vec3(0.65f, 0.16f, 0.16f);
	const glm::vec3 SAND = glm::vec3(0.76f, 0.7f, 0.5f);
	const glm::vec3 OLIVE = glm::vec3(0.5f, 0.5f, 0.0f);
	const glm::vec3 MOSS_GREEN = glm::vec3(0.55f, 0.64f, 0.45f);
	const glm::vec3 SLATE_GRAY = glm::vec3(0.44f, 0.5f, 0.56f);

	// Neon Colors
	const glm::vec3 NEON_PINK = glm::vec3(1.0f, 0.07f, 0.55f);
	const glm::vec3 NEON_YELLOW = glm::vec3(1.0f, 0.95f, 0.0f);
	const glm::vec3 NEON_GREEN = glm::vec3(0.29f, 0.95f, 0.29f);
	const glm::vec3 NEON_BLUE = glm::vec3(0.29f, 0.59f, 0.95f);
	const glm::vec3 NEON_PURPLE = glm::vec3(0.67f, 0.29f, 0.95f);
};