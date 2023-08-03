#pragma once
#include <glad/glad.h>

#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader/Shader.h>
#include <Camera/Camera.h>

#include <iostream>
#include <vector>

#include "FastNoiseLite.h"

class TerrainGenerator {
public:
	TerrainGenerator(const char* path) {
		this->setUpHeightMap(path);
		this->generateHeightMap();
		this->initBuffer();

		this->shader = Shader("Shader/terrain.vs", "Shader/terrain.fs");
	}
	TerrainGenerator(float ampltudeMultiplier, float width, float height) {
		this->generatePerlinNoise(ampltudeMultiplier, width, height);
		this->initBuffer();

		this->shader = Shader("Shader/terrain.vs", "Shader/terrain.fs");
	}

	void render(float deltaTime, glm::mat4 projection, glm::mat4 view) {
		this->shader.use();
		this->shader.setMat4("projection", projection);
		this->shader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		this->shader.setMat4("model", model);

		glBindVertexArray(terrainVAO);
		//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (unsigned strip = 0; strip < numStrips; strip++)
		{
			glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
				numTrisPerStrip + 2,   // number of indices to render
				GL_UNSIGNED_INT,     // index data type
				(void*)(sizeof(unsigned) * (numTrisPerStrip + 2) * strip)); // offset to starting index
		}
	}

	float getHeightAtPosition(float x, float z) {

		int xIndex = static_cast<int>((x + width / 2.0f) / (width / (float)(width - 1)));
		int zIndex = static_cast<int>((z + height / 2.0f) / (height / (float)(height - 1)));

		if (xIndex < 0 || xIndex >= width || zIndex < 0 || zIndex >= height) {
			return 0.0f; 
		}

		int vertexIndex = xIndex + width * zIndex;

		float y = vertices[vertexIndex * 3 + 1]; 

		return y;
	}

	int getWidth() {
		return this->width;
	}

	int getHeight() { return this->height; }

private:
	Shader shader;
	int width, height, nrChannels;
	std::vector<float> vertices;
	std::vector<unsigned> indices;
	unsigned char* data;
	int numStrips;
	int numTrisPerStrip;

	unsigned int terrainVAO, terrainVBO, terrainIBO;




	void setUpHeightMap(const char* path) {
		//stbi_set_flip_vertically_on_load(true);
		this->data = stbi_load(path, &this->width, &this->height, &this->nrChannels, 0);
		if (data)
		{
			std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
	}

	void generateHeightMap() {
		float yScale = 64.0f / 256.0f, yShift = 16.0f;
		int rez = 1;
		unsigned bytePerPixel = nrChannels;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
				unsigned char y = pixelOffset[0];

				// vertex
				vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
				vertices.push_back((int)y * yScale - yShift);   // vy
				vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
			}
		}
		std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
		stbi_image_free(data);

		for (unsigned i = 0; i < height - 1; i += rez)
		{
			for (unsigned j = 0; j < width; j += rez)
			{
				for (unsigned k = 0; k < 2; k++)
				{
					this->indices.push_back(j + width * (i + k * rez));
				}
			}
		}
		std::cout << "Loaded " << indices.size() << " indices" << std::endl;
		numStrips = (height - 1) / rez;
		numTrisPerStrip = (width / rez) * 2 - 2;
		std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
		std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;
	}

	void generatePerlinNoise(float amplitudeMultiplier, float width, float height) {
		float yScale = 64.0f / 256.0f, yShift = 16.0f;
		int rez = 1;
		unsigned bytePerPixel = 4;

		// Erstellen und konfigurieren des Perlin Noise Generators
		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetFrequency(0.02f); // Ändere die Frequenz, um die Detailstufe des Geländes zu ändern
		noise.SetFractalOctaves(4); // Ändere die Anzahl der Oktaven für mehr oder weniger Details

		// Generiere Höhenwerte für das Terrain anhand von Perlin Noise
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				float x = -height / 2.0f + height * i / (float)height;
				float z = -width / 2.0f + width * j / (float)width;

				// Berechne die Höhe anhand von Perlin Noise und multipliziere sie mit der Amplitude
				float y = noise.GetNoise(x, z) * amplitudeMultiplier;

				// vertex
				vertices.push_back(x);
				vertices.push_back(y * yScale - yShift);
				vertices.push_back(z);
			}
		}

		std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;

		// Indizes für die Dreiecksstreifen erzeugen
		for (unsigned i = 0; i < height - 1; i += rez)
		{
			for (unsigned j = 0; j < width; j += rez)
			{
				for (unsigned k = 0; k < 2; k++)
				{
					this->indices.push_back(j + width * (i + k * rez));
				}
			}
		}

		std::cout << "Loaded " << indices.size() << " indices" << std::endl;
		numStrips = (height - 1) / rez;
		numTrisPerStrip = (width / rez) * 2 - 2;
		std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
		std::cout << "Created " << numStrips * numTrisPerStrip << " triangles total" << std::endl;
	}

	void initBuffer() {
		glGenVertexArrays(1, &terrainVAO);
		glBindVertexArray(terrainVAO);

		glGenBuffers(1, &terrainVBO);
		glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &terrainIBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

	}
};