//
// Created by Tony MACK on 2017/07/31.
//

#ifndef BOMBERMAN_CAMERA2D_HPP
#define BOMBERMAN_CAMERA2D_HPP


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace WTCEngine {

    class Camera2D {
    public:
        Camera2D();

        virtual ~Camera2D();

        void init(int screenWidth, int screenHeight);

        void update();

        void setcamMatrix(glm::mat4 camMatrix);

        glm::mat4 getcamMatrix();

        void setScale(float scale);

        float getScale();

        void setPosition(const glm::vec2 &newPosition);
        glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

        glm::vec2 getPosition();


    private:
        int _ScreenWidth;
        int _ScreenHeight;
        bool _needsMatrixUpdate;
        glm::vec2 _position;
        glm::mat4 _camMatrix;
        glm::mat4 _orthoMatrix;
        float _scale;
    };
}


#endif //BOMBERMAN_CAMERA2D_HPP
