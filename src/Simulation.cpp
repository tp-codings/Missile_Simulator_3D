#include "Simulation.h"
#include <thread>
#include <random>

Simulation::Simulation(GLFWwindow* window, int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
	this->window = window;
	this->WINDOW_WIDTH = WINDOW_WIDTH;
	this->WINDOW_HEIGHT = WINDOW_HEIGHT;

	this->init();

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
	this->projection = glm::perspective(glm::radians(camera.Zoom), (float)this->WINDOW_WIDTH / (float)this->WINDOW_HEIGHT, 0.1f, 100000.0f);
	this->view = camera.GetViewMatrix();

	//Update Timing
	this->deltaTime = deltaTime;
	this->FPS = FPS;
	this->camera = camera;

	this->processInput();

	if (this->start)
	{
		this->updateSimulation();
	}
}

void Simulation::render()
{
	//render scene to framebuffer which will be rendered as a texture for post processing purposes
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	glEnable(GL_DEPTH_TEST);
	//Sets Backgroundcolor to dimmed color of the directional light color
	glClearColor(this->dirLightColor.x* 0.2f, this->dirLightColor.y* 0.2f, this->dirLightColor.z* 0.2f, 0.2f);

	//Bufferclear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render

	this->renderSimulation();

	this->screenRenderer->render(this->texColorBuffer);

	this->renderHUD();

	this->renderSettings();

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

void Simulation::init()
{
	this->screenRenderer = new ScreenRenderer();
	this->skybox = new Skybox();
	this->loader = new Loader();

	this->initMaster();
	this->initSettings();
	this->initHUD();
	this->initMatrices();
	this->initPlanes();
	this->initTorrets();
	this->initGunTower();
	this->initBuffer();
}

void Simulation::initMaster()
{
	this->terrain = new TerrainGenerator(R"(resources\textures\city_heightmap.png)");
	this->planeMaster = new PlaneMaster();
	this->missileMaster = new MissileMaster();
	this->collisionMaster = new CollisionMaster();
	this->torretMaster = new TorretMaster();
	this->s400Master = new S400Master();
	this->missileTruckMaster = new MissileTruckMaster();
	this->gunTowerMaster = new GunTowerMaster();
	this->bulletMaster = new BulletMaster();
}

void Simulation::initBuffer()
{
	this->loader->loadFrameBuffer(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, this->framebuffer, this->texColorBuffer);
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
	this->rangeKeyPressed = false;
	this->showRange = false;
	this->shootGunTower = false;
	this->shootMissileTruck = false;
	this->timeFactor = 1.0f;
	this->shot = false;
	this->skyBoxChoice = 1;
}

void Simulation::initHUD()
{
	this->textRenderer = new TextRenderer(10, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
	this->fontSize = 10;
}

void Simulation::initMatrices()
{
	this->projection = glm::mat4(1.0f);
	this->view = glm::mat4(1.0f);
}

void Simulation::initPlanes()
{
	this->planeMaster->addPlane(new Planes(glm::vec3(-100.0f, 7000.0f, -6100.0f), glm::vec3(0.001, 0.0, 1.00), 70, Colors::BLUE));
	this->planeMaster->addPlane(new Planes(glm::vec3(-100.0f, 1000.0f, -1100.0f), glm::vec3(0.001, 0.0, 1.00), 70, Colors::RED));
	this->planeMaster->addPlane(new Planes(glm::vec3(-50.0f, 1000.0f, -1100.0f), glm::vec3(0.001, 0.0, 1.00), 70, Colors::BLUE));
}

void Simulation::initTorrets()
{
	this->torretMaster->addTorrets(new Torret(glm::vec3(40.0f)));

	glm::vec2 randomPos = glm::vec2(-10, 10.0f);
	float height = this->terrain->getHeightAtPosition(randomPos.x, randomPos.y);

	this->missileTruckMaster->addMissileTrucks(new Torret(glm::vec3(randomPos.x, height + 1.5, randomPos.y)));
}

void Simulation::initGunTower()
{
	glm::vec2 randomPos = glm::vec2(10.0f, 10.0f);
	float height = this->terrain->getHeightAtPosition(randomPos.x, randomPos.y);
	this->gunTowerMaster->addGunTower(new GunTower(glm::vec3(randomPos.x, height + 1.5, randomPos.y), 15.0f));
}

//Inputhandling------------------------------------------------------------------------------

void Simulation::processInput()
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
	if (glfwGetKey(this->window, GLFW_KEY_R) == GLFW_PRESS && !this->rangeKeyPressed)
	{
		this->showRange = !this->showRange;
		this->rangeKeyPressed = true;
	}
	if (glfwGetKey(this->window, GLFW_KEY_R) == GLFW_RELEASE)
	{
		this->rangeKeyPressed = false;
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

//Updates------------------------------------------------------------------------------

void Simulation::updateSimulation()
{
	this->planeMaster->update(this->deltaTime * this->timeFactor, this->camera);
	this->missileMaster->update(this->deltaTime * this->timeFactor, this->camera, this->planeMaster->getPlanes());
	this->collisionMaster->updateMissileCollision(this->planeMaster, this->missileMaster);
	this->collisionMaster->updateS400Collision(this->planeMaster, this->s400Master);
	this->collisionMaster->updateBulletCollision(this->planeMaster, this->bulletMaster);
	this->missilesSelfDestruct += this->collisionMaster->updateMMCollision(this->missileMaster);
	this->missilesSelfDestruct += this->collisionMaster->updateSSCollision(this->s400Master);
	this->torretMaster->update(this->deltaTime * this->timeFactor, this->camera, this->planeMaster->getPlanes(), this->missileMaster);
	this->s400Master->update(this->deltaTime * this->timeFactor, this->camera, this->planeMaster->getPlanes());
	this->missileTruckMaster->update(this->deltaTime * this->timeFactor, this->camera, this->planeMaster->getPlanes(), this->s400Master, this->shootMissileTruck);
	this->gunTowerMaster->update(this->deltaTime * this->timeFactor, this->camera, this->planeMaster->getPlanes(), this->bulletMaster, this->shootGunTower);
	this->bulletMaster->update(this->deltaTime * this->timeFactor, this->camera);
	//this->skybox->updateSkybox(this->skyBoxChoice) //Noch Probleme
}

//Rendering------------------------------------------------------------------------------

void Simulation::renderSimulation()
{
	if (this->skyBoxChoice != 4) {
		this->skybox->render(this->camera, this->projection);
	}

	this->terrain->render(this->deltaTime, this->projection, this->view);
	this->planeMaster->render(this->projection, this->camera);
	this->missileMaster->render(this->projection, this->camera);
	this->collisionMaster->render(this->projection, this->camera, this->deltaTime * this->timeFactor);
	this->torretMaster->render(this->projection, this->camera, this->showRange);
	this->s400Master->render(this->projection, this->camera);
	this->missileTruckMaster->render(this->projection, this->camera);
	this->gunTowerMaster->render(this->projection, this->camera);
	this->bulletMaster->render(this->projection, this->camera);	
	glBindVertexArray(0);
}

void Simulation::renderSettings()
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

void Simulation::renderHUD()
{
	this->textRenderer->render("FPS: " + std::to_string((int)this->FPS), 0.0f, (float)this->WINDOW_HEIGHT - 1 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("Pos: " + std::to_string(this->camera.Position.x) + ", " + std::to_string(this->camera.Position.y) + ", " + std::to_string(this->camera.Position.z), 0.0f, (float)this->WINDOW_HEIGHT - 2 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("CameraView: " + std::to_string(this->camera.Front.x) + ", " + std::to_string(this->camera.Front.y) + ", " + std::to_string(this->camera.Front.z), 0.0f, (float)this->WINDOW_HEIGHT - 3 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("DeltaTime: " + std::to_string(this->deltaTime), 0.0f, (float)this->WINDOW_HEIGHT - 4 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("Start: " + std::to_string(this->start), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 1 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	
	std::string skyboxes[] = { "No Skybox", "Ocean", "Space", "Forest", "City" };
	this->textRenderer->render("Skybox: " + skyboxes[skyBoxChoice], this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 3 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("Planes: " + std::to_string(this->planeMaster->getPlanes().size()), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 4 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("Missiles: " + std::to_string(this->missileMaster->getMissiles().size()), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 5 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("MissilesSelfDestruct: " + std::to_string(this->missilesSelfDestruct), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 6 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("PlanesSelfDestruct: " + std::to_string(this->planesSelfDestruct), this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 7 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	this->textRenderer->render("Debug: " + this->debug, this->WINDOW_WIDTH / 2, (float)this->WINDOW_HEIGHT - 8 * (float)this->fontSize, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}