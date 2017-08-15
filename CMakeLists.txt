cmake_minimum_required(VERSION 3.7.2)
project(Bomberman)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${project_SOURCE_DIR}/cmake")

INCLUDE(FindPkgConfig)
INCLUDE(FindOpenGL REQUIRED)

find_package(SDL2 REQUIRED)

find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(Freetype REQUIRED)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()


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
        #src/AudioEngine.cpp
        #inc/AudioEngine.hpp
        src/ParticleBatch2D.cpp
        inc/ParticleBatch2D.hpp
        src/ParticleEngine2D.cpp
        inc/ParticleEngine2D.hpp
        src/SpriteFont.cpp
        inc/SpriteFont.hpp)

add_executable(Bomberman ${SOURCE_FILES})

include_directories(${OPENGL_INCLUDE_DIR}
                    ${GLM_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS}
                    ${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR}
                    ${FREETYPE_INCLUDE_DIR})

target_link_libraries(Bomberman
                      ${OPENGL_LIBRARIES}
                      ${GLM_LIBRARIES}
                      ${GLEW_LIBRARIES}
                      ${SDL2_LIBRARY}
                      ${SDL2_IMAGE_LIBRARIES}
                      ${SDL2_TTF_LIBRARIES}
                      ${FREETYPE_LIBRARIES})
