#include "core/Texture.hpp"

#include <iostream>
#include <cstring>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>


Texture::Texture() {
    glGenTextures(1, &id);
}


Texture::~Texture() {
    glDeleteTextures(1, &id);
}


std::shared_ptr<Texture> Texture::fromFile(
        std::string filename, GLuint filter) {

    SDL_Surface* surface = IMG_Load(filename.c_str());

    if (surface != nullptr) {
        //convert to RGBA
        SDL_Surface* surface_rgba;
        surface_rgba = SDL_ConvertSurfaceFormat(
            surface, SDL_PIXELFORMAT_ABGR8888, 0);

        std::shared_ptr<Texture> texture = Texture::fromData(
            (unsigned char*)surface_rgba->pixels,
            surface->w,
            surface->h,
            filter
        );

        SDL_FreeSurface(surface_rgba);
        SDL_FreeSurface(surface);

        return texture;
    }
    else {
        std::cout << "[Texture]\tFailed to load " << filename << std::endl;

        // If texture fails to return a 1x1 blank texture
        unsigned char data[4] = {0};
        return Texture::fromData(data, 1, 1);
    }
}


std::shared_ptr<Texture> Texture::fromData(
        const void* data,
        unsigned int width,
        unsigned int height,
        GLuint filter,
        GLenum format,
        GLenum type
) {
    std::shared_ptr<Texture> texture = std::make_shared<Texture>();

    texture->width = width;
    texture->height = height;
    texture->bufferData(data, filter, format, type);

    return texture;
}


void Texture::bufferData(
        const void* data,
        GLuint filter,
        GLenum format,
        GLenum type
) {
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        format,
        type,
        data
    );
}


GLuint Texture::getId() {
    return id;
}


int Texture::getWidth() {
    return width;
}


float Texture::getAspectRatio() {
    return float(width) / height;
}


int Texture::getHeight() {
    return height;
}
