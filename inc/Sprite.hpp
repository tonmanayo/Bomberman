//
// Created by Tony Mack on 2017/07/28.
//

#ifndef BOMBERMAN_SPRITE_HPP
#define BOMBERMAN_SPRITE_HPP

#include <GL/Glew.h>

class Sprite {
public:
    Sprite();
    virtual ~Sprite();
    void draw();
    void init(const unsigned int &x, const unsigned int &y, const unsigned int &width, const unsigned int &height);

private:
    unsigned int _x;
    unsigned int _y;
    unsigned int _width;
    unsigned int _height;
    GLuint _vboID;

};


#endif //BOMBERMAN_SPRITE_HPP
