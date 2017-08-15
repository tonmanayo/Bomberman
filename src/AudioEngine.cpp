//
// Created by tonmanayo on 2017/08/06.
//

#include <iostream>
#include "../inc/AudioEngine.hpp"
#include "../inc/ErrorHandle.hpp"


namespace WTCEngine {

    void SoundEffect::play(int loops /* = 0 */ ) {
        try {
            if (Mix_PlayChannel(-1, _chunk, loops) == -1) {
                if (Mix_PlayChannel(0, _chunk, loops) == -1) {
                    ErrorHandle("Mix_PlayChannel error: " + std::string(Mix_GetError()));
                }
            }
        } catch (ErrorHandle errorHandle) {
            std::cout << errorHandle.what() << std::endl;
        }
    }

    void Music::play(int loops /* = -1 */) {
        Mix_PlayMusic(_music, loops);
    }

    void Music::pause() {
        Mix_PauseMusic();
    }

    void Music::stop() {
        Mix_HaltMusic();
    }

    void Music::resume() {
        Mix_ResumeMusic();
    }

    AudioEngine::AudioEngine() {
        // Empty
    }

    AudioEngine::~AudioEngine() {
        destroy();
    }


    void AudioEngine::init() {
        try {
            if (_isInitialized) {
                ErrorHandle("Tried to initialize Audio Engine twice!\n");
            }

            // Parameter can be a bitwise combination of MIX_INIT_FAC,
            // MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG
            if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
                ErrorHandle("Mix_Init error: " + std::string(Mix_GetError()));
            }

            if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
                ErrorHandle("Mix_OpenAudio error: " + std::string(Mix_GetError()));
            }

            _isInitialized = true;
        } catch (ErrorHandle errorHandle) {
            std::cout << errorHandle.what() << std::endl;
        }
    }

    void AudioEngine::destroy() {
        if (_isInitialized) {
            _isInitialized = false;

            for (auto &it : _effectMap) {
                Mix_FreeChunk(it.second);
            }

            for (auto &it : _musicMap) {
                Mix_FreeMusic(it.second);
            }

            _effectMap.clear();
            _musicMap.clear();

            Mix_CloseAudio();
            Mix_Quit();
        }
    }

    SoundEffect AudioEngine::loadSoundEffect(const std::string &filePath) {
        // Try to find the audio in the cache
        try {
            auto it = _effectMap.find(filePath);

            SoundEffect effect;

            if (it == _effectMap.end()) {
                // Failed to find it, must load
                Mix_Chunk *chunk = Mix_LoadWAV(filePath.c_str());
                // Check for errors
                if (chunk == nullptr) {
                    ErrorHandle("Mix_LoadWAV error: " + std::string(Mix_GetError()));
                }

                effect._chunk = chunk;
                _effectMap[filePath] = chunk;

            } else {
                // Its already cached
                effect._chunk = it->second;
            }

            return effect;
        } catch (ErrorHandle errorHandle) {
            std::cout << errorHandle.what() << std::endl;
        }
    }

    Music AudioEngine::loadMusic(const std::string &filePath) {
        // Try to find the audio in the cache
        try {
            auto it = _musicMap.find(filePath);

            Music music;

            if (it == _musicMap.end()) {
                // Failed to find it, must load
                Mix_Music *mixMusic = Mix_LoadMUS(filePath.c_str());
                // Check for errors
                if (mixMusic == nullptr) {
                    ErrorHandle("Mix_LoadMUS error: " + std::string(Mix_GetError()));
                }

                music._music = mixMusic;
                _musicMap[filePath] = mixMusic;

            } else {
                // Its already cached
                music._music = it->second;
            }

            return music;
        } catch (ErrorHandle errorHandle) {
            std::cout << errorHandle.what() << std::endl;
        }
    }
}