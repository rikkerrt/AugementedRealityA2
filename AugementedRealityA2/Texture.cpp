#include "Texture.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string & fileName)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    int width, height, bpp;

    unsigned char* imgData = stbi_load(fileName.c_str(), &width, &height, &bpp, 4);

    if (imgData)
    {
		std::cout << "Loaded texture " << fileName;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(imgData);
    }
    else
    {
        std::cout << "Failed to load texture! \n";
    }
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id);
}
