#include <GLFW/glfw3.h>
#ifndef ANTENNA_MANAGER_H
#define ANTENNA_MANAGER_H

class AntennaManager
{
private:
	GLFWwindow* window;
	float antennaMove;
	bool antennaMoving;
public:
	AntennaManager(GLFWwindow* wdw);
	void handleAntennaMoving();
	float getAntennaMove();
	bool isAntennaMoving();
};

#endif
