#ifndef SKYBOX_RENDERER_H
#define SKYBOX_RENDERER_H

#include <glad/glad.h>

#include "Shader.h"

class Camera;

class SkyboxRenderer {
public:
    explicit SkyboxRenderer(GLuint cubeMapId);
    ~SkyboxRenderer();

    SkyboxRenderer(const SkyboxRenderer&) = delete;
    SkyboxRenderer& operator=(const SkyboxRenderer&) = delete;

    void draw(const Camera& camera, float aspect);

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint cubeMapId;
    Shader shader;
};

#endif
