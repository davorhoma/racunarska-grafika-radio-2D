#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <glm/glm.hpp>
#include <map>
#include <string>
#include <GL/glew.h>
#include <ft2build.h>
#include <chrono>
#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
    int offset;
};

class TextRenderer {
public:
    TextRenderer(int screenWidth, int screenHeight);
    bool LoadFont(const std::string& fontPath, int fontSize);
    void RenderText(unsigned int shader, const std::string& text, float x, float y, float scale, glm::vec3 color);
    void UpdateTextPosition();
private:
    std::map<char, Character> Characters;
    unsigned int VAO, VBO;
    int screenWidth, screenHeight;
    double lastUpdateTime;
    float speed = 100.0f;
    float textOffset = 0.0f;
    int timesShifted = 0;
    const float shiftValue = 323.0f;
};

#endif
