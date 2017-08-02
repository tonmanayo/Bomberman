//
// Created by Tony MACK on 2017/08/01.
//

#include <OpenGL/gl3.h>
#include "../inc/SpriteBatch.hpp"
#include <algorithm>

WTCEngine::SpriteBatch::SpriteBatch() : _vbo(0), _vao(0),_sortType(GlyphSortType::TEXTURE){
}

WTCEngine::SpriteBatch::~SpriteBatch() {

}

void WTCEngine::SpriteBatch::init() {
    createVertexArray();
}

void WTCEngine::SpriteBatch::draw(const glm::vec4 &destinationRect,
                                  const glm::vec4 &uvRect,
                                  const GLuint &texture,
                                  float depth,
                                  const Color &color) {
    Glyph* newGlyph = new Glyph;
    newGlyph->texture = texture;
    newGlyph->depth = depth;

    newGlyph->topL.color = color;
    newGlyph->topL.setPosition(destinationRect.x, destinationRect.y + destinationRect.w);
    newGlyph->topL.setUV(uvRect.x, uvRect.y + uvRect.w);

    newGlyph->bottomL.color = color;
    newGlyph->bottomL.setPosition(destinationRect.x, destinationRect.y);
    newGlyph->bottomL.setUV(uvRect.x, uvRect.y);

    newGlyph->bottomR.color = color;
    newGlyph->bottomR.setPosition(destinationRect.x + destinationRect.z, destinationRect.y);
    newGlyph->bottomR.setUV(uvRect.x + uvRect.z, uvRect.y);

    newGlyph->topR.color = color;
    newGlyph->topR.setPosition(destinationRect.x + destinationRect.z, destinationRect.y + destinationRect.w);
    newGlyph->topR.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

    _glyphs.push_back(newGlyph);
}

void WTCEngine::SpriteBatch::renderBatch() {
    glBindVertexArray(_vao);
    for (int i = 0; i < _renderBatches.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

        glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
    }
    glBindVertexArray(0);
}

void WTCEngine::SpriteBatch::end() {
    sortGlyphs();
    createRenderBatches();
}

void WTCEngine::SpriteBatch::createRenderBatches() {
    std::vector<Vertex> vertices;
    vertices.resize(_glyphs.size() * 6);
    if (_glyphs.empty())
        return;
    int cv = 0; //current vertex
    int offset = 0;
    _renderBatches.emplace_back(0, 6, _glyphs[0]->texture);
    vertices[cv++] = _glyphs[0]->topL;
    vertices[cv++] = _glyphs[0]->bottomL;
    vertices[cv++] = _glyphs[0]->bottomR;
    vertices[cv++] = _glyphs[0]->bottomR;
    vertices[cv++] = _glyphs[0]->topR;
    vertices[cv++] = _glyphs[0]->topL;
    offset += 6;

    for (int i = 1; i < _glyphs.size(); i++) {
        if (_glyphs[i]->texture != _glyphs[i - 1]->texture)
            _renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);
        else {
            _renderBatches.back().numVertices += 6;
        }
        vertices[cv++] = _glyphs[i]->topL;
        vertices[cv++] = _glyphs[i]->bottomL;
        vertices[cv++] = _glyphs[i]->bottomR;
        vertices[cv++] = _glyphs[i]->bottomR;
        vertices[cv++] = _glyphs[i]->topR;
        vertices[cv++] = _glyphs[i]->topL;
        offset += 6;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    //orphan buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    //upload the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void WTCEngine::SpriteBatch::createVertexArray() {
    if (_vao == 0)
        glGenVertexArrays(1, &_vao);

    glBindVertexArray(_vao);
    if (_vbo == 0)
        glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    //color pointer
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    //uv pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

    glBindVertexArray(0);

}

void WTCEngine::SpriteBatch::begin(WTCEngine::GlyphSortType sortType) {
    _sortType = sortType;
    _renderBatches.clear();
    for (auto &_glyph : _glyphs) {
        delete _glyph;
    }
    _glyphs.clear();
}

void WTCEngine::SpriteBatch::sortGlyphs() {
    switch (_sortType) {
        case GlyphSortType::BACK_TO_FRONT:
            std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
            break;
        case GlyphSortType::FRONT_TO_BACK:
            std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
            break;
        case GlyphSortType::TEXTURE:
            std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTextures);
            break;
    }
}

bool WTCEngine::SpriteBatch::compareFrontToBack(WTCEngine::Glyph *a, WTCEngine::Glyph *b) {
    return (a->depth < b->depth);
}

bool WTCEngine::SpriteBatch::compareBackToFront(WTCEngine::Glyph *a, WTCEngine::Glyph *b) {
    return (a->depth > b->depth);
}

bool WTCEngine::SpriteBatch::compareTextures(WTCEngine::Glyph *a, WTCEngine::Glyph *b) {
    return (a->texture < b->texture);
}


