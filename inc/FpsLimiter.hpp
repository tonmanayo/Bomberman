//
// Created by Tony MACK on 2017/08/02.
//

#ifndef BOMBERMAN_FPSLIMITER_HPP
#define BOMBERMAN_FPSLIMITER_HPP

#include <SDL.h>

namespace WTCEngine {
    class FpsLimiter {
    public:
        FpsLimiter();

        virtual ~FpsLimiter();

        void init(float targetFPS);

        void begin();
        //return fps
        float end();

        void setMaxFPS(float targetFPS);

    private:
        float           _fps;
        float           _maxFPS;
        unsigned int    _startTicks;
        float           _frameTime;

        void            calculateFPS();

    };
}


#endif //BOMBERMAN_FPSLIMITER_HPP
