#include "TextBox.h"

using tigl::Vertex;

TextBox::TextBox(const std::string& text, glm::vec2 position, glm::vec2 size)
{
    this->text = text;
    this->position = position;
    this->size = size;
    textColor = glm::vec4(1, 1, 1, 1);
}

void TextBox::loadFont(const std::string& fontPath)
{
    unsigned char* ttf_buffer = new unsigned char[1 << 20];
    unsigned char* temp_bitmap = new unsigned char[512 * 512];

    FILE* fontFile;
    errno_t err = fopen_s(&fontFile, fontPath.c_str(), "rb");
    if (err != 0 || !fontFile) throw "Font file not found!";

    fread(ttf_buffer, 1, 1 << 20, fontFile);
    fclose(fontFile);

    stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata);

    glGenTextures(1, &textTexture);
    glBindTexture(GL_TEXTURE_2D, textTexture);
   /* GLint swizzleMask[] = { GL_ALPHA, GL_ALPHA, GL_ALPHA, GL_ALPHA };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);*/
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    delete[] ttf_buffer;
    delete[] temp_bitmap;
}

void TextBox::draw()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Zwart transparant achtergrond vlak
    tigl::begin(GL_QUADS);
    tigl::addVertex(Vertex::PC(glm::vec3(position.x, position.y, 0), glm::vec4(0, 0, 0, 0.5))); // Boven links
    tigl::addVertex(Vertex::PC(glm::vec3(position.x + size.x, position.y, 0), glm::vec4(0, 0, 0, 0.5))); // Boven rechts
    tigl::addVertex(Vertex::PC(glm::vec3(position.x + size.x, position.y + size.y, 0), glm::vec4(0, 0, 0, 0.5))); // Onder rechts
    tigl::addVertex(Vertex::PC(glm::vec3(position.x, position.y + size.y, 0), glm::vec4(0, 0, 0, 0.5))); // Onder links
    tigl::end();

    // Tekst weergeven
    glBindTexture(GL_TEXTURE_2D, textTexture);
    tigl::shader->setColorMult(textColor);

    float x = position.x + 10;
    float y = position.y + size.y - 30;
    stbtt_aligned_quad q;

    tigl::begin(GL_QUADS);
    for (char c : text) {
        if (c >= 32 && c < 128) {
            stbtt_GetBakedQuad(cdata, 512, 512, c - 32, &x, &y, &q, 1);
            tigl::addVertex(Vertex::PT(glm::vec3(q.x0, q.y0, 0), glm::vec2(q.s0, q.t0)));
            tigl::addVertex(Vertex::PT(glm::vec3(q.x1, q.y0, 0), glm::vec2(q.s1, q.t0)));
            tigl::addVertex(Vertex::PT(glm::vec3(q.x1, q.y1, 0), glm::vec2(q.s1, q.t1)));
            tigl::addVertex(Vertex::PT(glm::vec3(q.x0, q.y1, 0), glm::vec2(q.s0, q.t1)));
        }
    }
    tigl::end();
    glEnable(GL_DEPTH_TEST);
}

void TextBox::setText(const std::string& newText) {
	text = newText;
}
