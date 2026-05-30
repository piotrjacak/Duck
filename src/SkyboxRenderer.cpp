#include "SkyboxRenderer.h"
#include "Camera.h"

SkyboxRenderer::SkyboxRenderer(GLuint cubeMapId)
    : cubeMapId(cubeMapId),
      shader("shaders/skybox.vs", "shaders/skybox.fs")
{
    float vertices[] = {
        // +X
         1.f, -1.f, -1.f,    1.f,  1.f, -1.f,    1.f,  1.f,  1.f,
         1.f, -1.f, -1.f,    1.f,  1.f,  1.f,    1.f, -1.f,  1.f,
        // -X
        -1.f, -1.f, -1.f,   -1.f, -1.f,  1.f,   -1.f,  1.f,  1.f,
        -1.f, -1.f, -1.f,   -1.f,  1.f,  1.f,   -1.f,  1.f, -1.f,
        // +Y
        -1.f,  1.f,  1.f,    1.f,  1.f,  1.f,    1.f,  1.f, -1.f,
        -1.f,  1.f,  1.f,    1.f,  1.f, -1.f,   -1.f,  1.f, -1.f,
        // -Y
        -1.f, -1.f, -1.f,    1.f, -1.f, -1.f,    1.f, -1.f,  1.f,
        -1.f, -1.f, -1.f,    1.f, -1.f,  1.f,   -1.f, -1.f,  1.f,
        // +Z
        -1.f, -1.f,  1.f,    1.f, -1.f,  1.f,    1.f,  1.f,  1.f,
        -1.f, -1.f,  1.f,    1.f,  1.f,  1.f,   -1.f,  1.f,  1.f,
        // -Z
        -1.f, -1.f, -1.f,   -1.f,  1.f, -1.f,    1.f,  1.f, -1.f,
        -1.f, -1.f, -1.f,    1.f,  1.f, -1.f,    1.f, -1.f, -1.f,
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

SkyboxRenderer::~SkyboxRenderer() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
}

void SkyboxRenderer::draw(const Camera& camera, float aspect) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    shader.use();
    shader.setMat4("view", camera.view());
    shader.setMat4("projection", camera.projection(aspect));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);
    shader.setInt("uCubemap", 0);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDisable(GL_CULL_FACE);
}
