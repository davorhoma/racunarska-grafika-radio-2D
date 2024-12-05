#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <thread>
#include <chrono>
#include "headers/FrameLimitter.h"
#include "headers/Slider.h"
#include "headers/TextRenderer.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "headers/RadioModeManager.h"
#include "headers/ShaderProgram.h"
#include "headers/TextureLoader.h"
#include "headers/RadioStationProgram.h"
#include "headers/AntennaManager.h"

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

float scalePos = 0.0;

int main(void)
{

    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 1500;
    unsigned int wHeight = 1000;
    const char wTitle[] = "[Project Radio]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    unsigned int unifiedShader = createShader("shaders/basic.vert", "shaders/basic.frag");
    unsigned int membraneShader = createShader("shaders/membrane.vert", "shaders/membrane.frag");
    unsigned int scalePointerShader = createShader("shaders/scale-pointer.vert", "shaders/scale-pointer.frag");
    unsigned int powerIndicatorShader = createShader("shaders/power-indicator.vert", "shaders/power-indicator.frag");
    unsigned int sliderButtonShader = createShader("shaders/slider-button.vert", "shaders/slider-button.frag");
    unsigned int antennaShader = createShader("shaders/antenna.vert", "shaders/antenna.frag");
    unsigned int glyphShader = createShader("shaders/glyph.vert", "shaders/glyph.frag");
    unsigned int soundProgressBarShader = createShader("shaders/sound-progress-bar.vert", "shaders/sound-progress-bar.frag");

    float vertices[] =
    {   //X    Y      S    T 
        // Radio coordinates
       -0.8, -0.4,   0.0, 0.0, //drugo tjeme
        0.8, -0.4,   1.0, 0.0,//prvo tjeme
       -0.8,  0.4,   0.0, 1.0, //trece tjeme
        0.8,  0.4,   1.0, 1.0,

        // Signature coordinates
       -1.0, -1.0,   0.0, 0.0,
       -0.3, -1.0,   1.0, 0.0,
       -1.0, -0.9,   0.0, 1.0,
       -0.3, -0.9,   1.0, 1.0,

        // Speaker membrane coordinates
       -0.7, -0.35,   0.0, 0.0,
       -0.1, -0.35,   1.0, 0.0,
       -0.7,  0.25,   0.0, 1.0,
       -0.1,  0.25,   1.0, 1.0,

        // Protective cover coordinates
       -0.8, -0.35,   0.0, 0.0,
       -0.0, -0.35,   1.0, 0.0,
       -0.8,  0.25,   0.0, 1.0,
       -0.0,  0.25,   1.0, 1.0,

       // Scale coordinates
       -0.06, 0.0,    0.0, 0.0,
        0.69, 0.0,    1.0, 0.0,
       -0.06, 0.216,  0.0, 1.0,
        0.69, 0.216,  1.0, 1.0,

        // Scale pointer coordinates (x value ranges from 0.02 to 0.62)
        0.02, 0.02,   0.0, 0.0,
        0.02, 0.19,  0.0, 0.0,

        // On/Off indicator
        0.70, -0.3,   0.0, 0.0,

        // On/Off button
        0.6,  -0.33,   0.0, 0.0,
        0.68, -0.33,   1.0, 0.0,
        0.6,  -0.2,   0.0, 1.0,
        0.68, -0.2,   1.0, 1.0,

        // Display screen
       -0.10, -0.35,   0.0, 0.0,
        0.40, -0.35,   1.0, 0.0,
       -0.10,  0.08,   0.0, 1.0,
        0.40,  0.08,   1.0, 1.0,

        // Slider bar
       -0.05, -0.3,    0.0, 1.0,
        0.35, -0.3,    1.0, 1.0,
       -0.05, -0.4 ,   0.0, 0.8,
        0.35, -0.4 ,   1.0, 0.8,

        // Slider button
       -0.05, -0.28,    0.0, 1.0,
        0.0 , -0.28,    0.2, 1.0,
       -0.05, -0.38 ,   0.0, 0.8,
        0.0 , -0.38 ,   0.2, 0.8,

        // Mode AM/FM
        0.43, -0.12,     0.0, 0.0,
        0.67, -0.12,     1.0, 0.0,
        0.43, -0.04,     0.0, 1.0,
        0.67, -0.04,     1.0, 1.0,

        // Antenna
        0.65, 0.3,        0.0, 0.0,
        0.70, 0.3,        1.0, 0.0,
        0.65, 0.8,        0.0, 1.0,
        0.70, 0.8,        1.0, 1.0,

        // Sound progress bar
        0.02, -0.20,      0.0, 0.0,
        0.34 , -0.20,      1.0, 0.0,
        0.02, -0.14,      0.0, 1.0,
        0.34 , -0.14,      1.0, 1.0,

        // Sound on/off icon
       -0.04, -0.22,      0.0, 0.0,
        0.00, -0.22,      1.0, 0.0,
       -0.04, -0.15,      0.0, 1.0,
        0.00, -0.15,      1.0, 1.0
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3,

        4, 5, 6,
        5, 6, 7,

        8, 9, 10,
        9, 10, 11,

        12, 13, 14,
        13, 14, 15,

        16, 17, 18,
        17, 18, 19,

        20, 21,

        22,

        23, 24, 25,
        24, 25, 26,

        27, 28, 29,
        28, 29, 30,

        31, 32, 33,
        32, 33, 34,

        35, 36, 37,
        36, 37, 38,

        39, 40, 41,
        40, 41, 42,

        43, 44, 45,
        44, 45, 46,

        47, 48, 49,
        48, 49, 50,

        51, 52, 53,
        52, 53, 54
    };
    unsigned int stride = (2 + 2) * sizeof(float);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
   
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const char* texturePaths[] = {
        "res/radio-cropped.png",
        "res/signature-text.png",
        "res/speaker-membrane.png",
        "res/protective-cover2.png",
        "res/am-fm-scale.png",
        "res/radio-on.png",
        "res/radio-off.png",
        "res/display-screen.png",
        "res/slider-bar.png",
        "res/slider-button.png",
        "res/mode-am.png",
        "res/mode-fm.png",
        "res/antenna-static.png",
        "res/antenna-dynamic.png",
        "res/sound-progress-bar.png",
        "res/sound-on-icon.png",
        "res/sound-off-icon.png"
    };

    unsigned radioTexture = loadImageToTexture(texturePaths[0], 0);
    unsigned signatureTexture = loadImageToTexture(texturePaths[1], 1);
    unsigned membraneTexture = loadImageToTexture(texturePaths[2], 2);
    unsigned coverTexture = loadImageToTexture(texturePaths[3], 3);
    unsigned scaleTexture = loadImageToTexture(texturePaths[4], 4);
    unsigned radioOnTexture = loadImageToTexture(texturePaths[5], 5);
    unsigned radioOffTexture = loadImageToTexture(texturePaths[6], 6);
    unsigned displayScreenTexture = loadImageToTexture(texturePaths[7], 7);
    unsigned sliderBarTexture = loadImageToTexture(texturePaths[8], 8);
    unsigned sliderButtonTexture = loadImageToTexture(texturePaths[9], 9);
    unsigned modeAMTexture = loadImageToTexture(texturePaths[10], 10);
    unsigned modeFMTexture = loadImageToTexture(texturePaths[11], 11);
    unsigned antennaStaticTexture = loadImageToTexture(texturePaths[12], 12);
    unsigned antennaDynamicTexture = loadImageToTexture(texturePaths[13], 13);
    unsigned soundProgressBarTexture = loadImageToTexture(texturePaths[14], 14);
    unsigned soundOnIconTexture = loadImageToTexture(texturePaths[15], 15);
    unsigned soundOffIconTexture = loadImageToTexture(texturePaths[16], 16);

    glUseProgram(membraneShader);
    unsigned uMembraneTexLoc = glGetUniformLocation(membraneShader, "uMembraneTex");
    glUniform1i(uMembraneTexLoc, 0);
    unsigned uCoverTexLoc = glGetUniformLocation(membraneShader, "uCoverTex");
    glUniform1i(uCoverTexLoc, 1);

    unsigned uScalePosLoc = glGetUniformLocation(scalePointerShader, "uScalePos");
    unsigned uSliderPosition = glGetUniformLocation(sliderButtonShader, "uSlider");    
    unsigned uAntennaPosition = glGetUniformLocation(antennaShader, "uAntennaPosition");

    glUseProgram(0);

    glfwSetScrollCallback(window, scrollCallback);

    float whiteColor[3] = {1.0f, 1.0f, 1.0f};
    float orangeColor[3] = {1.0f, 0.5f, 0.0f};
    float currentColor[3];
    float powerIndicatiorSpeed = 2.0f;
    bool upPressed = false, downPressed = false;
    bool isTurnedOn = false, wasMousePressed = false;
    double xpos, ypos;
    int sliderButtonYstart = 640;
    int sliderButtonYend = 663;
    int sliderButtonXcurrentStart = 712;
    int sliderButtonXcurrentEnd = 745;
    float oldMoveValue = 0.0, moveValue = 0.0;

    double prevXpos = 0.0;
    bool firstFrame = true;
    const float minSliderValue = -1.0f;
    const float maxSliderValue = 1.0f;
    bool holdingSlider = false;

    float antennaMove = -0.4;
    bool antennaMoving = false;
    bool shouldScanStations = false;
    int currentRadioStation = -1;
    RadioModeManager radioModeManager = RadioModeManager(window);
    RadioStationProgram radioStationProgram;
    AntennaManager antennaManager = AntennaManager(window);

    const double FRAME_DURATION = 1.0 / 60.0;
    double lastTime = glfwGetTime();

    Slider slider = Slider(sliderButtonXcurrentStart, sliderButtonXcurrentEnd, sliderButtonYstart, sliderButtonYend, wWidth);
    TextRenderer textRenderer = TextRenderer(wWidth, wHeight);
    textRenderer.LoadFont("res/fonts/arial.ttf", 25);
    glUseProgram(glyphShader);
    glm::mat4 projection = glm::ortho(0.0f, (float)wWidth, 0.0f, (float)wHeight);
    GLuint projectionLoc = glGetUniformLocation(glyphShader, "projection");
    glUniform1f(glGetUniformLocation(glyphShader, "xOffset"), 400.0f);

    if (projectionLoc != -1) {
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    } else {
        std::cerr << "Projection uniform not found in shader!" << std::endl;
    }

    unsigned int soundProgressBarMaxPosLoc = glGetUniformLocation(soundProgressBarShader, "maxPos");

    while (!glfwWindowShouldClose(window))
    {
        limitFrameRate(FRAME_DURATION, &lastTime);

        int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        glfwGetCursorPos(window, &xpos, &ypos);

        if (firstFrame)
        {
            prevXpos = xpos;
            firstFrame = false;
        }

        if (mouseState == GLFW_PRESS)
        {
            glfwGetCursorPos(window, &xpos, &ypos);
            if (!holdingSlider)
            {
                radioModeManager.setRadioMode(xpos, ypos);
            }

            glUseProgram(sliderButtonShader);
            if (slider.detectSliderMove(xpos, ypos, prevXpos, &holdingSlider))
            {
                glUniform1f(uSliderPosition, slider.getMoveValue());
            }
        }

        if (mouseState == GLFW_PRESS && !wasMousePressed)
        {
            glfwGetCursorPos(window, &xpos, &ypos);
            wasMousePressed = radioModeManager.manageIsTurnedOn(xpos, ypos);
        }
        else if (mouseState == GLFW_RELEASE)
        {
            wasMousePressed = false;
            holdingSlider = false;
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        antennaManager.handleAntennaMoving();
        radioStationProgram.setShouldScanStations(antennaManager.getAntennaMove() >= -0.2);
        radioStationProgram.scanStations(radioModeManager.getIsTurnedOn(), scalePos);
        
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(unifiedShader);
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, radioTexture);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, signatureTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));

        glUseProgram(membraneShader);
        glBindTexture(GL_TEXTURE_2D, membraneTexture);
        unsigned uVibrationIntensity = glGetUniformLocation(membraneShader, "uVibrationIntensity");
        float vibrationIntensity = 100;
        if (radioModeManager.getIsTurnedOn() && radioStationProgram.getCurrentRadioStation() != -1)
            glUniform1f(uVibrationIntensity, 2 * sin(2.0f * M_PI * slider.getMoveValue() * 10 * glfwGetTime()));
        else
            glUniform1f(uVibrationIntensity, 0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));

        glUseProgram(unifiedShader);
        glBindTexture(GL_TEXTURE_2D, coverTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(unsigned int)));

        glBindTexture(GL_TEXTURE_2D, scaleTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(unsigned int)));

        glLineWidth(5.0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(scalePointerShader);
        glUniform1f(uScalePosLoc, scalePos);
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(30 * sizeof(unsigned int)));

        glUseProgram(powerIndicatorShader);
        if (!radioModeManager.getIsTurnedOn())
        {
            currentColor[0] = currentColor[1] = currentColor[2] = 0.0;
        }
        else 
        {
            float factor = (sin(glfwGetTime() * powerIndicatiorSpeed) + 1.0f) / 2.0f;
            for (int i = 0; i < 3; ++i)
            {
                currentColor[i] = whiteColor[i] * (1.0f - factor) + orangeColor[i] * factor;
            }
        }
        int colorLoc = glGetUniformLocation(powerIndicatorShader, "dotColor");
        glUniform3fv(colorLoc, 1, currentColor);

        glPointSize(20.0f);
        glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (void*)(32 * sizeof(unsigned int)));

        glUseProgram(unifiedShader);
        if (radioModeManager.getIsTurnedOn())
            glBindTexture(GL_TEXTURE_2D, radioOnTexture);
        else
            glBindTexture(GL_TEXTURE_2D, radioOffTexture);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(33 * sizeof(unsigned int)));

        glBindTexture(GL_TEXTURE_2D, displayScreenTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(39 * sizeof(unsigned int)));

        glBindTexture(GL_TEXTURE_2D, sliderBarTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(45 * sizeof(unsigned int)));

        glUseProgram(sliderButtonShader);
        glBindTexture(GL_TEXTURE_2D, sliderButtonTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(51 * sizeof(unsigned int)));

        glUseProgram(unifiedShader);
        if (radioModeManager.getMode() == RadioMode::AM)
            glBindTexture(GL_TEXTURE_2D, modeAMTexture);
        else
            glBindTexture(GL_TEXTURE_2D, modeFMTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(57 * sizeof(unsigned int)));

        glBindTexture(GL_TEXTURE_2D, antennaStaticTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(63 * sizeof(unsigned int)));

        glUseProgram(antennaShader);
        glUniform1f(uAntennaPosition, antennaManager.getAntennaMove());
        glBindTexture(GL_TEXTURE_2D, antennaDynamicTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(63 * sizeof(unsigned int)));

        if (radioModeManager.getIsTurnedOn())
        {
            glUseProgram(soundProgressBarShader);
            int currentSliderButtonEnd = slider.getSliderButtonXcurrentEnd();
            glUniform1i(soundProgressBarMaxPosLoc, currentSliderButtonEnd);
            glBindTexture(GL_TEXTURE_2D, soundProgressBarTexture);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(69 * sizeof(unsigned int)));

            glUseProgram(unifiedShader);
            if (slider.getMoveValue() > 0.005)
                glBindTexture(GL_TEXTURE_2D, soundOnIconTexture);
            else
                glBindTexture(GL_TEXTURE_2D, soundOffIconTexture);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(75 * sizeof(unsigned int)));
        }

        if (radioStationProgram.getCurrentRadioStation() > -1)
        {
            glUseProgram(glyphShader);
            textRenderer.RenderText(glyphShader, radioStationProgram.getCurrentRadioStationName(), 1023.0f, 450.0f, 1.2f, glm::vec3(0.3f, 0.3f, 1.0f));
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        prevXpos = xpos;
    }


    glDeleteTextures(1, &radioTexture);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(unifiedShader);

    glfwTerminate();
    return 0;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset > 0 && scalePos < 0.585) // Scrolled up
        scalePos += 0.01f; // Move right
    else if (yoffset < 0 && scalePos >= 0.001) // Scrolled down
        scalePos -= 0.01f; // Move left
}
