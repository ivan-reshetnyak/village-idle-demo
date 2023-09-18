#include "pch.h"

#ifndef GLEW_STATIC
#  define GLEW_STATIC
#endif
#include "glew/glew.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glew32s")

#include "texture.h"


texture::manager_t texture::manager;


texture::manager_t::ptr texture::manager_t::Get(const std::string& name)
{
    return ptr(storage.find(name)->second);
}


texture::manager_t::ptr texture::manager_t::Add(const std::string& name, storage_type pTex)
{
    if (!Exists(name)) {
        storage[name] = pTex;
    }
    return storage[name];
}


texture::texture(int samplerNum)
    : sampler(samplerNum)
{
    glGenTextures(1, &glId);
}


texture::~texture()
{
    glDeleteTextures(1, &glId);
}


void texture::Enable()
{
    if (glId != INVALID_ID) {
        // glSamplerParameterf(GL_TEXTURE0 + sampler, GL_TEXTURE_MAX_ANISOTROPY, GL_MAX_TEXTURE_MAX_ANISOTROPY);
        glActiveTexture(GL_TEXTURE0 + sampler);
        glBindTexture(GL_TEXTURE_2D, glId);
    }
}


void texture::Disable()
{
    glActiveTexture(GL_TEXTURE0 + sampler);
    glBindTexture(GL_TEXTURE_2D, INVALID_ID);
}
