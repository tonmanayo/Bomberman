cmake_minimum_required(VERSION 3.8)
project(Bomberman)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)

INCLUDE(FindPkgConfig)
INCLUDE(FindOpenGL REQUIRED)
#INCLUDE(FindGLEW REQUIRED)
set(SOURCE_FILES src/main.cpp src/MainGame.cpp inc/MainGame.hpp src/ErrorHandle.cpp inc/ErrorHandle.hpp src/Sprite.cpp inc/Sprite.hpp inc/GLSLProgram.hpp src/GLSLProgram.cpp inc/Vertex.hpp src/IOManager.cpp inc/IOManager.hpp src/picoPNG.cpp inc/picoPNG.hpp inc/GLTexture.hpp src/imageLoader.cpp inc/imageLoader.hpp)

#find_package(GLEW REQUIRED)
#if (GLEW_FOUND)
#    include_directories(${GLEW_INCLUDE_DIRS})
#    link_libraries(${GLEW_LIBRARIES})
#endif()

add_executable(Bomberman ${SOURCE_FILES})

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

#${GLEW_LIBRARIES}