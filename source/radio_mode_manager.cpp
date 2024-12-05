#include "../headers/radio_mode_manager.h"

RadioModeManager::RadioModeManager() : mode(RadioMode::AM) {}

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