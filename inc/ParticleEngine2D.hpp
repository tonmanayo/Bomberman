//
// Created by tonmanayo on 2017/08/06.
//

#ifndef BOMBERMAN_PARTICLE2D_HPP
#define BOMBERMAN_PARTICLE2D_HPP


#include <vector>
#include "ParticleBatch2D.hpp"
#include "SpriteBatch.hpp"

namespace WTCEngine {

    class ParticleBatch2D;
    class SpriteBatch;

    class ParticleEngine2D {
    public:
        ParticleEngine2D();
        ~ParticleEngine2D();

        // After adding a particle batch, the ParticleEngine2D becomes
        // responsible for deallocation.
        void addParticleBatch(ParticleBatch2D* particleBatch);

        void update(float deltaTime);

        void draw(SpriteBatch* spriteBatch);

    private:
        std::vector<ParticleBatch2D*> _batches;
    };

}


#endif //BOMBERMAN_PARTICLE2D_HPP
