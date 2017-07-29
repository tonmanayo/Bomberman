//
// Created by Tony Mack on 2017/07/28.
//

#ifndef BOMBERMAN_SPRITE_HPP
#define BOMBERMAN_SPRITE_HPP

//#include <GL/glew.h>
#include <OpenGL/gl3.h>


class Sprite {
public:
    Sprite();
    virtual ~Sprite();
    void draw();
    void init(float x, float y, float width, float height);

private:
    float _x;
    float _y;
    float _width;
    float _height;
    GLuint _vboID;

};


#endif //BOMBERMAN_SPRITE_HPP
