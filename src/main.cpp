
#include <iostream>

#include "../ZombieGame/inc/MainGame.hpp"

int main(int argc, char **argv) {
    MainGame mainGame;
    srand(time(NULL));

    mainGame.run();

    return 0;
}