#pragma once
#include <vector>
#include <string>
#include <Shader/Shader.h>
#include <Camera/camera.h>
#include <stb_image/stb_image.h>



class Skybox
{
public: 
	Skybox(){
		this->shader = Shader("Shader/cubemap.vs", "Shader/cubemap.fs");
		this->initVertices();
		this->initBuffer();
		this->initDefaultSkyboxes();
		this->cubeMapTexture = this->loadCubeMap(this->skyBoxes[1]);
	};

	void render(Camera camera, glm::mat4 projection){
		//Skybox
		glDepthFunc(GL_LEQUAL);
		this->shader.use();

		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setInt("skybox", 0);

		glBindVertexArray(this->VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	};

	void updateSkybox(int index){
		if (index < this->skyBoxes.size()) {
			this->cubeMapTexture = loadCubeMap(this->skyBoxes[index]);
		}
		else {
			std::cout << "Skybox Index out of range" << std::endl;
		}
	};

private:
	//std::vector<std::string> faces;
	std::vector<std::vector<std::string>> skyBoxes;

	Shader shader;
	
	float* cubemapVertices;

	unsigned int VAO;
	unsigned int VBO;

	unsigned int cubeMapTexture;

	void initVertices(){
		float cmVertices[] =
		{
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		this->cubemapVertices = new float[3 * 36];
		std::memcpy(this->cubemapVertices, cmVertices, sizeof(cmVertices));
		};
	
	void initBuffer(){
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), this->cubemapVertices, GL_STATIC_DRAW); // Problem hier

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	};

	void initDefaultSkyboxes() {
		this->skyBoxes.push_back(std::vector<std::string>
		{
			R"(resources\textures\skybox\right.jpg)",
				R"(resources\textures\skybox\left.jpg)",
				R"(resources\textures\skybox\top.jpg)",
				R"(resources\textures\skybox\bottom.jpg)",
				R"(resources\textures\skybox\front.jpg)",
				R"(resources\textures\skybox\back.jpg)"
		});
		this->skyBoxes.push_back(std::vector<std::string>
		{
			R"(resources\textures\skybox\space_1_right.jpg)",
				R"(resources\textures\skybox\space_1_left.jpg)",
				R"(resources\textures\skybox\space_1_top.jpg)",
				R"(resources\textures\skybox\space_1_bottom.jpg)",
				R"(resources\textures\skybox\space_1_front.jpg)",
				R"(resources\textures\skybox\space_1_back.jpg)"
		});
		this->skyBoxes.push_back(std::vector<std::string>
		{
			R"(resources\textures\skybox\forest_1_right.jpg)",
				R"(resources\textures\skybox\forest_1_left.jpg)",
				R"(resources\textures\skybox\forest_1_top.jpg)",
				R"(resources\textures\skybox\forest_1_bottom.jpg)",
				R"(resources\textures\skybox\forest_1_front.jpg)",
				R"(resources\textures\skybox\forest_1_back.jpg)"
		});
		this->skyBoxes.push_back(std::vector<std::string>
		{
			R"(resources\textures\skybox\city_2_right.jpg)",
				R"(resources\textures\skybox\city_2_left.jpg)",
				R"(resources\textures\skybox\city_2_top.jpg)",
				R"(resources\textures\skybox\city_2_bottom.jpg)",
				R"(resources\textures\skybox\city_2_front.jpg)",
				R"(resources\textures\skybox\city_2_back.jpg)"
		});
	}
	
	unsigned int loadCubeMap(std::vector<std::string> faces){
		unsigned int textureID;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;

		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

			if (data)
			{
				stbi_set_flip_vertically_on_load(GL_FALSE);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	};
};

