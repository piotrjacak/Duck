#ifndef DUCK_MESH_H
#define DUCK_MESH_H

#include <glad/glad.h>

class DuckMesh {
public:
    explicit DuckMesh(const char* path);
    ~DuckMesh();

    DuckMesh(const DuckMesh&) = delete;
    DuckMesh& operator=(const DuckMesh&) = delete;

    void draw() const;

    int vertexCount()   const { return vCount; }
    int triangleCount() const { return indexCount / 3; }

private:
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    int vCount = 0;
    int indexCount = 0;
};

#endif
