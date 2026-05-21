#include "WaterRenderer.h"
#include "Camera.h"

WaterRenderer::WaterRenderer(int normalTexSize, GLuint cubeMapId)
    : cubeMapId(cubeMapId),
      shader("shaders/water.vs", "shaders/water.fs")
{
    glGenTextures(1, &normalTexture);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, normalTexSize, normalTexSize, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);


    float vertices[] = {
        // position             // uv
        -1.0f, 0.0f, -1.0f,     0.0f, 0.0f,
         1.0f, 0.0f, -1.0f,     1.0f, 0.0f,
         1.0f, 0.0f,  1.0f,     1.0f, 1.0f,
        -1.0f, 0.0f,  1.0f,     0.0f, 1.0f,
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

WaterRenderer::~WaterRenderer() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
    if (normalTexture) glDeleteTextures(1, &normalTexture);
}

void WaterRenderer::draw(const Camera& camera, float aspect) {
    shader.use();

    glm::mat4 model(1.0f);
    glm::mat4 view = camera.view();
    glm::mat4 projection = camera.projection(aspect);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("viewPos", camera.position());
    shader.setVec3("cubeMin", glm::vec3(-1.0f));
    shader.setVec3("cubeMax", glm::vec3( 1.0f));
    shader.setFloat("eta", 1.0f / 1.33f);
    shader.setFloat("F0", 0.02f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    shader.setInt("uNormalMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapId);
    shader.setInt("uCubemap", 1);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
