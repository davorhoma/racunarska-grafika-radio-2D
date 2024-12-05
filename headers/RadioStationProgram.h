#ifndef RADIO_STATION_MANAGER_H
#define RADIO_STATION_MANAGER_H

struct RadioStation {
    const char* name;
    float minFrequency;
    float maxFrequency;
};

class RadioStationProgram
{
private:
    RadioStation radioStations[5] = {
        {"Radio S", 0.0585, 2*0.0585},
        {"Play Radio", 3*0.0585, 4*0.0585},
        {"Radio AS FM", 5*0.0585, 6*0.0585},
        {"Hit FM", 7*0.0585, 8*0.0585},
        {"Naxi Radio", 9*0.0585, 10*0.0585}
    };
    bool shouldScanStations = false;
    int currentRadioStation = -1;
public:
    RadioStationProgram();
    void scanStations(bool isTurnedOn, float scalePos);
    const char* getCurrentRadioStationName();
    void setShouldScanStations(bool should);
    int getCurrentRadioStation();
};

#endif // !RADIO_STATION_MANAGER_H
