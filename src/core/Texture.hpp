#pragma once

#include <string>
#include <memory>

#include "gl_inc.h"

class Texture {
public:
    Texture();
    ~Texture();

    /**
     * @brief Load texture from raw data.
     */
    static std::shared_ptr<Texture> fromData(
        const unsigned char* data,
        unsigned int width,
        unsigned int height,
        GLuint filter = GL_LINEAR,
        GLenum format = GL_RGBA,
        GLenum type = GL_UNSIGNED_INT
    );

    /**
     * @brief Read texture data from file.
     */
    static std::shared_ptr<Texture> fromFile(
        std::string filename, GLuint filter = GL_LINEAR);


    int getWidth();
    int getHeight();
    GLuint getId();

private:
    GLuint id;

    unsigned int width;
    unsigned int height;

    void bufferData(
        const unsigned char* data,
        GLuint filter,
        GLenum format,
        GLenum type
    );
};
