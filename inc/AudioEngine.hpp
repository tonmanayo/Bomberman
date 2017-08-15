//
// Created by tonmanayo on 2017/08/06.
//

#ifndef BOMBERMAN_AUDIOENGINE_HPP
#define BOMBERMAN_AUDIOENGINE_HPP

#include <string>
#include <map>
#include <SDL_mixer.h>

namespace WTCEngine {

    class SoundEffect {
    public:
        friend class AudioEngine;

        /// Plays the effect file
        /// @param loops: If loops == -1, loop forever,
        /// otherwise play it loops+1 times
        void play(int loops = 0);
    private:
        Mix_Chunk* _chunk = nullptr;
    };

    class Music {
    public:
        friend class AudioEngine;

        /// Plays the music file
        /// @param loops: If loops == -1, loop forever,
        /// otherwise play it loops times
        void play(int loops = 1);

        /// Pauses whatever song is currently playing
        static void pause();
        /// Stops whatever song is currently playing
        static void stop();
        /// Resumes whatever song is currently playing
        static void resume();
    private:
        Mix_Music* _music = nullptr;
    };

    class AudioEngine {
    public:
        AudioEngine();
        ~AudioEngine();

        void init();
        void destroy();

        SoundEffect loadSoundEffect(const std::string& filePath);
        Music loadMusic(const std::string& filePath);
    private:

        std::map<std::string, Mix_Chunk*> _effectMap; ///< Effects cache
        std::map<std::string, Mix_Music*> _musicMap; ///< Music cache

        bool _isInitialized = false;
    };

}


#endif //BOMBERMAN_AUDIOENGINE_HPP
