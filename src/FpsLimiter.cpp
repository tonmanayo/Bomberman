//
// Created by Tony MACK on 2017/08/02.
//

#include <iostream>
#include "../inc/FpsLimiter.hpp"


namespace WTCEngine {

    FpsLimiter::FpsLimiter() {

    }

    FpsLimiter::~FpsLimiter() {

    }

    void FpsLimiter::init(float maxFPS) {
        setMaxFPS(maxFPS);
    }

    void FpsLimiter::begin() {
        _startTicks = SDL_GetTicks();
    }

    float FpsLimiter::end() {
        calculateFPS();

        float frameTicks = SDL_GetTicks() - _startTicks;
        if (1000.00f / _maxFPS > frameTicks) {
            SDL_Delay(1000.0f / _maxFPS - frameTicks);
        }
        return _fps;
    }

    void FpsLimiter::setMaxFPS(float maxFPS) {
        _maxFPS = maxFPS;
    }

    void FpsLimiter::calculateFPS() {
        static const int NUM_SAMPLES = 10;
        static float frameTimes[NUM_SAMPLES];
        static int curFrame = 0;

        static float prevTicks = SDL_GetTicks();
        float curTicks = SDL_GetTicks();

        _frameTime = curTicks - prevTicks;
        prevTicks = curTicks;
        frameTimes[curFrame % NUM_SAMPLES] = _frameTime;

        int count = 0;
        if (curFrame < NUM_SAMPLES) {
            count = curFrame;
        } else
            count = NUM_SAMPLES;
        float frameTimeAvg = 0;
        for (int i = 0; i < count; ++i) {
            frameTimeAvg += frameTimes[i];
        }
        if (count > 0)
            frameTimeAvg /= count;

        if (frameTimeAvg > 0)
            _fps = 1000.0f / frameTimeAvg;
        else
            _fps = 60;
        curFrame++;
    }
}