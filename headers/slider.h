#ifndef CLASSSlider_H
#define CLASSSLider_H

class Slider
{
private:
    int sliderButtonYstart;
    int sliderButtonYend;
    int sliderButtonXcurrentStart;
    int sliderButtonXcurrentEnd;
    unsigned int wWidth;
    const float minSliderValue = -1.0f;
    const float maxSliderValue = 1.0f;
    float oldMoveValue = 0.0;
    float moveValue = 0.0;
public:
    Slider(int startX, int endX, int startY, int endY, unsigned int width);
    bool detectSliderMove(double xpos, double ypos, double prevXpos, bool* holdingSlider);
    float getMoveValue();
};

#endif // CLASSSlider_H