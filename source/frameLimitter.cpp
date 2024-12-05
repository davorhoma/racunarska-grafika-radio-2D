#include "../headers/FrameLimitter.h"
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>

void limitFrameRate(const double FRAME_DURATION, double* lastTime)
{
	double currentTime = glfwGetTime();
    double elapsedTime = currentTime - *lastTime;

    if (elapsedTime < FRAME_DURATION) {
        // Sleep for the remaining time to achieve 60 FPS
        std::this_thread::sleep_for(std::chrono::duration<double>(FRAME_DURATION - elapsedTime));
        currentTime = glfwGetTime(); // Update currentTime after sleep
    }

    *lastTime = currentTime;
}