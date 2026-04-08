#include "renderer.h"
#include "ui.h"

Renderer::Renderer(const Shader& shader)
    : shader_(shader)
{
    vertsPerCircle_ = 1 + (Config::CIRCLE_SEGMENTS + 1); // center + ring(+1)
    circles_.resize(3 * vertsPerCircle_ * 3);
    traceVerts_.resize(Config::TRACE_POINTS * 3, 0.0f);

    initLineBuffers();
    initCircleBuffers();
    initTraceBuffers();
}

Renderer::~Renderer() {
    if (lineVBO_)   glDeleteBuffers(1, &lineVBO_);
    if (lineVAO_)   glDeleteVertexArrays(1, &lineVAO_);
    if (circleVBO_) glDeleteBuffers(1, &circleVBO_);
    if (circleVAO_) glDeleteVertexArrays(1, &circleVAO_);
    if (traceVBO_)  glDeleteBuffers(1, &traceVBO_);
    if (traceVAO_)  glDeleteVertexArrays(1, &traceVAO_);
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

void Renderer::initTraceBuffers() {
    glGenVertexArrays(1, &traceVAO_);
    glGenBuffers(1, &traceVBO_);

    glBindVertexArray(traceVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, traceVBO_);
    glBufferData(GL_ARRAY_BUFFER, Config::TRACE_POINTS * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

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
    // Multiplier1 for circle 1, Multiplier2 for circle 2
    float multiplier = 0.0f;
    for (int i = 0; i <= Config::CIRCLE_SEGMENTS; ++i) {
            if (circleIndex == 1) {
                multiplier = circleMultiplier1_;
            } else if (circleIndex == 2) {
                multiplier = circleMultiplier2_;
            } else {
                multiplier = 1.0f; // Default for the first circle
            }
        float a = 2.0f * (float)M_PI * (float)i / (float)Config::CIRCLE_SEGMENTS;
        circles_[startFloat + 3 * (1 + i) + 0] = cx + Config::CIRCLE_RADIUS * std::cos(a) * multiplier;
        circles_[startFloat + 3 * (1 + i) + 1] = cy + Config::CIRCLE_RADIUS * std::sin(a) * multiplier;
        circles_[startFloat + 3 * (1 + i) + 2] = 0.0f;
    }
}

void Renderer::addTracePoint(float x, float y) {
    if (tracePointCount_ < Config::TRACE_POINTS) {
        int idx = tracePointCount_ * 3;
        traceVerts_[idx + 0] = x;
        traceVerts_[idx + 1] = y;
        traceVerts_[idx + 2] = 0.0f;
        ++tracePointCount_;
    } else {
        // Shift all points left and add new point at the end
        for (int i = 0; i < (Config::TRACE_POINTS - 1) * 3; ++i) {
            traceVerts_[i] = traceVerts_[i + 3];
        }
        int idx = (Config::TRACE_POINTS - 1) * 3;
        traceVerts_[idx + 0] = x;
        traceVerts_[idx + 1] = y;
        traceVerts_[idx + 2] = 0.0f;
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

    addTracePoint(x2, y2);

    // Upload to GPU
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lineVerts_), lineVerts_);

    glBindBuffer(GL_ARRAY_BUFFER, circleVBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, circles_.size() * sizeof(float), circles_.data());

    glBindBuffer(GL_ARRAY_BUFFER, traceVBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, tracePointCount_ * 3 * sizeof(float), traceVerts_.data());
}

void Renderer::draw(float scale, bool showTrace) {
    shader_.use();
    shader_.setFloat("scale", scale);
    shader_.setVec2("offset", 0.0f, 0.0f);

    glLineWidth(Config::LINE_WIDTH);          

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

    // Trace
    if (showTrace) {
        shader_.setVec4("uColor", 1.0f, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(traceVAO_);
        glDrawArrays(GL_LINE_STRIP, 0, tracePointCount_);
    }

    glBindVertexArray(0);
}