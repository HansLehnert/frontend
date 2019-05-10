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
        unsigned int channels = 4,
        GLuint filter = GL_LINEAR
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
    unsigned int channels;

    void bufferData(const unsigned char* data, GLuint filter);
};
