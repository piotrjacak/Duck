#include "DuckMesh.h"

#include <fstream>
#include <iostream>
#include <vector>

DuckMesh::DuckMesh(const char* path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        std::cout << "ERROR::DUCK_MESH::FAILED_TO_OPEN: " << path << std::endl;
        return;
    }

    int V = 0;
    f >> V;
    vCount = V;
    
    std::vector<float> vertices;
    vertices.reserve(V * 8);
    for (int i = 0; i < V; ++i) {
        float px, py, pz, nx, ny, nz, u, v;
        f >> px >> py >> pz >> nx >> ny >> nz >> u >> v;
        vertices.push_back(px); vertices.push_back(py); vertices.push_back(pz);
        vertices.push_back(nx); vertices.push_back(ny); vertices.push_back(nz);
        vertices.push_back(u);  vertices.push_back(v);
    }

    int T = 0;
    f >> T;
    std::vector<unsigned int> indices;
    indices.reserve(T * 3);
    for (int i = 0; i < T; ++i) {
        unsigned int a, b, c;
        f >> a >> b >> c;
        indices.push_back(a);
        indices.push_back(b);
        indices.push_back(c);
    }
    indexCount = static_cast<int>(indices.size());

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_STATIC_DRAW);

    const GLsizei stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    std::cout << "DuckMesh loaded: " << V << " vertices, " << (indexCount / 3) << " triangles\n";
}

DuckMesh::~DuckMesh() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
}

void DuckMesh::draw() const {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
