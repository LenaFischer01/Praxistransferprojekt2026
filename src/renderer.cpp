#include "renderer.h"

Renderer::Renderer(const Shader& shader)
    : shader_(shader)
{
    vertsPerCircle_ = 1 + (Config::CIRCLE_SEGMENTS + 1); // center + ring(+1)
    circles_.resize(3 * vertsPerCircle_ * 3);

    initLineBuffers();
    initCircleBuffers();
}

Renderer::~Renderer() {
    if (lineVBO_)   glDeleteBuffers(1, &lineVBO_);
    if (lineVAO_)   glDeleteVertexArrays(1, &lineVAO_);
    if (circleVBO_) glDeleteBuffers(1, &circleVBO_);
    if (circleVAO_) glDeleteVertexArrays(1, &circleVAO_);
}

void Renderer::initLineBuffers() {
    glGenVertexArrays(1, &lineVAO_);
    glGenBuffers(1, &lineVBO_);

    glBindVertexArray(lineVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerts_), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Renderer::initCircleBuffers() {
    glGenVertexArrays(1, &circleVAO_);
    glGenBuffers(1, &circleVBO_);

    glBindVertexArray(circleVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO_);
    glBufferData(GL_ARRAY_BUFFER, circles_.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Renderer::fillDisk(int circleIndex, float cx, float cy) {
    const int startVertex = circleIndex * vertsPerCircle_;
    const int startFloat  = 3 * startVertex;

    // Center
    circles_[startFloat + 0] = cx;
    circles_[startFloat + 1] = cy;
    circles_[startFloat + 2] = 0.0f;

    // Ring
    for (int i = 0; i <= Config::CIRCLE_SEGMENTS; ++i) {
        float a = 2.0f * (float)M_PI * (float)i / (float)Config::CIRCLE_SEGMENTS;
        circles_[startFloat + 3 * (1 + i) + 0] = cx + Config::CIRCLE_RADIUS * std::cos(a);
        circles_[startFloat + 3 * (1 + i) + 1] = cy + Config::CIRCLE_RADIUS * std::sin(a);
        circles_[startFloat + 3 * (1 + i) + 2] = 0.0f;
    }
}

void Renderer::update(float x1, float y1, float x2, float y2) {
    // Linien: (0,0)->(x1,y1) und (x1,y1)->(x2,y2)
    lineVerts_[0]  = 0.0f; lineVerts_[1]  = 0.0f; lineVerts_[2]  = 0.0f;
    lineVerts_[3]  = x1;   lineVerts_[4]  = y1;   lineVerts_[5]  = 0.0f;

    lineVerts_[6]  = x1;   lineVerts_[7]  = y1;   lineVerts_[8]  = 0.0f;
    lineVerts_[9]  = x2;   lineVerts_[10] = y2;   lineVerts_[11] = 0.0f;

    fillDisk(0, 0.0f, 0.0f);
    fillDisk(1, x1,   y1);
    fillDisk(2, x2,   y2);

    // Upload to GPU
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lineVerts_), lineVerts_);

    glBindBuffer(GL_ARRAY_BUFFER, circleVBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, circles_.size() * sizeof(float), circles_.data());
}

void Renderer::draw(float scale) {
    shader_.use();
    shader_.setFloat("scale", scale);
    shader_.setVec2("offset", 0.0f, 0.0f);

    // Lines
    shader_.setVec4("uColor", 0.8f, 0.2f, 0.6f, 1.0f);
    glBindVertexArray(lineVAO_);
    glDrawArrays(GL_LINES, 0, 4);

    // Circles
    shader_.setVec4("uColor", 1.0f, 0.2f, 0.2f, 1.0f);
    glBindVertexArray(circleVAO_);
    glDrawArrays(GL_TRIANGLE_FAN, 0 * vertsPerCircle_, vertsPerCircle_);
    glDrawArrays(GL_TRIANGLE_FAN, 1 * vertsPerCircle_, vertsPerCircle_);
    glDrawArrays(GL_TRIANGLE_FAN, 2 * vertsPerCircle_, vertsPerCircle_);

    glBindVertexArray(0);
}