//Autor: Nedeljko Tesanovic
//Opis: Primjer upotrebe tekstura

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//stb_image.h je header-only biblioteka za ucitavanje tekstura.
//Potrebno je definisati STB_IMAGE_IMPLEMENTATION prije njenog ukljucivanja
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath); //Ucitavanje teksture, izdvojeno u funkciju
void setTextureParameters(unsigned texture, const unsigned i);
static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

enum RadioMode {
    AM = 0,
    FM = 1
};

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

    glfwSetCursorPosCallback(window, cursorPositionCallback);

    unsigned int unifiedShader = createShader("shaders/basic.vert", "shaders/basic.frag");
    unsigned int membraneShader = createShader("shaders/membrane.vert", "shaders/membrane.frag");
    unsigned int scalePointerShader = createShader("shaders/scale-pointer.vert", "shaders/scale-pointer.frag");
    unsigned int powerIndicatorShader = createShader("shaders/power-indicator.vert", "shaders/power-indicator.frag");

    float vertices[] =
    {   //X    Y      S    T 
       -0.8, -0.4,   0.0, 0.0, //drugo tjeme
        0.8, -0.4,   1.0, 0.0,//prvo tjeme
       -0.8,  0.4,   0.0, 1.0, //trece tjeme
        0.8,  0.4,   1.0, 1.0,

        // Radio coordinates
       -1.0, -1.0,   0.0, 0.0,
       -0.3, -1.0,   1.0, 0.0,
       -1.0, -0.9,   0.0, 1.0,
       -0.3, -0.9,   1.0, 1.0,

        // Signature coordinates
       -0.7, -0.35,   0.0, 0.0,
       -0.1, -0.35,   1.0, 0.0,
       -0.7,  0.25,   0.0, 1.0,
       -0.1,  0.25,   1.0, 1.0,

        // Speaker membrane coordinates
       -0.8, -0.35,   0.0, 0.0,
       -0.0, -0.35,   1.0, 0.0,
       -0.8,  0.25,   0.0, 1.0,
       -0.0,  0.25,   1.0, 1.0,

        // Protective cover coordinates
       -0.06, 0.0,    0.0, 0.0,
        0.69, 0.0,    1.0, 0.0,
       -0.06, 0.216,  0.0, 1.0,
        0.69, 0.216,  1.0, 1.0,

        // Scale pointer coordinates (x value ranges from 0.02 to 0.62)
        0.02, 0.02,   0.0, 0.0,
        0.02, 0.19,  0.0, 0.0,

        // On/Off indicator
        0.70, -0.3,   0.0, 0.0,

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
        40, 41, 42
    };
    // notacija koordinata za teksture je STPQ u GLSL-u (ali se cesto koristi UV za 2D teksture i STR za 3D)
    //ST koordinate u nizu tjemena su koordinate za teksturu i krecu se od 0 do 1, gdje je 0, 0 donji lijevi ugao teksture
    //Npr. drugi red u nizu tjemena ce da mapira boje donjeg lijevog ugla teksture na drugo tjeme
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
        "res/mode-fm.png"
    };

    unsigned radioTexture = loadImageToTexture(texturePaths[0]);
    setTextureParameters(radioTexture, 0);
    unsigned signatureTexture = loadImageToTexture(texturePaths[1]);
    setTextureParameters(signatureTexture, 1);
    unsigned membraneTexture = loadImageToTexture(texturePaths[2]);
    setTextureParameters(membraneTexture, 2);
    unsigned coverTexture = loadImageToTexture(texturePaths[3]);
    setTextureParameters(coverTexture, 3);
    unsigned scaleTexture = loadImageToTexture(texturePaths[4]);
    setTextureParameters(scaleTexture, 4);
    unsigned radioOnTexture = loadImageToTexture(texturePaths[5]);
    setTextureParameters(radioOnTexture, 5);
    unsigned radioOffTexture = loadImageToTexture(texturePaths[6]);
    setTextureParameters(radioOffTexture, 6);
    unsigned displayScreenTexture = loadImageToTexture(texturePaths[7]);
    setTextureParameters(displayScreenTexture, 7);
    unsigned sliderBarTexture = loadImageToTexture(texturePaths[8]);
    setTextureParameters(sliderBarTexture, 8);
    unsigned sliderButtonTexture = loadImageToTexture(texturePaths[9]);
    setTextureParameters(sliderButtonTexture, 9);
    unsigned modeAMTexture = loadImageToTexture(texturePaths[10]);
    setTextureParameters(modeAMTexture, 10);
    unsigned modeFMTexture = loadImageToTexture(texturePaths[11]);
    setTextureParameters(modeFMTexture, 11);

    glUseProgram(unifiedShader);
    unsigned uTexLoc = glGetUniformLocation(unifiedShader, "uTex");
    glUniform1i(uTexLoc, 0); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)
    unsigned uTexLoc1 = glGetUniformLocation(unifiedShader, "uTex1");
    glUniform1i(uTexLoc1, 1);
    //unsigned uTexLoc2 = glGetUniformLocation(unifiedShader, "uTex2");
    //glUniform1i(uTexLoc2, 2);

    glUseProgram(membraneShader);
    unsigned uMembraneTexLoc = glGetUniformLocation(membraneShader, "uMembraneTex");
    glUniform1i(uMembraneTexLoc, 0);
    unsigned uCoverTexLoc = glGetUniformLocation(membraneShader, "uCoverTex");
    glUniform1i(uCoverTexLoc, 1);

    glUseProgram(scalePointerShader);
    unsigned uScalePosLoc = glGetUniformLocation(scalePointerShader, "uScalePos");

    glUseProgram(0);
    //Odnosi se na glActiveTexture(GL_TEXTURE0) u render petlji
    //Moguce je sabirati indekse, tj GL_TEXTURE5 se moze dobiti sa GL_TEXTURE0 + 5 , sto je korisno za iteriranje kroz petlje

    glfwSetScrollCallback(window, scrollCallback);

    float whiteColor[3] = {1.0f, 1.0f, 1.0f};
    float orangeColor[3] = {1.0f, 0.5f, 0.0f};
    float currentColor[3];
    float time = 0.0f;
    float speed = 2.0f;
    unsigned vibrationIntensity = 0;
    bool upPressed = false, downPressed = false;
    //float scalePos = 0;
    float lastTime = 0.0f;
    bool isTurnedOn = false, wasMousePressed = false;
    double xpos, ypos;
    RadioMode mode = RadioMode::AM;
    while (!glfwWindowShouldClose(window))
    {
        int mouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (mouseState == GLFW_PRESS)
        {
            // -0.12, -0.04
            // 1165, 559
            glfwGetCursorPos(window, &xpos, &ypos);
            std::cout << mode << std::endl;
            std::cout << xpos << std::endl;
            std::cout << ypos << std::endl;
            if (xpos >= 1072 && xpos <= 1165 && ypos >= 520 && ypos <= 559)
            {
                mode = RadioMode::AM;
                std::cout << mode << std::endl;
            }
            if (xpos >= 1165 && xpos <= 1252 && ypos >= 520 && ypos <= 559)
            {
                mode = RadioMode::FM;
                std::cout << mode << std::endl;
            }
        }

        if (mouseState == GLFW_PRESS && !wasMousePressed)
        {
            glfwGetCursorPos(window, &xpos, &ypos);
            if (xpos > 1212 && xpos < 1247 && ypos < 653 && ypos > 612)
            {
                isTurnedOn = !isTurnedOn;
                wasMousePressed = true;
            }
        }
        else if (mouseState == GLFW_RELEASE)
        {
            wasMousePressed = false;
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !upPressed)
        {
            upPressed = true;
            if (vibrationIntensity < 100)
                vibrationIntensity = vibrationIntensity + 10;
                std::cout << vibrationIntensity << std::endl;
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
        {
            upPressed = false;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !downPressed)
        {
            downPressed = true;
            if (vibrationIntensity >= 10)
            {
                vibrationIntensity = vibrationIntensity - 10;
                std::cout << vibrationIntensity << std::endl;
            }
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
        {
            downPressed = false;
        }
        /*if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            float currentTime = glfwGetTime();
            if (currentTime - lastTime > 0.02)
            {
                if (scalePos >= 0.001)
                {
                    scalePos -= 0.01;
                }
                lastTime = currentTime;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            float currentTime = glfwGetTime();
            if (currentTime - lastTime > 0.02)
            {
                if (scalePos < 0.585)
                {
                    scalePos += 0.01;
                }
                lastTime = currentTime;
            }
        }*/
        
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(unifiedShader);
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0); //tekstura koja se bind-uje nakon ovoga ce se koristiti sa SAMPLER2D uniformom u sejderu koja odgovara njenom indeksu
        glBindTexture(GL_TEXTURE_2D, radioTexture);

        unsigned uTime = glGetUniformLocation(unifiedShader, "uTime");
        glUniform1f(uTime, glfwGetTime());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindTexture(GL_TEXTURE_2D, signatureTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));

        glUseProgram(membraneShader);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, membraneTexture);
        unsigned uVibrationIntensity = glGetUniformLocation(membraneShader, "uVibrationIntensity");
        glUniform1f(uVibrationIntensity, sin(vibrationIntensity * glfwGetTime()));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));

        glUseProgram(unifiedShader);
        glActiveTexture(GL_TEXTURE0);
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
        if (!isTurnedOn)
        {
            currentColor[0] = currentColor[1] = currentColor[2] = 0.0;
        }
        else 
        {
            float factor = (sin(glfwGetTime() * speed) + 1.0f) / 2.0f;
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
        if (isTurnedOn)
            glBindTexture(GL_TEXTURE_2D, radioOnTexture);
        else
            glBindTexture(GL_TEXTURE_2D, radioOffTexture);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(33 * sizeof(unsigned int)));

        glBindTexture(GL_TEXTURE_2D, displayScreenTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(39 * sizeof(unsigned int)));

        glBindTexture(GL_TEXTURE_2D, sliderBarTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(45 * sizeof(unsigned int)));

        glBindTexture(GL_TEXTURE_2D, sliderButtonTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(51 * sizeof(unsigned int)));

        if (mode == RadioMode::AM)
            glBindTexture(GL_TEXTURE_2D, modeAMTexture);
        else
            glBindTexture(GL_TEXTURE_2D, modeFMTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(57 * sizeof(unsigned int)));

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteTextures(1, &radioTexture);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(unifiedShader);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);
    
    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 2: InternalFormat = GL_RG; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}

void setTextureParameters(unsigned texture, const unsigned i)
{
    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
    glBindTexture(GL_TEXTURE_2D, i);
}

static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "Cursor position: ";
    std::cout << (xpos - 750) / 750;
    std::cout << ", ";
    std::cout << (500 - ypos) / 500 << std::endl;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Update the line's horizontal position based on the scroll direction
    if (yoffset > 0 && scalePos < 0.585) // Scrolled up
        scalePos += 0.01f; // Move right
    else if (yoffset < 0 && scalePos >= 0.001) // Scrolled down
        scalePos -= 0.01f; // Move left

    std::cout << "Line Offset X: " << scalePos << std::endl;
}