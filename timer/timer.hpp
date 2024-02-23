#pragma once

#include <stdint.h>
#define NOMINMAX
#include "windows.h"


struct TimerState {
    LARGE_INTEGER freq;
    LARGE_INTEGER start;
    LARGE_INTEGER elapsed;
};


inline void timerCreate(TimerState& state) {
    QueryPerformanceFrequency(&state.freq);
}

inline void timerStart(TimerState& state) {
    QueryPerformanceCounter(&state.start);
}

inline float timerElapsed(TimerState& state) {
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    uint64_t elapsedMicrosec = ((end.QuadPart - state.start.QuadPart) * 1000000LL) / state.freq.QuadPart;
    return (float)elapsedMicrosec * .000001f;
}

inline void timerPreciseSleep(float seconds) {
    if (seconds <= .0f) {
        return;
    }
    float estimate = 5e-3f;
    float mean = 5e-3f;
    float m2 = .0f;
    int64_t count = 1;

    TimerState timer;
    timerCreate(timer);

    while (seconds > estimate) {
        timerStart(timer);
        Sleep(1);
        auto elapsed = timerElapsed(timer);
        seconds -= elapsed;

        ++count;
        float delta = elapsed - mean;
        mean += delta / count;
        m2 += delta * (elapsed - mean);
        float stddev = sqrtf(m2 / (count - 1));
        estimate = mean + stddev;
    }
    timerStart(timer);
    while (timerElapsed(timer) < seconds);
}
