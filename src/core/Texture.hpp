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
    GLuint getId() const;

    // Get the texture width in pixels
    int width() const { return width_; }
    int height() const { return height_; };

    // Get the aspect ratio of the image as width / height
    float aspectRatio() const { return static_cast<float>(width_) / height_; }

private:
    GLuint id;

    uint64_t width_;
    uint64_t height_;

    void bufferData(const void* data, GLuint filter, GLenum format, GLenum type);
};
