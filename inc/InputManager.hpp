//
// Created by Tony MACK on 2017/08/02.
//

#ifndef BOMBERMAN_INPUTMANAGER_HPP
#define BOMBERMAN_INPUTMANAGER_HPP

#include <unordered_map>
#include <glm/vec2.hpp>

namespace WTCEngine {
    class InputManager {
    public:
        InputManager();
        virtual ~InputManager();
        void pressKey(unsigned int keyID);
        void releaseKey(unsigned int keyID);
        bool isKeyPressed(unsigned int keyID);
        void setMouseCoords(float x, float y);

        glm::vec2 getMouseCoords() const;

    private:
    std::unordered_map<unsigned int, bool> _keyMap;
        glm::vec2   _mouseCoords;

    };
}


#endif //BOMBERMAN_INPUTMANAGER_HPP
