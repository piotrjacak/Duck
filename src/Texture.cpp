#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

Texture::Texture(const char* path, bool flipVertically) {
    stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

    int channels = 0;
    unsigned char* data = stbi_load(path, &w, &h, &channels, 0);
    if (!data) {
        std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD: " << path
                  << "  reason: " << stbi_failure_reason() << std::endl;
        return;
    }

    GLenum format = GL_RGB;
    GLenum internalFormat = GL_RGB8;
    if (channels == 1) { format = GL_RED;  internalFormat = GL_R8;    }
    else if (channels == 3) { format = GL_RGB;  internalFormat = GL_RGB8;  }
    else if (channels == 4) { format = GL_RGBA; internalFormat = GL_RGBA8; }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

Texture::~Texture() {
    if (textureId) glDeleteTextures(1, &textureId);
}
