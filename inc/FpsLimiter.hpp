//
// Created by Tony MACK on 2017/08/02.
//

#ifndef BOMBERMAN_FPSLIMITER_HPP
#define BOMBERMAN_FPSLIMITER_HPP


class FpsLimiter {
public:
    FpsLimiter();
    virtual ~FpsLimiter();
    void begin();
    int end();

private:
};


#endif //BOMBERMAN_FPSLIMITER_HPP
