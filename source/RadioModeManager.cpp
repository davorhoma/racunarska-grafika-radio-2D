#include "../headers/RadioModeManager.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

RadioModeManager::RadioModeManager(GLFWwindow* wdw) : mode(RadioMode::AM)
{
    window = wdw;
}

void RadioModeManager::setRadioMode(double xpos, double ypos)
{
	if (xpos >= 1072 && xpos <= 1165 && ypos >= 520 && ypos <= 559)
    {
        mode = RadioMode::AM;
    }
    if (xpos >= 1165 && xpos <= 1252 && ypos >= 520 && ypos <= 559)
    {
        mode = RadioMode::FM;
    }
}

RadioMode RadioModeManager::getMode()
{
    return mode;
}

bool RadioModeManager::manageIsTurnedOn(double xpos, double ypos)
{
    if (xpos > 1212 && xpos < 1247 && ypos < 653 && ypos > 612 && !holdingSlider)
    {
        isTurnedOn = !isTurnedOn;
        return true;
    }

    return false;
}

bool RadioModeManager::getIsTurnedOn()
{
    return isTurnedOn;
}
