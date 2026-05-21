#ifndef DUCK_RENDERER_H
#define DUCK_RENDERER_H

#include <glm/glm.hpp>

#include "DuckMesh.h"
#include "Texture.h"
#include "Shader.h"

class Camera;

class DuckRenderer {
public:
    DuckRenderer(const char* meshPath, const char* texPath);

    DuckRenderer(const DuckRenderer&) = delete;
    DuckRenderer& operator=(const DuckRenderer&) = delete;

    void draw(const Camera& camera, float aspect,
              const glm::mat4& model, const glm::vec3& lightPos);

private:
    DuckMesh mesh;
    Texture  texture;
    Shader   shader;
};

#endif
