#include <stb/stb_image.h>
#include <GL/glew.h>
#include <stdexcept>

#include "App/graphics/Texture.h"

Texture::Texture(const char *filepath, int slot): m_Slot(slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GLint width, height, channel;
    stbi_set_flip_vertically_on_load(true);
    auto image = stbi_load(filepath, &width, &height, &channel, 4);
    if (image != nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image);
    } else throw std::runtime_error("unable to load image at " + std::string(filepath));
}

Texture::~Texture() {
    glDeleteTexturesEXT(1, &m_TextureID);
}

void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0 + m_Slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}
