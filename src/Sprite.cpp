//
// Created by Tony Mack on 2017/07/28.
//

#include <cstddef>
#include "../inc/Sprite.hpp"
#include "../inc/ResourceManager.hpp"

Sprite::Sprite() : _vboID(0){


}

Sprite::~Sprite() {

}

void Sprite::draw() {
    glBindTexture(GL_TEXTURE_2D, _texture.id);
    glBindBuffer(GL_ARRAY_BUFFER, _vboID);
    glEnableVertexAttribArray(0);

    //position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

    glDisableVertexAttribArray(0);

}

void Sprite::init(float x, float y, float width, float height, std::string texturePath) {
    _x = x;
    _y = y;
    _width = width;
    _height = height;

    _texture = ResourceManager::getTexture(texturePath);

    if (_vboID == 0) {
        glGenBuffers(1, &_vboID);
    }

    Vertex vertexData[6];
    //first triangle
    vertexData[0].setPosition(x + width, y + height);
    vertexData[0].setUV(1.0f, 1.0f);

    vertexData[1].setPosition(x, y + height);
    vertexData[1].setUV(0.0f, 1.0f);

    vertexData[2].setPosition(x, y);
    vertexData[2].setUV(0.0f, 0.0f);
    //second triangle
    vertexData[3].setPosition(x, y);
    vertexData[3].setUV(0.0f, 0.0f);

    vertexData[4].setPosition(x + width, y);
    vertexData[4].setUV(1.0f, 0.0f);

    vertexData[5].setPosition(x + width, y + width);
    vertexData[5].setUV(1.0f, 1.0f);

    for (int i = 0; i < 6; ++i) {
        vertexData[i].setColor(255, 0, 255, 255);
    }

    vertexData[1].setColor(255, 255, 0, 255);
    vertexData[4].setColor(0, 255, 255, 255);



    glBindBuffer(GL_ARRAY_BUFFER, _vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}
