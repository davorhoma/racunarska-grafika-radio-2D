#include "../headers/TextRenderer.h"
#include <iostream>
#include <GLFW/glfw3.h>

TextRenderer::TextRenderer(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight) {
    lastUpdateTime = glfwGetTime();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool TextRenderer::LoadFont(const std::string& fontPath, int fontSize) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        FT_Done_FreeType(ft);
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph " << c << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void TextRenderer::RenderText(unsigned int shader, const std::string& text, float x, float y, float scale, glm::vec3 color) {
    UpdateTextPosition();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLuint textColorLocation = glGetUniformLocation(shader, "textColor");
    if (textColorLocation == -1) {
        std::cout << "Warning: 'textColor' uniform not found in the shader!" << std::endl;
    }
    glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    std::string::const_iterator c;
    float textPosX = x;
    bool shifted = false;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];
        if (ch.TextureID == 0) {
            std::cout << "Error: Character texture not loaded properly!" << std::endl;
        }
        if (VAO == 0 || VBO == 0) {
            std::cout << "Error: VAO or VBO not initialized!" << std::endl;
        }

        float xpos = x + ch.offset + textOffset + timesShifted * shiftValue + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        if (xpos < 700.0f)
        {
            xpos += shiftValue;
            ch.offset += shiftValue;
        }

        float vertices[6][4] = {
            { xpos, ypos + h, 0.0f, 0.0f },
            { xpos, ypos, 0.0f, 1.0f },
            { xpos + w, ypos, 1.0f, 1.0f },

            { xpos, ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos, 1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;

        if (c == text.end() - 1 && ch.offset == shiftValue)
        {
            shifted = true;
        }
    }

    if (shifted)
    {
        timesShifted += 1;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::UpdateTextPosition() {
    double currentTime = glfwGetTime();
    float deltaTime = static_cast<float>(currentTime - lastUpdateTime);
    lastUpdateTime = currentTime;

    textOffset -= speed * deltaTime;
}
