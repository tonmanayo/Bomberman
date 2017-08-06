cmake_minimum_required(VERSION 3.7.2)
project(Bomberman)

set(CMAKE_CXX_STANDARD 14)

INCLUDE(FindPkgConfig)
INCLUDE(FindOpenGL REQUIRED)

find_package(SDL2 REQUIRED)

find_package(SDL_mixer REQUIRED)
if (NOT SDL_MIXER_FOUND)
    message (FATAL_ERROR "SDL_mixer library could not be found my msg!")
endif (NOT SDL_MIXER_FOUND)

find_package(SDL_ttf REQUIRED)
if (NOT SDL_TTF_FOUND)
    message (FATAL_ERROR "SDL2_TTF library could not be found my msg!")
endif (NOT SDL2_TTF_FOUND)


find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(SOURCE_FILES src/main.cpp
        src/ErrorHandle.cpp
        inc/ErrorHandle.hpp
        src/Sprite.cpp
        inc/Sprite.hpp
        inc/GLSLProgram.hpp
        src/GLSLProgram.cpp
        inc/Vertex.hpp
        src/IOManager.cpp
        inc/IOManager.hpp
        src/picoPNG.cpp
        inc/picoPNG.hpp
        inc/GLTexture.hpp
        src/imageLoader.cpp
        inc/imageLoader.hpp
        src/ResourceManager.cpp
        inc/ResourceManager.hpp
        src/TextureCache.cpp
        inc/TextureCache.hpp
        src/Window.cpp
        inc/Window.hpp
        src/WTCEngine.cpp
        inc/WTCEngine.hpp
        src/Camera2D.cpp
        inc/Camera2D.hpp
        src/SpriteBatch.cpp
        inc/SpriteBatch.hpp
        src/InputManager.cpp
        inc/InputManager.hpp
        src/FpsLimiter.cpp
        inc/FpsLimiter.hpp
        ZombieGame/src/Player.cpp
        ZombieGame/inc/Player.hpp
        ZombieGame/src/Enemy.cpp
        ZombieGame/inc/Enemy.hpp
        ZombieGame/src/Level.cpp
        ZombieGame/inc/Level.hpp
        ZombieGame/src/Agent.cpp
        ZombieGame/inc/Agent.hpp
        ZombieGame/src/MainGame.cpp
        ZombieGame/inc/MainGame.hpp
        ZombieGame/src/Human.cpp
        ZombieGame/inc/Human.hpp
        ZombieGame/src/Bullet.cpp
        ZombieGame/inc/Bullet.hpp
        ZombieGame/src/Zombie.cpp
        ZombieGame/inc/Zombie.hpp
        ZombieGame/src/Gun.cpp
        ZombieGame/inc/Gun.hpp
        src/AudioEngine.cpp
        inc/AudioEngine.hpp
        src/ParticleBatch2D.cpp
        inc/ParticleBatch2D.hpp
        src/ParticleEngine2D.cpp
        inc/ParticleEngine2D.hpp
        src/SpriteFont.cpp
        inc/SpriteFont.hpp)

link_directories(
        ${GLEW_LIBRARY_DIRS}
        ${SDL2_LIBRARY_DIRS}
        ${GLM_LIBRARY_DIRS}
        ${SDL_MIXER_LIBRARY_DIRS}
        ${SDL_TTF_LIBRARY_DIRS})

add_executable(Bomberman ${SOURCE_FILES})

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR}
                    ${SDL2_INCLUDE_DIRS}
                    ${SDL2IMAGE_INCLUDE_DIRS}
                    ${GLM_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS}
                    ${SDL_MIXER_INCLUDE_DIRS}
                    ${SDL_TTF_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
                      ${SDL2_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${GLM_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      ${SDL_MIXER_LIBRARIES}
                      ${SDL_TTF_LIBRARIES})
