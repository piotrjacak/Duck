#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <glad/glad.h>

class CubeMap {
public:
    // Buduje GL_TEXTURE_CUBE_MAP z dwoch obrazkow 2D:
    CubeMap(const char* forestPath, const char* rocksPath, int faceSize = 512);
    ~CubeMap();

    CubeMap(const CubeMap&) = delete;
    CubeMap& operator=(const CubeMap&) = delete;

    GLuint id() const { return textureId; }

private:
    GLuint textureId = 0;
};

#endif
