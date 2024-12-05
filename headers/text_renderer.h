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
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;         // Size of glyph
    glm::ivec2 Bearing;      // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Horizontal offset to advance to next glyph
    int offset;
};

class TextRenderer {
public:
    // Constructor
    TextRenderer(int screenWidth, int screenHeight);

    // Initialize FreeType and load the font
    bool LoadFont(const std::string& fontPath, int fontSize);

    // Render a string of text
    void RenderText(unsigned int shader, const std::string& text, float x, float y, float scale, glm::vec3 color);

    void UpdateTextPosition();
private:
    std::map<char, Character> Characters; // Map of characters to their data
    unsigned int VAO, VBO;                // Vertex Array Object and Vertex Buffer Object
    int screenWidth, screenHeight;        // Screen dimensions
    double lastUpdateTime;
    float speed = 100.0f;
    float textOffset = 0.0f;
    int timesShifted = 0;
    const float shiftValue = 323.0f;
};

#endif
