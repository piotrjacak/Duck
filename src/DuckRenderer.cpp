#include "DuckRenderer.h"
#include "Camera.h"

DuckRenderer::DuckRenderer(const char* meshPath, const char* texPath)
    : mesh(meshPath),
      texture(texPath, /*flipVertically=*/false),
      shader("shaders/duck.vs", "shaders/duck.fs")
{}

void DuckRenderer::draw(const Camera& camera, float aspect,
                        const glm::mat4& model, const glm::vec3& lightPos) {
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", camera.view());
    shader.setMat4("projection", camera.projection(aspect));
    shader.setVec3("viewPos", camera.position());
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("lightColor", glm::vec3(1.0f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id());
    shader.setInt("uTexture", 0);

    mesh.draw();
}
