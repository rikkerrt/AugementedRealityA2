#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "tigl.h"
#include "stb_truetype.h"
#include <string>

class TextBox
{
public:
    TextBox(const std::string& text, glm::vec2 position, glm::vec2 size);
    void loadFont(const std::string& fontPath);
    void draw();
    void setText(const std::string& newText);

private:
    GLuint textTexture;
    stbtt_bakedchar cdata[96];
    glm::vec4 textColor;
    std::string text;
    glm::vec2 position;
    glm::vec2 size;
};
