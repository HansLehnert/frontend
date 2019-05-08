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
    std::shared_ptr<Texture> texture = std::make_shared<Texture>();

    SDL_Surface* surface = IMG_Load(filename.c_str());

    if (surface != nullptr) {
        //convert to RGBA
        SDL_Surface* surface_rgba;
        surface_rgba = SDL_ConvertSurfaceFormat(
            surface, SDL_PIXELFORMAT_ABGR8888, 0);

        texture->width = surface->w;
        texture->height = surface->h;
        texture->channels = 4;
        texture->bufferData((unsigned char*)surface_rgba->pixels, filter);

        SDL_FreeSurface(surface_rgba);
        SDL_FreeSurface(surface);
    }
    else {
        std::cout << "[Texture]\tFailed to load " << filename << std::endl;
    }

    return texture;
}


std::shared_ptr<Texture> Texture::fromData(
        const unsigned char* data,
        unsigned int width,
        unsigned int height,
        unsigned int channels,
        GLuint filter
) {
    std::shared_ptr<Texture> texture = std::make_shared<Texture>();

    texture->width = width;
    texture->height = height;
    texture->channels = channels;
    texture->bufferData(data, filter);

    return texture;
}


void Texture::bufferData(const unsigned char* data, GLuint filter) {
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
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );
}


GLuint Texture::getId() {
    return id;
}


int Texture::getWidth() {
    return width;
}


int Texture::getHeight() {
    return height;
}
