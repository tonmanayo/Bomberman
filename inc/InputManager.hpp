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
        ~InputManager();

        void update();

        void pressKey(unsigned int keyID);
        void releaseKey(unsigned int keyID);

        void setMouseCoords(float x, float y);

        /// Returns true if the key is held down
        bool isKeyDown(unsigned int keyID);

        /// Returns true if the key was just pressed
        bool isKeyPressed(unsigned int keyID);

        //getters
        glm::vec2 getMouseCoords() const { return _mouseCoords; }
    private:
        /// Returns true if the key is held down
        bool wasKeyDown(unsigned int keyID);

        std::unordered_map<unsigned int, bool> _keyMap;
        std::unordered_map<unsigned int, bool> _previousKeyMap;
        glm::vec2 _mouseCoords;
    };
}


#endif //BOMBERMAN_INPUTMANAGER_HPP
