#include "pch.h"

#include <Windows.h>
#undef min
#undef max

#include "timer.h"


timer::timer()
{
    LARGE_INTEGER CurrentTime;
    QueryPerformanceCounter(&CurrentTime);
    startTime = oldTime = lastTime = CurrentTime.QuadPart;
    QueryPerformanceFrequency(&CurrentTime);
    timesPerSecond = CurrentTime.QuadPart;
}


void timer::Update()
{
    LARGE_INTEGER CurrentTime;
    QueryPerformanceCounter(&CurrentTime);

    /* Update global time */
    globalTime = (float)(CurrentTime.QuadPart - startTime) / timesPerSecond;
    globalDeltaTime = (float)(CurrentTime.QuadPart - oldTime) / timesPerSecond;

    /* Update FPS */
    if (CurrentTime.QuadPart - lastTime > timesPerSecond) {
        fps = (float)frameCount / (CurrentTime.QuadPart - lastTime) * timesPerSecond;
        lastTime = CurrentTime.QuadPart;
        frameCount = 0;
    }

    /* Update local time */
    time = (float)(CurrentTime.QuadPart - startTime - pauseTime) / timesPerSecond;
    if (IsPaused()) {
        deltaTime = 0;
        pauseTime += CurrentTime.QuadPart - oldTime;
    } else {
        deltaTime = globalDeltaTime;
    }

    oldTime = CurrentTime.QuadPart;
}
