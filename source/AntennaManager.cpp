#include "../headers/AntennaManager.h"
#include <iostream>

AntennaManager::AntennaManager(GLFWwindow* wdw)
{
	window = wdw;
    antennaMove = -0.4;
	antennaMoving = false;
}

void AntennaManager::handleAntennaMoving()
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !antennaMoving)
        {
            if (antennaMove < -0.01)
            {
                antennaMoving = true;
                antennaMove += 0.01;
            }
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
        {
            antennaMoving = false;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            if (antennaMove > -0.4)
            {
                antennaMoving = true;
                antennaMove -= 0.01;
            }
        }
        else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
        {
            antennaMoving = false;
        }
}

float AntennaManager::getAntennaMove()
{
    return antennaMove;
}

bool AntennaManager::isAntennaMoving()
{
    return antennaMoving;
}
