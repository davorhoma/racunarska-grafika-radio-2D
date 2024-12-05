#include "../headers/slider.h"
#include <stdio.h>
#include <iostream>

Slider::Slider(int startX, int endX, int startY, int endY, unsigned int width)
{
    sliderButtonXcurrentStart = startX;
    sliderButtonXcurrentEnd = endX;
    sliderButtonYstart = startY;
    sliderButtonYend = endY;
    wWidth = width;
}

bool Slider::detectSliderMove(double xpos, double ypos, double prevXpos, bool* holdingSlider)
{
    if ((xpos >= sliderButtonXcurrentStart
        && xpos <= sliderButtonXcurrentEnd
        && ypos >= sliderButtonYstart 
        && ypos <= sliderButtonYend)
        || (*holdingSlider))
    {
        *holdingSlider = true;
        float diff = xpos - prevXpos;
        sliderButtonXcurrentStart += diff;
        sliderButtonXcurrentEnd += diff;

        float prevNormalizedX = 2.0f * static_cast<float>(prevXpos) / wWidth - 1.0f;
        float currentNormalizedX = 2.0f * static_cast<float>(xpos) / wWidth - 1.0f;

        // Calculate the difference in normalized space
        diff = currentNormalizedX - prevNormalizedX;

        // Update the slider's value, clamped within the range
        if (moveValue + diff < minSliderValue)
        {
            moveValue = minSliderValue;
        }
        else if (moveValue + diff > maxSliderValue)
        {
            moveValue = maxSliderValue;
        }
        else
        {
            moveValue += diff;
            //std::cout << "MoveValue:" << moveValue << std::endl;
        }

        if (moveValue > 0.356 || moveValue < 0)
        {
            moveValue = oldMoveValue;
            sliderButtonXcurrentStart -= diff;
            sliderButtonXcurrentEnd -= diff;
        }

        oldMoveValue = moveValue;
        return true;
    }

    return false;
}

float Slider::getMoveValue()
{
    return moveValue;
}

int Slider::getSliderButtonXcurrentEnd()
{
    return sliderButtonXcurrentEnd;
}