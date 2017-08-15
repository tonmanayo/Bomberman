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
        _position = newPosition;
        _needsMatrixUpdate = true;

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
        _scale = scale;
        _needsMatrixUpdate = true;
    }

    float Camera2D::getScale() {
        return _scale;
    }

    void Camera2D::init(int screenWidth, int screenHeight) {
        _ScreenWidth = screenWidth;
        _ScreenHeight = screenHeight;
        _orthoMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);
    }

    void Camera2D::update() {
        if (_needsMatrixUpdate) {
            //translate
            glm::vec3 translate(-_position.x + _ScreenWidth / 2, -_position.y + _ScreenHeight / 2, 0.0f);
            _camMatrix = glm::translate(_orthoMatrix, translate);
            //scale
            glm::vec3 scale(_scale, _scale, 0.0f);
            _camMatrix = glm::scale(glm::mat4(1.0f), scale) * _camMatrix;

            _needsMatrixUpdate = false;
        }
    }

    glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords) {
        //invert Y
        screenCoords.y = _ScreenHeight - screenCoords.y;
        //0 is the centre
        screenCoords -= glm::vec2(_ScreenWidth / 2, _ScreenHeight / 2);
        // scale coords
        screenCoords /= _scale;
        // translate with the camera position
        screenCoords += _position;
        return screenCoords;
    }

    bool Camera2D::isBoxInView(const glm::vec2 &position, const glm::vec2 &dimensions) {
        glm::vec2 scaledScreenDimensions = glm::vec2(_ScreenWidth, _ScreenHeight) / (_scale);

        // The minimum distance before a collision occurs
        const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDimensions.x / 2.0f;
        const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDimensions.y / 2.0f;

        // Center position of the parameters
        glm::vec2 centerPos = position + dimensions / 2.0f;
        // Center position of the camera
        glm::vec2 centerCameraPos = _position;
        // Vector from the input to the camera
        glm::vec2 distVec = centerPos - centerCameraPos;

        // Get the depth of the collision
        float xDepth = MIN_DISTANCE_X - std::abs(distVec.x);
        float yDepth = MIN_DISTANCE_Y - std::abs(distVec.y);

        // If both the depths are > 0, then we collided
        if (xDepth > 0 && yDepth > 0) {
            // There was a collision
            return true;
        }
        return false;
    }


}


