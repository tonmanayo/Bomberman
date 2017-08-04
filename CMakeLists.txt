cmake_minimum_required(VERSION 3.8)
project(Bomberman)

set(CMAKE_CXX_STANDARD 14)

INCLUDE(FindPkgConfig)
INCLUDE(FindOpenGL REQUIRED)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(SOURCE_FILES src/main.cpp
        src/MainGame.cpp
        inc/MainGame.hpp
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
        inc/WTCEngine.hpp src/Camera2D.cpp inc/Camera2D.hpp src/SpriteBatch.cpp inc/SpriteBatch.hpp src/InputManager.cpp inc/InputManager.hpp src/FpsLimiter.cpp inc/FpsLimiter.hpp)

link_directories(

        ${GLEW_LIBRARY_DIRS}
        ${SDL2_LIBRARY_DIRS}
        ${GLM_LIBRARY_DIRS})

add_executable(Bomberman ${SOURCE_FILES})

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR}
                    ${SDL2_INCLUDE_DIRS}
                    ${SDL2IMAGE_INCLUDE_DIRS}
                    ${GLM_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_DIRS}
                    )

target_link_libraries(${PROJECT_NAME}
                      ${SDL2_LIBRARIES}
                      ${OPENGL_LIBRARIES}
                      ${GLM_LIBRARIES}
                      ${GLEW_LIBRARIES}
        )

