//
// Created by Tony MACK on 2017/07/31.
//

#include "../inc/Camera2D.hpp"

namespace WTCEngine {

    Camera2D::Camera2D() : _position(0.0f), _camMatrix(1.0f), _scale(1), _needsMatrixUpdate(true), _ScreenHeight(500),
                           _ScreenWidth(500), _orthoMatrix(1.0) {

    }

    Camera2D::~Camera2D() {

    }

    void Camera2D::setPosition(const glm::vec2& newPosition) {
        _needsMatrixUpdate = true;
        _position = newPosition;
    }

    glm::vec2 Camera2D::getPosition() {
        return _position;
    }

    void Camera2D::setcamMatrix(glm::mat4 camMatrix) {
        _camMatrix = camMatrix;
    }

    glm::mat4 Camera2D::getcamMatrix() {
        return _camMatrix;
    }

    void Camera2D::setScale(float scale) {
        _needsMatrixUpdate = true;
        _scale = scale;
    }

    float Camera2D::getScale() {
        return _scale;
    }

    void Camera2D::init(int screenWidth, int screenHeight) {
        _ScreenWidth = screenWidth;
        _ScreenHeight = screenHeight;
        _orthoMatrix = glm::ortho(0.0f, (float) _ScreenWidth, 0.0f, (float) _ScreenHeight);
    }

    void Camera2D::update() {
        if (_needsMatrixUpdate) {
            glm::vec3 translate(-_position.x, -_position.y, 0.0f);
            _camMatrix = glm::translate(_orthoMatrix, translate);
            glm::vec3 scale(_scale, _scale, 0.0f);
            _camMatrix = glm::translate(_camMatrix, scale);
            _needsMatrixUpdate = false;
        }
    }
}


