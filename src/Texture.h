#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class Texture {
public:
    explicit Texture(const char* path, bool flipVertically = true);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    GLuint id() const { return textureId; }
    int width()  const { return w; }
    int height() const { return h; }

private:
    GLuint textureId = 0;
    int w = 0, h = 0;
};

#endif
