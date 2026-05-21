#include "CubeMap.h"

#include <stb/stb_image.h>

#include <vector>
#include <iostream>

namespace {

// Nearest-neighbor resample obrazka RGB (3 bajty na piksel) ze src do dst.
void resampleRGB(const unsigned char* src, int srcW, int srcH,
                 unsigned char* dst, int dstW, int dstH) {
    for (int y = 0; y < dstH; ++y) {
        int sy = (y * srcH) / dstH;
        const unsigned char* srcRow = src + sy * srcW * 3;
        unsigned char* dstRow = dst + y * dstW * 3;
        for (int x = 0; x < dstW; ++x) {
            int sx = (x * srcW) / dstW;
            const unsigned char* s = srcRow + sx * 3;
            unsigned char* d = dstRow + x * 3;
            d[0] = s[0]; d[1] = s[1]; d[2] = s[2];
        }
    }
}

} // namespace

CubeMap::CubeMap(const char* forestPath, const char* rocksPath, int faceSize) {
    stbi_set_flip_vertically_on_load(0);

    int fw, fh, fc;
    int rw, rh, rc;
    unsigned char* forest = stbi_load(forestPath, &fw, &fh, &fc, 3);
    unsigned char* rocks  = stbi_load(rocksPath,  &rw, &rh, &rc, 3);
    if (!forest || !rocks) {
        std::cout << "ERROR::CUBEMAP::FAILED_TO_LOAD  forest=" << (void*)forest
                  << " rocks=" << (void*)rocks
                  << "  reason: " << stbi_failure_reason() << std::endl;
        if (forest) stbi_image_free(forest);
        if (rocks)  stbi_image_free(rocks);
        return;
    }

    const int F = faceSize;
    const int half = F / 2;

    std::vector<unsigned char> topFace(F * F * 3);
    std::vector<unsigned char> bottomFace(F * F * 3);
    std::vector<unsigned char> sideFace(F * F * 3);

    // Gorna sciana
    resampleRGB(forest, fw, fh, topFace.data(), F, F);

    // Dolna sciana
    resampleRGB(rocks, rw, rh, bottomFace.data(), F, F);

    // Sciana boczna: gorna polowa = forest, dolna polowa = rocks
    resampleRGB(forest, fw, fh, sideFace.data(), F, half);
    resampleRGB(rocks,  rw, rh, sideFace.data() + half * F * 3, F, F - half);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB8, F, F, 0, GL_RGB, GL_UNSIGNED_BYTE, sideFace.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB8, F, F, 0, GL_RGB, GL_UNSIGNED_BYTE, sideFace.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB8, F, F, 0, GL_RGB, GL_UNSIGNED_BYTE, sideFace.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB8, F, F, 0, GL_RGB, GL_UNSIGNED_BYTE, sideFace.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB8, F, F, 0, GL_RGB, GL_UNSIGNED_BYTE, topFace.data());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB8, F, F, 0, GL_RGB, GL_UNSIGNED_BYTE, bottomFace.data());

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    stbi_image_free(forest);
    stbi_image_free(rocks);
}

CubeMap::~CubeMap() {
    if (textureId) glDeleteTextures(1, &textureId);
}
