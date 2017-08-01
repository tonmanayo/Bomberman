//
// Created by Tony MACK on 2017/08/01.
//

#ifndef BOMBERMAN_SPRITEBATCH_HPP
#define BOMBERMAN_SPRITEBATCH_HPP

#include <OpenGL/gl3.h>
#include "Vertex.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace WTCEngine {

    enum class GlyphSortType {
        NONE,
        FRONT_TO_BACK,
        BACK_TO_FRONT,
        TEXTURE
    };

    class RenderBatch {
    public:
        RenderBatch(){};
        RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset),
                                                                         numVertices(NumVertices),
                                                                         texture(Texture) {};
        GLuint offset;
        GLuint numVertices;
        GLuint texture;
    };

    struct Glyph {
        GLuint texture;
        float depth;

        Vertex topL;
        Vertex topR;
        Vertex bottomR;
        Vertex bottomL;
    };

    class SpriteBatch {

    private:
        GLuint _vbo;
        GLuint _vao;
        std::vector<Glyph*> _glyphs;
        GlyphSortType _sortType;
        std::vector<RenderBatch> _renderBatches;

        void createRenderBatches();
        void createVertexArray();
        void sortGlyphs();
        static bool compareFrontToBack(Glyph *a, Glyph *b);
        static bool compareBackToFront(Glyph *a, Glyph *b);
        static bool compareTextures(Glyph *a, Glyph *b);

    public:
        SpriteBatch();
        virtual ~SpriteBatch();

        void init();
        void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);

        void draw(const glm::vec4 &destinationRect,
                  const glm::vec4 &uvRect,
                  const GLuint &texture,
                  float depth,
                  const Color &color);

        void end();
        void renderBatch();


    };

}
#endif //BOMBERMAN_SPRITEBATCH_HPP
