#include "Sun.h"

Sun::Sun()
{
	this->model = new ModelHandler(R"(resources\models\sphere\sphere.obj)");
	this->model->Scale(50.0f);
	this->shader = Shader("Shader/sun.vs", "Shader/sun.fs");

}

void Sun::render(glm::mat4 projection, glm::mat4 view)
{
	this->model->Translate(Settings::dirLightPos);
	this->model->Draw(&this->shader, projection, view);
}
