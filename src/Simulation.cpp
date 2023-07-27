#include "Simulation.h"
#include <thread>
#include <random>

// Base Colors
#define RED glm::vec3(1.0f, 0.0f, 0.0f)
#define GREEN glm::vec3(0.0f, 1.0f, 0.0f)
#define BLUE glm::vec3(0.0f, 0.0f, 1.0f)
#define WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define BLACK glm::vec3(0.0f, 0.0f, 0.0f)

// Custom Colors
#define ORANGE glm::vec3(1.0f, 0.5f, 0.0f)
#define PURPLE glm::vec3(0.5f, 0.0f, 0.5f)
#define YELLOW glm::vec3(1.0f, 1.0f, 0.0f)
#define CYAN glm::vec3(0.0f, 1.0f, 1.0f)
#define MAGENTA glm::vec3(1.0f, 0.0f, 1.0f)


// Pastell Colors
#define PASTEL_PINK glm::vec3(0.86f, 0.63f, 0.69f)
#define PASTEL_YELLOW glm::vec3(0.95f, 0.95f, 0.58f)
#define PASTEL_BLUE glm::vec3(0.62f, 0.77f, 0.87f)
#define PASTEL_GREEN glm::vec3(0.64f, 0.87f, 0.68f)
#define PASTEL_PURPLE glm::vec3(0.73f, 0.62f, 0.81f)

// Metallic Colors
#define GOLD glm::vec3(1.0f, 0.84f, 0.0f)
#define SILVER glm::vec3(0.75f, 0.75f, 0.75f)
#define BRONZE glm::vec3(0.8f, 0.5f, 0.2f)
#define COPPER glm::vec3(0.85f, 0.55f, 0.4f)
#define STEEL glm::vec3(0.6f, 0.6f, 0.67f)

// Rainbow Colors
#define VIOLET glm::vec3(0.5f, 0.0f, 1.0f)
#define INDIGO glm::vec3(0.29f, 0.0f, 0.51f)
#define BLUE_GREEN glm::vec3(0.0f, 0.5f, 0.5f)
#define YELLOW_GREEN glm::vec3(0.6f, 0.8f, 0.2f)
#define YELLOW_ORANGE glm::vec3(1.0f, 0.71f, 0.0f)
#define RED_ORANGE glm::vec3(1.0f, 0.27f, 0.0f)

// Earth Tones
#define BROWN glm::vec3(0.65f, 0.16f, 0.16f)
#define SAND glm::vec3(0.76f, 0.7f, 0.5f)
#define OLIVE glm::vec3(0.5f, 0.5f, 0.0f)
#define MOSS_GREEN glm::vec3(0.55f, 0.64f, 0.45f)
#define SLATE_GRAY glm::vec3(0.44f, 0.5f, 0.56f)

// Neon Colors
#define NEON_PINK glm::vec3(1.0f, 0.07f, 0.55f)
#define NEON_YELLOW glm::vec3(1.0f, 0.95f, 0.0f)
#define NEON_GREEN glm::vec3(0.29f, 0.95f, 0.29f)
#define NEON_BLUE glm::vec3(0.29f, 0.59f, 0.95f)
#define NEON_PURPLE glm::vec3(0.67f, 0.29f, 0.95f)

#define DIMENSION 1000.0f

Simulation::Simulation(GLFWwindow* window, int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
	this->window = window;
	this->WINDOW_WIDTH = WINDOW_WIDTH;
	this->WINDOW_HEIGHT = WINDOW_HEIGHT;

	this->initShader();
	this->initVertices();
	this->initVariables();
	this->initModels();
	this->initBuffer();

	//ImGUI Setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(this->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

}

void Simulation::update(float deltaTime, int FPS, Camera camera)
{
	//Update view and projection matrix
	this->projection = glm::perspective(glm::radians(camera.Zoom), (float)this->WINDOW_WIDTH / (float)this->WINDOW_HEIGHT, 0.1f, 10000.0f);
	this->view = camera.GetViewMatrix();

	//Update Timing
	this->deltaTime = deltaTime;
	this->FPS = FPS;
	this->camera = camera;

	this->processInput(deltaTime);

	if (this->start)
	{
		this->updateSimulation();
	}
}

void Simulation::render()
{
	//Draw scene to framebuffer which will be rendered as a texture for post processing purposes
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	glEnable(GL_DEPTH_TEST);
	//Sets Backgroundcolor to dimmed color of the directional light color
	glClearColor(this->dirLightColor.x* 0.2f, this->dirLightColor.y* 0.2f, this->dirLightColor.z* 0.2f, 0.2f);

	//Bufferclear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render
	this->DrawSimulation();

	this->DrawSkyBox();

	this->particleMaster->render(this->projection, this->camera);

	this->DrawScreen();

	this->DrawText();

	this->DrawSettings();

	ImGui::EndFrame();
}

float Simulation::getCameraSpeed()
{
	return this->cameraSpeed;
}

bool Simulation::getViewMode()
{
	return this->viewMode;
}

//Inits------------------------------------------------------------------------------

void Simulation::initVertices()
{
	//Screen quad
	float qVertices[] = {
		// positions // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	this->quadVertices = new float[6 * 4];

	std::memcpy(this->quadVertices, qVertices, sizeof(qVertices));

	float gVertices[] = {
		// positions          // normals           // texture coords
		// Bottom face
		-DIMENSION, -0.5f, -DIMENSION,  0.0f, 1.0f,  0.0f,  0.0f,  0.0f,
		 DIMENSION, -0.5f, -DIMENSION,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 DIMENSION, -0.5f,  DIMENSION,  0.0f, 1.0f,  0.0f,  1.0f,  1.0f,
		 DIMENSION, -0.5f,  DIMENSION,  0.0f, 1.0f,  0.0f,  1.0f,  1.0f,
		-DIMENSION, -0.5f,  DIMENSION,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-DIMENSION, -0.5f, -DIMENSION,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,

		// Top face
		-DIMENSION,  0.5f, -DIMENSION,  0.0f,  -1.0f,  0.0f,  0.0f,  0.0f,
		 DIMENSION,  0.5f, -DIMENSION,  0.0f,  -1.0f,  0.0f,  1.0f,  0.0f,
		 DIMENSION,  0.5f,  DIMENSION,  0.0f,  -1.0f,  0.0f,  1.0f,  1.0f,
		 DIMENSION,  0.5f,  DIMENSION,  0.0f,  -1.0f,  0.0f,  1.0f,  1.0f,
		-DIMENSION,  0.5f,  DIMENSION,  0.0f,  -1.0f,  0.0f,  0.0f,  1.0f,
		-DIMENSION,  0.5f, -DIMENSION,  0.0f,  -1.0f,  0.0f,  0.0f,  1.0f,

		// Front face
		-DIMENSION, -0.5f, -DIMENSION,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 DIMENSION, -0.5f, -DIMENSION,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 DIMENSION,  0.5f, -DIMENSION,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 DIMENSION,  0.5f, -DIMENSION,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-DIMENSION,  0.5f, -DIMENSION,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-DIMENSION, -0.5f, -DIMENSION,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

		// Back face
		-DIMENSION, -0.5f,  DIMENSION,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 DIMENSION, -0.5f,  DIMENSION,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 DIMENSION,  0.5f,  DIMENSION,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 DIMENSION,  0.5f,  DIMENSION,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-DIMENSION,  0.5f,  DIMENSION,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-DIMENSION, -0.5f,  DIMENSION,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

		// Left face
		-DIMENSION, -0.5f,  DIMENSION, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-DIMENSION, -0.5f, -DIMENSION, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-DIMENSION,  0.5f, -DIMENSION, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-DIMENSION,  0.5f, -DIMENSION, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-DIMENSION,  0.5f,  DIMENSION, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-DIMENSION, -0.5f,  DIMENSION, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,

		// Right face
		 DIMENSION, -0.5f,  DIMENSION,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 DIMENSION, -0.5f, -DIMENSION,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 DIMENSION,  0.5f, -DIMENSION,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 DIMENSION,  0.5f, -DIMENSION,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 DIMENSION,  0.5f,  DIMENSION,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 DIMENSION, -0.5f,  DIMENSION,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	};




	this->groundVertices = new float[36 * 8];

	std::memcpy(this->groundVertices, gVertices, sizeof(gVertices));

}

void Simulation::initBuffer()
{
	//Screen Buffer
	glGenVertexArrays(1, &this->screenVAO);
	glGenBuffers(1, &this->screenVBO);
	glBindVertexArray(this->screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->screenVBO);
	//position attribute
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, this->quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Ground Buffer
	glGenVertexArrays(1, &this->groundVAO);
	glGenBuffers(1, &this->groundVBO);
	glBindVertexArray(this->groundVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->groundVBO);
	//position attribute
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 8, this->groundVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	//textur attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6* sizeof(float)));
	glEnableVertexAttribArray(2);

	//Screen Framebuffer
	glGenFramebuffers(1, &this->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	//create a color attachment texture
	glGenTextures(1, &this->texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, this->texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->WINDOW_WIDTH, this->WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texColorBuffer, 0);

	//create a renderbuffer object for depth and stencil attachment
	glGenRenderbuffers(1, &this->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, this->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Simulation::initShader()
{
	//Create Shader objects for several shader units
	this->screenShader = Shader("Shader/screen.vs", "Shader/screen.fs");
	this->textShader = Shader("Shader/text.vs", "Shader/text.fs");
	this->cubeMapShader = Shader("Shader/cubemap.vs", "Shader/cubemap.fs");
	this->groundShader = Shader("Shader/ground.vs", "Shader/ground.fs");
	this->planeShader = Shader("Shader/plane.vs", "Shader/plane.fs");
	this->missileShader = Shader("Shader/missile.vs", "Shader/missile.fs");
	this->torretShader = Shader("Shader/torret.vs", "Shader/torret.fs");
}

void Simulation::initVariables()
{
	this->initSettings();
	this->initTextures();
	this->initText();
	this->initMatrices();
	this->initSkybox();
	this->initParticleSystem();
	this->initPlanes();
	this->initTorrets();
	this->initGunTower();
}

void Simulation::initSettings()
{
	this->cameraSpeed = 150.0f;
	this->start = false;
	this->viewMode = true;
	this->dirLightColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->dirLightPos = glm::vec3(glm::vec3(-0.5, 0.7, -1.0) * 40.f);
	this->startKeyPressed = false;
	this->settingsKeyPressed = false;
	this->shadingKeyPressed = false;
	this->shootGunTower = false;
	this->shootMissileTruck = false;
	this->shadingChoice = 0;
	this->timeFactor = 1.0f;
	this->shot = false;
}

void Simulation::initTextures()
{
	this->star = this->loadTextures(R"(resources/textures/particleStar.png)");

}
void Simulation::initText()
{
	this->textRenderer = new TextRenderer(10, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	this->fontSize = 10;
}

void Simulation::initMatrices()
{
	this->projection = glm::mat4(1.0f);
	this->view = glm::mat4(1.0f);
}

void Simulation::initSkybox()
{
	this->oceanBox = new Skybox(this->ocean);
	this->spaceBox = new Skybox(this->space);
	this->forestBox = new Skybox(this->forest);
	this->cityBox = new Skybox(this->city);
	this->skyBoxChoice = 1;
}

void Simulation::initParticleSystem()
{
	this->particleMaster = new ParticleMaster();
}

void Simulation::initPlanes()
{
	int amountPlanes = 6;
	for (int i = 0; i < amountPlanes; i++) {
		for (int j = 0; j < amountPlanes; j++) {
			this->planes.push_back(new Planes(glm::vec3(j * (rand() % 200 + -100), 400.0 + j * (rand() % 10 + 1), -1000.0 + i * (rand() % 20 - 100)), glm::vec3(0.001, 0.0, 1.00), 70, glm::vec3(0.1)));
		}
	}
	this->rotSpeedX = 0.0f;
	this->rotSpeedY = 0.0f;
	this->rotSpeedZ = 0.0f;
}

void Simulation::initTorrets()
{
	int amnountTorrets = 2;
	for (int i = 0; i < amnountTorrets; i++) {
		for (int j = 0; j < amnountTorrets; j++) {
			glm::vec3 randomPos = glm::vec3(-600 + j * 50, 0.0f, 0.0f + i * 50);
			this->torrets.push_back(new Torret(randomPos));
		}
	}
	amnountTorrets = 2;
	for (int i = 0; i < amnountTorrets; i++) {
		for (int j = 0; j < amnountTorrets; j++) {
			glm::vec3 randomPos = glm::vec3(-200 + j * 50, 0.0f, 0.0f + i * 50);
			this->missileTrucks.push_back(new Torret(randomPos));
		}
	}
}

void Simulation::initGunTower()
{
	int amountgunTower = 2;
	for (int i = 0; i < amountgunTower; i++) {
		for (int j = 0; j < amountgunTower; j++) {
			glm::vec3 randomPos = glm::vec3(0.0 + j * 50, 0.0f, 0.0f + i * 50);
			this->gunTowers.push_back(new GunTower(randomPos, 15.0f));
		}
	}
}

void Simulation::initModels()
{
	this->plane = new ModelHandler(R"(resources\models\plane\plane.obj)");
	this->plane->Scale(1.0f);
	this->missile = new ModelHandler(R"(resources\models\missile\missile.obj)");
	this->missile->Scale(1.0f);
	this->torret = new ModelHandler(R"(resources\models\torret\torret.obj)");
	this->torret->Scale(1.0f);
	this->missileTruck = new ModelHandler(R"(resources\models\missileTruck\missileTruck.obj)");
	this->missileTruck->Scale(1.0f);
	this->s400 = new ModelHandler(R"(resources\models\missile\s400.obj)");
	this->s400->Scale(1.0f);
	this->gunTower = new ModelHandler(R"(resources\models\gunTower\gunTower.obj)");
	this->gunTower->Scale(1.0f);
	this->barrel = new ModelHandler(R"(resources\models\gunTower\barrel.obj)");
	this->barrel->Scale(1.0f);
}

//Inputhandling------------------------------------------------------------------------------

void Simulation::processInput(float deltaTime)
{
	//Keyboard shortcuts
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, true);
	}
	if (glfwGetKey(this->window, GLFW_KEY_F) == GLFW_PRESS && !this->settingsKeyPressed)
	{
		this->viewMode = !this->viewMode;
		this->settingsKeyPressed = true;
	}

	if (glfwGetKey(this->window, GLFW_KEY_F) == GLFW_RELEASE)
	{
		this->settingsKeyPressed = false;
	}
	if (!this->viewMode) {
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (glfwGetKey(this->window, GLFW_KEY_ENTER) == GLFW_PRESS && !this->startKeyPressed)
	{
		this->start = !this->start;
		this->startKeyPressed = true;
	}
	if (glfwGetKey(this->window, GLFW_KEY_ENTER) == GLFW_RELEASE)
	{
		this->startKeyPressed = false;
	}
	if (glfwGetKey(this->window, GLFW_KEY_B) == GLFW_PRESS && !this->shadingKeyPressed)
	{
		this->shadingChoice = ++this->shadingChoice%2;
		this->shadingKeyPressed = true;
	}
	if (glfwGetKey(this->window, GLFW_KEY_B) == GLFW_RELEASE)
	{
		this->shadingKeyPressed = false;
	}
	if (glfwGetKey(this->window, GLFW_KEY_4) == GLFW_PRESS)
	{
		this->skyBoxChoice = 4;
	}
	if (glfwGetKey(this->window, GLFW_KEY_3) == GLFW_PRESS)
	{
		this->skyBoxChoice = 3;
	}
	if (glfwGetKey(this->window, GLFW_KEY_2) == GLFW_PRESS)
	{
		this->skyBoxChoice = 2;
	}
	if (glfwGetKey(this->window, GLFW_KEY_1) == GLFW_PRESS)
	{
		this->skyBoxChoice = 1;
	}
	if (glfwGetKey(this->window, GLFW_KEY_0) == GLFW_PRESS)
	{
		this->skyBoxChoice = 0;
	}
	if (glfwGetKey(this->window, GLFW_KEY_R) == GLFW_PRESS)
	{
		for (auto i : planes) {
			i->setPosition(i->getStartPosition());
			i->setDirection(i->getStartDirection());
		}
		for (auto i : missiles) {
			i->setPosition(i->getStartPosition());
			i->setDirection(i->getStartDirection());
		}
	}

	if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotSpeedZ = 0.0f;
		rotSpeedY = 0.0f;
		rotSpeedX = -0.05f; 
	}
	else if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotSpeedZ = 0.0f;
		rotSpeedY = 0.0f;
		rotSpeedX = 0.05f;
	}

	if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS) {
		rotSpeedZ = 0.0f;
		rotSpeedX = 0.0f;
		rotSpeedY = -0.05f; 
	}
	else if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		rotSpeedZ = 0.0f;
		rotSpeedX = 0.0f;
		rotSpeedY = 0.05f;
	}

	if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS) {
		rotSpeedX = 0.0f;
		rotSpeedY = 0.0f;
		rotSpeedZ = 0.05f; 
	}
	else if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS) {
		rotSpeedX = 0.0f;
		rotSpeedY = 0.0f;
		rotSpeedZ = -0.05f;
	} 
	if (glfwGetKey(this->window, GLFW_KEY_K) == GLFW_PRESS) {
		this->shootGunTower = true;
	}
	else {
		this->shootGunTower = false;
	}
	if (glfwGetKey(this->window, GLFW_KEY_P) == GLFW_PRESS && !this->shootMissileTruck) {
		this->shootMissileTruck = true;
	}

}

//Helper------------------------------------------------------------------------------

int Simulation::random(int range, int start)
{
	return rand() % range + start;
}

void Simulation::explosion(glm::vec3 pos, glm::vec3 direction, int spreadDiversity, float spreadFactor, int amount, int maxDuration, float gravityImpact = 0.001f, float scale = 0.5f)
{
	for (int k = 0; k < amount; k++) {
		spreadDiversity += rand() % (int)(spreadDiversity / 5) - spreadDiversity / 10;
		this->particleMaster->addParticle(new Particle(
			ParticleTextureHandler(this->star, 1),
			pos,
			glm::vec3(spreadFactor * (rand() % spreadDiversity - (spreadDiversity / 2)), spreadFactor * (rand() % spreadDiversity - (spreadDiversity / 2)), spreadFactor * (rand() % spreadDiversity - (spreadDiversity / 2)))+direction,
			gravityImpact, (rand() % 5*maxDuration + 1) / maxDuration, 0, scale, "EXPLOSION"));

	}
}
unsigned int Simulation::loadTextures(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (format = GL_RGBA)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;

}
//Updates------------------------------------------------------------------------------

void Simulation::updateSimulation()
{
	this->updatePlanes();
	this->updateTorrets();
	this->updateCruiseMissile();
	this->updateMissiles();
	this->updateGunTower();

	this->updateCrashingPlanes();
	this->updateHitPlane();
	this->updateHitMissile();
	this->updateErasing();
	this->updatePlaneHitsPlane();

	this->particleMaster->update(this->deltaTime*this->timeFactor);

}

void Simulation::updatePlanes()
{
	for (auto i : this->planes) {
		i->update(this->deltaTime * this->timeFactor);
		i->setDirection(glm::vec3(i->getDirection().x + rotSpeedX, i->getDirection().y + rotSpeedY, i->getDirection().z + rotSpeedZ));

		int spreadFactor = 5;
		float spread = 0.1;
		this->particleMaster->addParticle(new Particle(ParticleTextureHandler(this->star, 1),i->getPosition()-10.0f*i->getDirection(), 0.2f * -glm::normalize(glm::vec3(i->getDirection().x + spread * (rand() % spreadFactor - (spreadFactor / 2)), i->getDirection().y + spread * (rand() % spreadFactor - (spreadFactor / 2)), i->getDirection().z + spread * (rand() % spreadFactor - (spreadFactor / 2)))), 0.001, (rand() % 40 + 10) / 20, 0, 0.5, "TAIL"));
	}
}

void Simulation::updateMissiles()
{
	for (size_t i = 0; i < this->missiles.size(); ++i) {
		this->missiles[i]->update(this->deltaTime * this->timeFactor);

		if (this->planes.size() > 0) {

			int nearest;
			float nearestDistance;
			std::tuple<int, float> result = this->updateNearestPlane(missiles[i], planes);
			nearest = std::get<0>(result);
			nearestDistance = std::get<1>(result);
			glm::vec3 direction = glm::normalize(planes[nearest]->getPosition() - missiles[i]->getPosition());

			float angleToTarget = glm::angle(glm::normalize(this->missiles[i]->getDirection()), direction);
			if (angleToTarget < 0.9) {
				if (nearestDistance > 50 && nearestDistance < 1000)
				{
					float acc = 3.0f * this->deltaTime * this->timeFactor;
					this->missiles[i]->setDirection(glm::vec3(this->missiles[i]->getDirection().x + direction.x * acc, this->missiles[i]->getDirection().y + direction.y * acc, this->missiles[i]->getDirection().z + direction.z * acc));
				}

				else if (nearestDistance <= 50) {
					float acc = 8.0f * this->deltaTime * this->timeFactor;
					this->missiles[i]->setDirection(glm::vec3(this->missiles[i]->getDirection().x + direction.x * acc, this->missiles[i]->getDirection().y + direction.y * acc, this->missiles[i]->getDirection().z + direction.z * acc));
				}
			}
		}
		int spreadFactor = 5;
		float spread = 0.05;
		this->particleMaster->addParticle(new Particle(ParticleTextureHandler(this->star, 1),
			this->missiles[i]->getPosition()-this->missiles[i]->getDirection(),
			0.2f*-glm::normalize(glm::vec3(this->missiles[i]->getDirection().x + spread*(rand()% spreadFactor - (spreadFactor/2)), this->missiles[i]->getDirection().y + spread*(rand() % spreadFactor - (spreadFactor / 2)), this->missiles[i]->getDirection().z + spread*(rand() % spreadFactor - (spreadFactor / 2)) / 10)), 
			0.01, (rand()%40+10)/20, 0, 0.5, "TAIL"));

		if (missiles[i]->getPosition().y < 0) {
			this->eraseMissiles.insert(i);
			this->explosion(this->missiles[i]->getPosition(), -this->missiles[i]->getDirection(), 1000, 0.001, 150, 70, 0.09);
		}
		if (this->missiles[i]->getTimer() > 35.0f) {
			this->eraseMissiles.insert(i);
			this->explosion(this->missiles[i]->getPosition(), this->missiles[i]->getDirection(), 1000, 0.001, 150, 70, 0.09);
		}
	}
}

void Simulation::updateCruiseMissile()
{

	for (size_t i = 0; i < this->cruiseMissiles.size(); ++i) {
		this->cruiseMissiles[i]->update(this->deltaTime * this->timeFactor);
		int nearest;
		float nearestDistance = 1001;
		std::tuple<int, float> result;
		glm::vec3 direction = this->cruiseMissiles[i]->getDirection();

		if (this->planes.size() > 0) {
			result = this->updateNearestPlane(cruiseMissiles[i], planes);
			nearest = std::get<0>(result);
			nearestDistance = std::get<1>(result);
			direction = glm::normalize(planes[nearest]->getPosition() - cruiseMissiles[i]->getPosition());
		}

		if (this->cruiseMissiles[i]->getTimer() < 1.3f) {
			this->cruiseMissiles[i]->setAccAcc(glm::vec3(0.0f));
			float acc = 2.0f * this->deltaTime*this->timeFactor;
			this->cruiseMissiles[i]->setVelocity(glm::vec3(0.0f, 14.0f, 0.0f)-glm::vec3(0.0f, 10*this->cruiseMissiles[i]->getTimer(), 0.0f));
			this->cruiseMissiles[i]->setMaxVelocity(glm::vec3(300.0f));
			//this->cruiseMissiles[i]->setDirection(glm::vec3(this->cruiseMissiles[i]->getDirection().x + direction.x * acc, this->cruiseMissiles[i]->getDirection().y + direction.y * acc, this->cruiseMissiles[i]->getDirection().z + direction.z * acc));
		}

		else {
			this->cruiseMissiles[i]->setAccAcc(glm::vec3(50.0f));
			if (this->cruiseMissiles[i]->getTimer() < 1.6f) {
				this->cruiseMissiles[i]->setDirection(glm::vec3(0.0f, 1.0f, 0.001f));

			}
			else if (this->cruiseMissiles[i]->getTimer() < 2.0f) {
				this->cruiseMissiles[i]->setAcceleration(glm::vec3(this->cruiseMissiles[i]->getAcceleration().x, this->cruiseMissiles[i]->getAcceleration().x + 60, this->cruiseMissiles[i]->getAcceleration().z));
				float acc = 4.0f * this->deltaTime*this->timeFactor;
				this->cruiseMissiles[i]->setDirection(glm::vec3(this->cruiseMissiles[i]->getDirection().x + direction.x * acc, this->cruiseMissiles[i]->getDirection().y + direction.y * acc, this->cruiseMissiles[i]->getDirection().z + direction.z * acc));
			}

			else {
				this->cruiseMissiles[i]->setAcceleration(glm::vec3(this->cruiseMissiles[i]->getAcceleration().z));

				float angleToTarget = glm::angle(glm::normalize(this->cruiseMissiles[i]->getDirection()), direction);
				if (angleToTarget < 0.9) {
					if (nearestDistance > 50)
					{
						float acc = 4.0f * this->deltaTime*this->timeFactor;
						this->cruiseMissiles[i]->setDirection(glm::vec3(this->cruiseMissiles[i]->getDirection().x + direction.x * acc, this->cruiseMissiles[i]->getDirection().y + direction.y * acc, this->cruiseMissiles[i]->getDirection().z + direction.z * acc));
					}

					else if (nearestDistance <= 50) {
						float acc = 8.0f * this->deltaTime*this->timeFactor;
						this->cruiseMissiles[i]->setDirection(glm::vec3(this->cruiseMissiles[i]->getDirection().x + direction.x * acc, this->cruiseMissiles[i]->getDirection().y + direction.y * acc, this->cruiseMissiles[i]->getDirection().z + direction.z * acc));
					}
				}
			}
			int spreadFactor = 5;
			float spread = 0.05;
			this->particleMaster->addParticle(new Particle(
				ParticleTextureHandler(this->star, 1),
				this->cruiseMissiles[i]->getPosition() - this->cruiseMissiles[i]->getDirection(),
				0.2f * -glm::normalize(glm::vec3(this->cruiseMissiles[i]->getDirection().x + spread * (rand() % spreadFactor - (spreadFactor / 2)), this->cruiseMissiles[i]->getDirection().y + spread * (rand() % spreadFactor - (spreadFactor / 2)), this->cruiseMissiles[i]->getDirection().z + spread * (rand() % spreadFactor - (spreadFactor / 2)) / 10)),
				0.01, (rand() % 40 + 10) / 20, 0, 0.5, "TAIL"));
		}

		if (this->cruiseMissiles[i]->getPosition().y < 0) {
			this->eraseCruiseMissiles.insert(i);
			this->explosion(this->cruiseMissiles[i]->getPosition(), -this->cruiseMissiles[i]->getDirection(), 1000, 0.001, 150, 70, 0.09);
		}
		if (this->cruiseMissiles[i]->getTimer() > 35.0f) {
			this->eraseCruiseMissiles.insert(i);
			this->explosion(this->cruiseMissiles[i]->getPosition(), this->cruiseMissiles[i]->getDirection(), 1000, 0.001, 150, 70, 0.09);
		}
	}
}

void Simulation::updateTorrets()
{
	for (size_t i = 0; i < this->torrets.size(); ++i) {

		int nearest;
		float nearestDistance = 1001;
		glm::vec3 direction;

		if (!this->torrets[i]->getShot() && this->planes.size()>0) {
			std::tuple<int, float> result = this->updateNearestPlane(torrets[i]->getMissile(), planes);
			nearest = std::get<0>(result);
			nearestDistance = std::get<1>(result);

			direction = glm::normalize(planes[nearest]->getPosition() - this->torrets[i]->getPosition());
		}

		if (nearestDistance < 1000) {
			if (!this->torrets[i]->getShot()) {
				float rotSpeed = 0.1f;
				this->torrets[i]->setDirection(glm::vec3(this->torrets[i]->getDirection().x + direction.x * rotSpeed, 0.0f, this->torrets[i]->getDirection().z + direction.z * rotSpeed));
				this->torrets[i]->getMissile()->setDirection(glm::vec3(this->torrets[i]->getMissile()->getDirection().x + rotSpeed * direction.x, this->torrets[i]->getMissile()->getDirection().y + rotSpeed * direction.y, this->torrets[i]->getMissile()->getDirection().z + rotSpeed * direction.z));
			}
			this->torrets[i]->update(this->deltaTime*this->timeFactor);
		}

		if (nearestDistance < 800 && this->planes.size()>0) {
			this->torrets[i]->setShot(true);
			this->missiles.push_back(this->torrets[i]->getMissile());
		}
	}

	for (size_t i = 0; i < this->missileTrucks.size(); ++i) {
		this->missileTrucks[i]->update(this->deltaTime*this->timeFactor);
		if (this->shootMissileTruck && this->planes.size()>0 && !this->shot) {
			this->missileTrucks[i]->setShot(true);
			this->cruiseMissiles.push_back(this->missileTrucks[i]->getMissile());

			this->explosion(this->missileTrucks[i]->getPosition() + glm::vec3(0.0f, 4.13f, 0.0f), glm::vec3(0.0f), 1000.0f, 0.0001, 5, 5, 0.01, 0.32);
		}
	}
	if (this->shootMissileTruck) {
		this->shot = true;
	}
}

void Simulation::updateGunTower()
{
	for (size_t i = 0; i < this->gunTowers.size(); ++i) {

		int nearest;
		float nearestDistance = 1001;
		glm::vec3 direction;
		float rotSpeed = 0.1f;

		if (this->planes.size() > 0) {
			std::tuple<int, float> result = this->updateNearestPlane(gunTowers[i]->getTower(), planes);
			nearest = std::get<0>(result);
			nearestDistance = std::get<1>(result);

			direction = glm::normalize(planes[nearest]->getPosition() - this->gunTowers[i]->getPosition());
			float holdUpFactor = 0.2f;
			glm::vec3 shootDirection = direction + this->planes[nearest]->getDirection() * holdUpFactor;

			this->gunTowers[i]->setDirection(glm::vec3(this->gunTowers[i]->getDirection().x + shootDirection.x * rotSpeed, 0.0f, this->gunTowers[i]->getDirection().z + shootDirection.z * rotSpeed));
			this->gunTowers[i]->getTower()->setDirection(glm::vec3(this->gunTowers[i]->getTower()->getDirection().x + rotSpeed * shootDirection.x, this->gunTowers[i]->getTower()->getDirection().y + rotSpeed * shootDirection.y, this->gunTowers[i]->getTower()->getDirection().z + rotSpeed * shootDirection.z));

			if (this->shootGunTower) {

				for (int l = 0; l < (int)this->gunTowers[i]->getSpeed(); l++) {
					Particle* p = new Particle(ParticleTextureHandler(this->star, 1),
						this->gunTowers[i]->getPosition() + glm::normalize(shootDirection) * (float)l,
						gunTowers[i]->getSpeed() * glm::normalize(shootDirection),
						0.1f,
						5.0f,
						0.0f,
						0.5f,
						"TAIL");
					this->particleMaster->addParticle(p);
					this->particles.push_back(p);
				}
				this->explosion(this->gunTowers[i]->getTower()->getPosition()+shootDirection, glm::vec3(0.0f), 1000, 0.0001, 5, 10, 0.01, 0.02);
			}
		}
		else {
			this->gunTowers[i]->setDirection(glm::vec3(this->gunTowers[i]->getTower()->getDirection().x + rotSpeed * 0.0f, this->gunTowers[i]->getTower()->getDirection().y + rotSpeed * 0.0f, this->gunTowers[i]->getTower()->getDirection().z + rotSpeed * -1.0f));
			this->gunTowers[i]->getTower()->setDirection(glm::vec3(this->gunTowers[i]->getTower()->getDirection().x + rotSpeed * 0.0f, this->gunTowers[i]->getTower()->getDirection().y + rotSpeed * 0.0f, this->gunTowers[i]->getTower()->getDirection().z + rotSpeed * -1.0f));
		}
		this->gunTowers[i]->update(this->deltaTime * this->timeFactor);
	}
}

std::tuple<int, float> Simulation::updateNearestPlane(Missile* missile, vector<Planes*> planes)
{
	int nearest = 0;
	float tempDistance = 100000000.0f;

	for (size_t i = 0; i < planes.size(); i++ ) {
		float distance;
		glm::vec3 connectionVector = planes[i]->getPosition() - missile->getPosition();
		distance = sqrt(connectionVector.x * connectionVector.x + connectionVector.y * connectionVector.y + connectionVector.z * connectionVector.z);
		if (distance < tempDistance) {
			tempDistance = distance;
			nearest = i;
		}
	}

	return std::make_tuple(nearest, tempDistance);
}

void Simulation::updateHitPlane()
{
	float radius = 5;
	for (size_t i = 0; i < missiles.size(); ++i) {
		for (size_t j = 0; j < planes.size(); ++j) {

			glm::vec3 connectionVector = planes[j]->getPosition() - missiles[i]->getPosition();
			float distance = sqrt(connectionVector.x * connectionVector.x + connectionVector.y * connectionVector.y + connectionVector.z * connectionVector.z);
		
			if (distance <= radius) {
				int crashProb = rand() % 2 + 1;
				if (crashProb == 1) {
					planes[j]->setCrashRotationSpeed(rand() % 10 + - 5);
					this->crashingPlanes.push_back(planes[j]);
				}
				this->eraseMissiles.insert(i);
				this->erasePlanes.insert(j);
				this->explosion(this->planes[j]->getPosition(), this->planes[j]->getDirection(), 1000, 0.002, 1000, 100);
			}
		}
	}
	radius = 5;
	for (size_t i = 0; i < cruiseMissiles.size(); ++i) {
		for (size_t j = 0; j < planes.size(); ++j) {

			glm::vec3 connectionVector = planes[j]->getPosition() - cruiseMissiles[i]->getPosition();
			float distance = sqrt(connectionVector.x * connectionVector.x + connectionVector.y * connectionVector.y + connectionVector.z * connectionVector.z);

			if (distance <= radius) {
				int crashProb = rand() % 2 + 1;
				if (crashProb == 1) {
					planes[j]->setCrashRotationSpeed(rand() % 10 + -5);
					this->crashingPlanes.push_back(planes[j]);
				}
				this->eraseCruiseMissiles.insert(i);
				this->erasePlanes.insert(j);
				this->explosion(this->planes[j]->getPosition(), this->planes[j]->getDirection(), 1000, 0.002, 1000, 100);
			}
		}
	}
	radius = 5;
	for (size_t i = 0; i < particles.size(); ++i) {
		for (size_t j = 0; j < planes.size(); ++j) {
			Particle* p = particles[i];
			glm::vec3 connectionVector = planes[j]->getPosition() - p->getPosition();
			float distance = sqrt(connectionVector.x * connectionVector.x + connectionVector.y * connectionVector.y + connectionVector.z * connectionVector.z);

			if (distance <= radius) {
				int crashProb = rand() % 2 + 1;
				if (crashProb == 1) {
					planes[j]->setCrashRotationSpeed(rand() % 10 + -5);
					this->crashingPlanes.push_back(planes[j]);
				}
				this->erasePlanes.insert(j);
				this->explosion(this->planes[j]->getPosition(), this->planes[j]->getDirection(), 1000, 0.002, 100, 100);
			}
		}
	}
}

void Simulation::updateHitMissile()
{
	float radius = 1;
	for (size_t i = 0; i < missiles.size(); ++i) {
		for (size_t j = i + 1; j < missiles.size(); ++j) {
			glm::vec3 connectionVector = missiles[j]->getPosition() - missiles[i]->getPosition();
			float distance = sqrt(connectionVector.x * connectionVector.x + connectionVector.y * connectionVector.y + connectionVector.z * connectionVector.z);

			if (distance <= radius) {
				this->eraseMissiles.insert(i);
				this->eraseMissiles.insert(j);
				this->missilesSelfDestruct++;
				this->explosion(this->missiles[i]->getPosition(), this->missiles[i]->getDirection(), 1000, 0.001, 150, 70);
			}
		}
	}
}

void Simulation::updatePlaneHitsPlane()
{
	float radius = 5;
	for (size_t i = 0; i < crashingPlanes.size(); ++i) {
		for (size_t j = i + 1; j < planes.size(); ++j) {
			glm::vec3 connectionVector = planes[j]->getPosition() - crashingPlanes[i]->getPosition();
			float distance = sqrt(connectionVector.x * connectionVector.x + connectionVector.y * connectionVector.y + connectionVector.z * connectionVector.z);

			if (distance <= radius) {
				int crashProb = rand() % 2 + 1;
				if (crashProb == 1) {
					planes[j]->setCrashRotationSpeed(rand() % 10 + -5);
					this->crashingPlanes.push_back(planes[j]);
				}
				this->erasePlanes.insert(i);
				this->erasePlanes.insert(j);
				this->planesSelfDestruct++;
				this->explosion(this->planes[j]->getPosition(), this->planes[j]->getDirection(), 1000, 0.002, 1000, 100);

			}
		}
	}
}

void Simulation::updateCrashingPlanes()
{
	for (size_t i = 0; i < this->crashingPlanes.size(); i++) {
		this->crashingPlanes[i]->update(this->deltaTime * this->timeFactor);

		if (this->crashingPlanes[i]->getDirection().y > -0.9) {
			this->crashingPlanes[i]->setDirection(glm::vec3(this->crashingPlanes[i]->getDirection().x, this->crashingPlanes[i]->getDirection().y - 0.003, this->crashingPlanes[i]->getDirection().z));
		}
		else {
			this->crashingPlanes[i]->setDirection(glm::vec3(this->crashingPlanes[i]->getDirection().x, -0.9, this->crashingPlanes[i]->getDirection().z));
		}
		int explodeProb = rand() % 1000 + 1;
		if (explodeProb == 1) {
			this->eraseCrashedPlanes.insert(i);
			this->explosion(this->crashingPlanes[i]->getPosition(), this->crashingPlanes[i]->getDirection(), 1000, 0.002, 1000, 100);
		}
		if (crashingPlanes[i]->getPosition().y < 0) {
			this->eraseCrashedPlanes.insert(i);
			this->explosion(this->crashingPlanes[i]->getPosition(), -this->crashingPlanes[i]->getDirection(), 1000, 0.002, 1000, 100, 0.09);

		}
	}
}

void Simulation::updateErasing()
{
	//Crashed Planes
	for (auto it = eraseCrashedPlanes.rbegin(); it != eraseCrashedPlanes.rend(); ++it) {
		this->crashingPlanes.erase(this->crashingPlanes.begin() + *it);
	}
	this->eraseCrashedPlanes.clear();

	//Missiles
	for (auto it = eraseMissiles.rbegin(); it != eraseMissiles.rend(); ++it) {
		this->missiles.erase(this->missiles.begin() + *it);

	}
	this->eraseMissiles.clear();

	for (auto it = eraseCruiseMissiles.rbegin(); it != eraseCruiseMissiles.rend(); ++it) {
		this->cruiseMissiles.erase(this->cruiseMissiles.begin() + *it);

	}
	this->eraseCruiseMissiles.clear();

	//Planes
	for (auto it = erasePlanes.rbegin(); it != erasePlanes.rend(); ++it) {
		this->planes.erase(this->planes.begin() + *it);
	}
	this->erasePlanes.clear();
}

//Rendering------------------------------------------------------------------------------

void Simulation::DrawSimulation()
{
	this->DrawGround();
	this->DrawTorrets();
	this->DrawPlanes();
	this->DrawGunTower();
	this->DrawMissiles();
	
	glBindVertexArray(0);
}

void Simulation::DrawGround()
{
	this->groundShader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
	this->groundShader.setMat4("model", model);
	this->groundShader.setMat4("view", this->view);
	this->groundShader.setMat4("projection", this->projection);
	this->groundShader.setVec3("viewPos", this->camera.Position);
	glBindVertexArray(this->groundVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Simulation::DrawPlanes()
{
	this->planeShader.use();
	this->planeShader.setVec3("viewPos", this->camera.Position); 

	for (auto i : planes) {
		this->plane->Translate(i->getPosition());
		this->plane->Rotate(i->getRotationAngle(), i->getRotationAxis());
		//this->plane->Rotate(glfwGetTime() * i->getCrahsRotationSpeed(), i->getDirection());

		this->plane->Draw(&this->planeShader, this->projection, this->view, i->getColor());
	}
	for (auto i : crashingPlanes) {
		this->plane->Translate(i->getPosition());

		this->plane->Rotate(i->getRotationAngle(), i->getRotationAxis());
		//this->plane->Rotate(glfwGetTime()*i->getCrahsRotationSpeed(), glm::vec3(1.0, 0.0, 0.0));
		this->plane->Draw(&this->planeShader, this->projection, this->view, i->getColor());
	}

}

void Simulation::DrawMissiles()
{
	this->missileShader.use();
	this->missileShader.setVec3("viewPos", this->camera.Position);

	for (auto i : missiles) {
		this->missile->Translate(i->getPosition());
		this->missile->Rotate(i->getRotationAngle(), i->getRotationAxis());
		this->missile->Draw(&this->missileShader, this->projection, this->view, i->getColor());
	}
	for (auto i : cruiseMissiles) {
		this->s400->Translate(i->getPosition());
		this->s400->Rotate(i->getRotationAngle(), i->getRotationAxis());
		this->s400->Draw(&this->missileShader, this->projection, this->view, i->getColor());
	}
}

void Simulation::DrawTorrets()
{
	this->torretShader.use();
	this->torretShader.setVec3("viewPos", this->camera.Position);

	for (auto i : torrets) {
		this->torret->Translate(i->getPosition());
		this->torret->Rotate(i->getRotationAngle(), i->getRotationAxis());
		this->torret->Draw(&this->torretShader, this->projection, this->view, i->getColor());

		if (!i->getShot()) {
			this->missileShader.use();
			this->missileShader.setVec3("viewPos", this->camera.Position);

			this->missile->Translate(i->getPosition());
			this->missile->Rotate(i->getMissile()->getRotationAngle(), i->getMissile()->getRotationAxis());
			this->missile->Draw(&this->missileShader, this->projection, this->view, i->getColor());
		}
	}

	for (auto i : missileTrucks) {
		this->missileTruck->Translate(i->getPosition());
		this->missileTruck->Rotate(i->getRotationAngle(), i->getRotationAxis());
		this->missileTruck->Draw(&this->torretShader, this->projection, this->view, i->getColor());

		if (!i->getShot()) {
			this->missileShader.use();
			this->missileShader.setVec3("viewPos", this->camera.Position);

			this->s400->Translate(i->getPosition());
			this->s400->Rotate(i->getMissile()->getRotationAngle(), i->getMissile()->getRotationAxis());
			this->s400->Draw(&this->missileShader, this->projection, this->view, i->getColor());
		}
	}

}



void Simulation::DrawGunTower()
{
	for (auto i : gunTowers) {
		this->gunTower->Translate(i->getPosition());
		this->gunTower->Rotate(i->getRotationAngle(), i->getRotationAxis());
		this->gunTower->Draw(&this->torretShader, this->projection, this->view, i->getColor());

		this->missileShader.use();
		this->missileShader.setVec3("viewPos", this->camera.Position);

		this->barrel->Translate(i->getPosition());
		this->barrel->Rotate(i->getTower()->getRotationAngle(), i->getTower()->getRotationAxis());
		this->barrel->Draw(&this->missileShader, this->projection, this->view, i->getColor());
	}
}

void Simulation::DrawSettings()
{
	//Tell ImGUI its a new Frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (!viewMode)
	{
		ImGui::Begin("Steuerung", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImVec2 pos(5, 50);
		ImGui::SetWindowPos(pos);

		// Init Settingswindow
		ImVec2 size((float)this->WINDOW_WIDTH / 4, (float)this->WINDOW_HEIGHT * 0.9f);
		ImGui::SetWindowSize(size);

		//Settings
		ImGui::Text("Settings");
		ImGui::SetNextItemWidth((float)this->WINDOW_WIDTH / 5);
		ImGui::SliderFloat("Camspeed", &this->cameraSpeed, 1.0f, 600.0f);

		//Simulation control

		const char* play = "Start";
		if (this->start) {
			play = "Stopp";
		}
		if (ImGui::Button(play))
		{
			this->start = !this->start;
		}				
		ImGui::SliderFloat("Timefactor", &this->timeFactor, 0.0f, 1.0f);

		const char* shading = "Reflection";
		if (this->shadingChoice == 1) {
			shading = "Colors";
		}
		ImGui::SameLine();
		if (ImGui::Button(shading))
		{
			this->shadingChoice = ++this->shadingChoice % 2;
		}		
		if (ImGui::Button("Ocean"))
		{
			this->skyBoxChoice = 1;
		}
		ImGui::SameLine();
		if (ImGui::Button("Space"))
		{
			this->skyBoxChoice = 2;
		}
		ImGui::SameLine();
		if (ImGui::Button("Forest"))
		{
			this->skyBoxChoice = 3;
		}
		ImGui::SameLine();
		if (ImGui::Button("City"))
		{
			this->skyBoxChoice = 4;
		}


		ImGui::ColorPicker3("DirLight", (float*)&this->dirLightColor, ImGuiColorEditFlags_InputRGB);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

void Simulation::DrawScreen()
{
	//Draw Scene texture to standard Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	this->screenShader.use();
	this->screenShader.setInt("screenTexture", 0);
	this->screenShader.setInt("choice", 7);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->screenVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, this->texColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Simulation::DrawText()
{
	this->textRenderer->Draw(this->textShader, "FPS: " + std::to_string((int)this->FPS), 0.0f, (float)this->WINDOW_HEIGHT - 1 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->Draw(this->textShader, "Pos: " + std::to_string(this->camera.Position.x) + ", " + std::to_string(this->camera.Position.y) + ", " + std::to_string(this->camera.Position.z), 0.0f, (float)this->WINDOW_HEIGHT - 2 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->Draw(this->textShader, "CameraView: " + std::to_string(this->camera.Front.x) + ", " + std::to_string(this->camera.Front.y) + ", " + std::to_string(this->camera.Front.z), 0.0f, (float)this->WINDOW_HEIGHT - 3 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->Draw(this->textShader, "DeltaTime: " + std::to_string(this->deltaTime), 0.0f, (float)this->WINDOW_HEIGHT - 4 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	this->textRenderer->Draw(this->textShader, "Start: " + std::to_string(this->start), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 1 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	std::string shading = "Colors";
	if (this->shadingChoice == 1) {
		shading = "Reflection";
	}
	this->textRenderer->Draw(this->textShader, "Shading: " + shading, this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 2 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	
	std::string skyboxes[] = { "No Skybox", "Ocean", "Space", "Forest", "City" };
	this->textRenderer->Draw(this->textShader, "Skybox: " + skyboxes[skyBoxChoice], this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 3 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->Draw(this->textShader, "Planes: " + std::to_string(this->planes.size()), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 4 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->Draw(this->textShader, "Missiles: " + std::to_string(this->missiles.size()), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 5 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->Draw(this->textShader, "MissilesSelfDestruct: " + std::to_string(this->missilesSelfDestruct), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 6 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->Draw(this->textShader, "PlanesSelfDestruct: " + std::to_string(this->planesSelfDestruct), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 7 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->Draw(this->textShader, "ParticlesAlive: " + std::to_string(this->particleMaster->getParticlesAlive()), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 8 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->Draw(this->textShader, "Debug: " + this->debug, this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 9 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Simulation::DrawSkyBox()
{
	switch (this->skyBoxChoice) {
	case 1:
		this->oceanBox->render(this->cubeMapShader, this->camera, this->projection);
		break;
	case 2:
		this->spaceBox->render(this->cubeMapShader, this->camera, this->projection);
		break;	
	case 3:
		this->forestBox->render(this->cubeMapShader, this->camera, this->projection);
		break;	
	case 4:
		this->cityBox->render(this->cubeMapShader, this->camera, this->projection);
		break;
	default:
		break;
	}
}
