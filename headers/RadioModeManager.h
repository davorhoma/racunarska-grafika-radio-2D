#ifndef RADIO_MODE_MANAGER_H
#define RADIO_MODE_MANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum RadioMode {
    AM = 0,
    FM = 1
};

class RadioModeManager
{
private:
	RadioMode mode;
    GLFWwindow* window;
    bool isTurnedOn = false;
    bool wasMousePressed = false;
    bool holdingSlider = false;
public:
    RadioModeManager(GLFWwindow* window);
    void setRadioMode(double xpos, double ypos);
    RadioMode getMode();
    bool manageIsTurnedOn(double xpos, double ypos);
    bool getIsTurnedOn();
};

#endif