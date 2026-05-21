#ifndef WATER_RENDERER_H
#define WATER_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"

class Camera;

class WaterRenderer {
public:
    WaterRenderer(int normalTexSize, GLuint cubeMapId);
    ~WaterRenderer();

    WaterRenderer(const WaterRenderer&) = delete;
    WaterRenderer& operator=(const WaterRenderer&) = delete;

    void draw(const Camera& camera, float aspect);

    GLuint getNormalTexture() const { return normalTexture; }

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLuint normalTexture = 0;
    GLuint cubeMapId = 0;
    Shader shader;
};

#endif
