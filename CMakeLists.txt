cmake_minimum_required(VERSION 3.5)
project(game)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

# For compatitability with yaml
set(CMAKE_MACOSX_RPATH 1)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ${PROJECT_SOURCE_DIR}/lib/irrKlang/*)

# Adding dependencies
pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLFW3 REQUIRED glfw3)
pkg_search_module(GLM REQUIRED glm)

include_directories(
        ${PROJECT_SOURCE_DIR}/Core/includes
        ${GLEW_INCLUDE_DIRS}
        ${GLFW3_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/Bomberman/include
)
link_directories(
        ${GLEW_LIBRARY_DIRS}
        ${GLFW3_LIBRARY_DIRS}
        ${GLM_LIBRARY_DIRS}
)

set(LIB_SOURCE_FILES Core/entities/window.cpp Core/entities/input.cpp
        Core/entities/shader.cpp Core/utils/file.cpp Core/sprite/squareSprite.cpp
        Core/entities/material.cpp Core/sprite/cubeSprite.cpp Core/entities/camera.cpp
        Core/render/renderer.cpp Core/models/gltf.cpp Core/models/gltfAnimation.cpp
        Core/entities/animation.cpp Core/entities/particle.cpp Core/entities/particleRenderer.cpp Core/entities/particleMaster.cpp Core/entities/particleSystem.cpp)

add_library(zion STATIC ${LIB_SOURCE_FILES})
if (APPLE)
    set(OPENGL_FLAGS "-framework OpenGL -framework GLUT -framework Cocoa -framework IOKit -framework CoreVideo")
endif()

set(SOURCE_FILES Game/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW3_LIBRARIES}
        ${GLM_LIBRARIES}
        zion
)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_FLAGS})
endif()

add_subdirectory(lib/nanogui)
add_subdirectory(ext/yaml-cpp)
include_directories(lib/nanogui/include
        ${PROJECT_SOURCE_DIR}/ext/yaml-cpp/include
        ${PROJECT_SOURCE_DIR}/ext/yaml-cpp
        lib/irrKlang/include)
include_directories(${NANOGUI_EXTRA_INCS})
add_definitions(${NANOGUI_EXTRA_DEFS})

link_directories("${PROJECT_SOURCE_DIR}/lib/irrKlang/lib")

set(B_SOURCE_FILES Bomberman/main.cpp Bomberman/src/MainGame.cpp Bomberman/src/scene.cpp
        Bomberman/src/player.cpp Bomberman/src/block.cpp Bomberman/src/sceneUpdate.cpp
        Bomberman/src/Bomb.cpp Bomberman/src/sceneCollision.cpp Bomberman/src/menu.cpp
        Bomberman/src/EnemyStuff.cpp Bomberman/src/sceneLoad.cpp Bomberman/src/BombStuff.cpp)

add_executable(Bomberman ${B_SOURCE_FILES})
target_link_libraries(Bomberman
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLM_LIBRARIES}
        zion
        nanogui
        yaml-cpp
        irrKlang
        ${NANOGUI_EXTRA_LIBS}
        )
target_link_libraries(Bomberman ${PROJECT_SOURCE_DIR}/lib/irrKlang/lib/libirrklang.dylib)
if (APPLE)
    target_link_libraries(Bomberman ${OPENGL_FLAGS})
endif()
