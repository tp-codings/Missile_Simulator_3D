#include "RangeRenderer.h"

RangeRenderer::RangeRenderer(float radius):radiusShoot(radius)
{
	this->shader = Shader("Shader/range.vs", "Shader/range.fs");
	this->initVertices();
	this->initBuffer();
}

void RangeRenderer::update(float radius)
{
	this->radiusShoot = radius;
	initVertices();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
}

void RangeRenderer::render(glm::mat4 projection, Camera& camera)
{
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", camera.GetViewMatrix());

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);

}

void RangeRenderer::initVertices()
{
    vertices.clear();
    // Generate the vertices for the range
    // Assuming you want to generate the vertices for a half sphere similar to the previous implementation
    const int numSegments = 20;
    const int numRings = 10;

    for (int ring = 0; ring < numRings; ++ring) {
        float phi0 = glm::pi<float>() / 2.0f * (static_cast<float>(ring) / numRings);
        float phi1 = glm::pi<float>() / 2.0f * (static_cast<float>(ring + 1) / numRings);
        for (int segment = 0; segment < numSegments; ++segment) {
            float theta0 = 2.0f * glm::pi<float>() * (static_cast<float>(segment) / numSegments);
            float theta1 = 2.0f * glm::pi<float>() * (static_cast<float>(segment + 1) / numSegments);

            // Vertices for the current quad
            glm::vec3 v00(radiusShoot * glm::cos(theta0) * glm::sin(phi0),
                radiusShoot * glm::cos(phi0),  // <-- Swap y and z coordinates
                radiusShoot * glm::sin(theta0) * glm::sin(phi0)); // <-- Swap y and z coordinates

            glm::vec3 v10(radiusShoot * glm::cos(theta1) * glm::sin(phi0),
                radiusShoot * glm::cos(phi0),  // <-- Swap y and z coordinates
                radiusShoot * glm::sin(theta1) * glm::sin(phi0)); // <-- Swap y and z coordinates

            glm::vec3 v01(radiusShoot * glm::cos(theta0) * glm::sin(phi1),
                radiusShoot * glm::cos(phi1),  // <-- Swap y and z coordinates
                radiusShoot * glm::sin(theta0) * glm::sin(phi1)); // <-- Swap y and z coordinates

            glm::vec3 v11(radiusShoot * glm::cos(theta1) * glm::sin(phi1),
                radiusShoot * glm::cos(phi1),  // <-- Swap y and z coordinates
                radiusShoot * glm::sin(theta1) * glm::sin(phi1)); // <-- Swap y and z coordinates

            // Add two triangles for the quad
            vertices.push_back(v00);
            vertices.push_back(v10);
            vertices.push_back(v11);

            vertices.push_back(v00);
            vertices.push_back(v11);
            vertices.push_back(v01);
        }
    }
}


void RangeRenderer::initBuffer()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // Assuming the position attribute is at location 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

