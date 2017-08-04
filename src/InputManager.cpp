//
// Created by Tony MACK on 2017/08/02.
//

#include "../inc/InputManager.hpp"

WTCEngine::InputManager::InputManager() : _mouseCoords(0){

}

WTCEngine::InputManager::~InputManager() {

}

void WTCEngine::InputManager::pressKey(unsigned int keyID) {
    _keyMap[keyID] = true;
}

void WTCEngine::InputManager::releaseKey(unsigned int keyID) {
    _keyMap[keyID] = false;
}

bool WTCEngine::InputManager::isKeyPressed(unsigned int keyID) {

    auto it = _keyMap.find(keyID);
    if (it != _keyMap.end()){
        return it->second;
    }
    else {
        return false;
    }
}

void WTCEngine::InputManager::setMouseCoords(float x, float y) {
    _mouseCoords.x = x;
    _mouseCoords.y = y;
}

glm::vec2 WTCEngine::InputManager::getMouseCoords() const{
    return _mouseCoords;
}