#pragma once

#include <string>
#include <memory>

#include "gl_inc.h"

class Texture {
public:
    Texture();
    ~Texture();

    // Load texture from raw data.
    static std::shared_ptr<Texture> fromData(
        const void* data,
        unsigned int width,
        unsigned int height,
        GLuint filter = GL_LINEAR,
        GLenum format = GL_RGBA,
        GLenum type = GL_UNSIGNED_BYTE
    );

    // Read texture data from file.
    static std::shared_ptr<Texture> fromFile(
        std::string filename, GLuint filter = GL_LINEAR);

    // Get the openGL id for the texture
    GLuint getId();

    // Get the texture width in pixels
    int getWidth();

    // Gen the texture height in pixels
    int getHeight();

    // Get the aspect ratio of the image as width / height
    float getAspectRatio();

private:
    GLuint id;

    unsigned int width;
    unsigned int height;

    void bufferData(
        const void* data,
        GLuint filter,
        GLenum format,
        GLenum type
    );
};
