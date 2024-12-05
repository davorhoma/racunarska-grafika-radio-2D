#include "../headers/RadioStationProgram.h"

RadioStationProgram::RadioStationProgram() {}

void RadioStationProgram::scanStations(bool isTurnedOn, float scalePos)
{
	bool found = false;
    if (isTurnedOn && shouldScanStations)
    {
        int numStations = sizeof(radioStations) / sizeof(radioStations[0]);
        for (int i = 0; i < numStations; ++i) {
            float lowerBound = (2 * i + 1) * 0.0585f;
            float upperBound = (2 * i + 2) * 0.0585f;
    
            if (scalePos > radioStations[i].minFrequency && scalePos < radioStations[i].maxFrequency) {
                currentRadioStation = i;
                found = true;
                break;
            }
        }

        if (!found) {
            currentRadioStation = -1;
        }   
    }
    else
    {
        currentRadioStation = -1;
    }
}

const char* RadioStationProgram::getCurrentRadioStationName()
{
    return radioStations[currentRadioStation].name;
}

void RadioStationProgram::setShouldScanStations(bool should)
{
    shouldScanStations = should;
}

int RadioStationProgram::getCurrentRadioStation()
{
    return currentRadioStation;
}




