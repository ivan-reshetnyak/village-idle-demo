#pragma once

#include <cstdint>


class timer {
public:
    float
        globalTime = 0,      /// Time from program start
        globalDeltaTime = 0, /// Delta time from last frame
        time = 0,            /// Time from program start (with pause)
        deltaTime = 0,       /// Delta time from last frame (with pause)
        fps = 0;

  timer();
  void Update();
  void IncrFrameCount() { ++frameCount; }
  bool IsPaused() const { return isPaused; }
  void Pause() { isPaused = true; }
  void Unpause() { isPaused = false; }
  void TogglePause() { isPaused = !isPaused; }

private:
    int64_t
        startTime,           /// Time from program start
        oldTime,             /// Time of last frame
        lastTime,            /// Time of last FPS measure
        pauseTime = 0,       /// Total pause time
        timesPerSecond = 0,  /// Timer sensitivity
        frameCount = 0;
    bool isPaused = false;
};

