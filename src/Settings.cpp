#include "Settings.h"

Settings::Settings(int windowWidth, int windowHeight, GLFWwindow* window):WINDOW_WIDTH(windowWidth), WINDOW_HEIGHT(windowHeight), window(window)
{
	this->init();
}

void Settings::render()
{
    // Tell ImGUI it's a new Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (!viewMode) {
        ImGui::Begin("Steuerung", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImVec2 pos(5, 50);
        ImGui::SetWindowPos(pos);

        // Init Settingswindow
        ImVec2 size((float)WINDOW_WIDTH / 4, (float)WINDOW_HEIGHT * 0.9f);
        ImGui::SetWindowSize(size);

        // Settings
        ImGui::Text("Settings");
        ImGui::SetNextItemWidth((float)WINDOW_WIDTH / 5);
        ImGui::SliderFloat("Camspeed", &cameraSpeed, 1.0f, 600.0f);

        // Simulation control
        const char* play = "Start";
        if (start) {
            play = "Stopp";
        }
        if (ImGui::Button(play)) {
            start = !start;
        }

        ImGui::SameLine();
        const char* range = "Show Range";
        if (showRange) {
            range = "Hide Range";
        }
        if (ImGui::Button(range)) {
            showRange = !showRange;
        }

        ImGui::SameLine();
        const char* cam = camMode;
        if (camMode=="MAIN") {
            cam = "S400";
        }
        if (ImGui::Button(cam)) {
            if (camMode == "MAIN") {
                camMode = "S400";
            }
            else if (camMode == "S400") {
                camMode = "MAIN";
            }
          
        }

        ImGui::SliderFloat("Timefactor", &timeFactor, 0.0f, 2.0f);

        if (ImGui::Button("Ocean")) {
            skyBoxChoice = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Space")) {
            skyBoxChoice = 2;
        }
        ImGui::SameLine();
        if (ImGui::Button("Forest")) {
            skyBoxChoice = 3;
        }
        ImGui::SameLine();
        if (ImGui::Button("City")) {
            skyBoxChoice = 4;
        }

        ImGui::ColorPicker3("PointLightColor", (float*)&this->pointLightColor, ImGuiColorEditFlags_InputRGB);
        ImGui::SliderFloat3("PointLightPosition", (float*)&this->pointLightPos, -2000.0f, 2000.0f);
        ImGui::ColorPicker3("DirLightColor", (float*)&this->dirLightColor, ImGuiColorEditFlags_InputRGB);
        ImGui::SliderFloat3("DirLightDir", (float*)&this->dirLightDir, -1.0f, 1.0f);
        ImGui::SliderFloat("AmbientStrength", &this->ambientStrength, 0, 1);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    ImGui::EndFrame();

}

void Settings::init()
{
	//ImGUI Setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(this->window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}


bool Settings::viewMode = true;
bool Settings::start = false;
float Settings::cameraSpeed = 600.0f;
float Settings::timeFactor = 1.0f;
int Settings::skyBoxChoice = 4;
bool Settings::showRange = false;
const char* Settings::camMode = "MAIN";
float Settings::ambientStrength = 0.2;

ImVec4 Settings::dirLightColor = ImVec4(0.3, 0.3, 0.8, 1.0);
ImVec4 Settings::pointLightColor = ImVec4(0.3, 0.3, 0.8, 1.0);
float Settings::shininess = 64.f;

glm::vec3 Settings::pointLightPos = glm::vec3(1000.0f, 1000.0f, 1000.0f);
glm::vec3 Settings::dirLightDir = glm::vec3(1.0f, 1.0f, 1.0f);