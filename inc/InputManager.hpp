//
// Created by Tony MACK on 2017/08/02.
//

#ifndef BOMBERMAN_INPUTMANAGER_HPP
#define BOMBERMAN_INPUTMANAGER_HPP

#include <unordered_map>

namespace WTCEngine {
    class InputManager {
    public:
        InputManager();
        virtual ~InputManager();
        void pressKey(unsigned int keyID);
        void releaseKey(unsigned int keyID);
        bool isKeyPressed(unsigned int keyID);

    private:
    std::unordered_map<unsigned int, bool> _keyMap;

    };
}


#endif //BOMBERMAN_INPUTMANAGER_HPP
