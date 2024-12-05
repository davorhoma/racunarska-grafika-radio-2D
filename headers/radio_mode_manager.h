#ifndef RADIO_MODE_MANAGER_H
#define RADIO_MODE_MANAGER_H

enum RadioMode {
    AM = 0,
    FM = 1
};

class RadioModeManager
{
private:
	RadioMode mode;
public:
    RadioModeManager();
    void setRadioMode(double xpos, double ypos);
    RadioMode getMode();
};

#endif