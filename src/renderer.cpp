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

Renderer::~Renderer() {}

void Renderer::resetRenderer(int newTraceLength) {
    // Reshape traceVerts_ to new size based on updated trace length
    tracePointCount_ = 0;
    traceVerts_.resize(newTraceLength * 3, 0.0f);
    initTraceBuffers(newTraceLength); // Reinitialize trace buffers to reflect new size
}

void Renderer::initLineBuffers() {
    lineBuffer_.bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerts_), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    lineBuffer_.unbind();
}

void Renderer::initCircleBuffers() {
    circleBuffer_.bind();
    glBufferData(GL_ARRAY_BUFFER, circles_.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    circleBuffer_.unbind();
}

void Renderer::initTraceBuffers(int TracePoints) {
    traceBuffer_.bind();
    glBufferData(GL_ARRAY_BUFFER, TracePoints * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    traceBuffer_.unbind();
}

void Renderer::fillDisk(int circleIndex, float cx, float cy, float multiplier) {
    const int startVertex = circleIndex * vertsPerCircle_;
    const int startFloat  = 3 * startVertex;

    // Center
    circles_[startFloat + 0] = cx;
    circles_[startFloat + 1] = cy;
    circles_[startFloat + 2] = 0.0f;

    for (int i = 0; i <= Config::CIRCLE_SEGMENTS; ++i) {
        float a = 2.0f * (float)M_PI * (float)i / (float)Config::CIRCLE_SEGMENTS;
        circles_[startFloat + 3 * (1 + i) + 0] = cx + Config::CIRCLE_RADIUS * std::cos(a) * multiplier;
        circles_[startFloat + 3 * (1 + i) + 1] = cy + Config::CIRCLE_RADIUS * std::sin(a) * multiplier;
        circles_[startFloat + 3 * (1 + i) + 2] = 0.0f;
    }
}

void Renderer::addTracePoint(float x, float y) {
    if (tracePointCount_ < traceVerts_.size() / 3) {
        int idx = tracePointCount_ * 3;
        traceVerts_[idx + 0] = x;
        traceVerts_[idx + 1] = y;
        traceVerts_[idx + 2] = 0.0f;
        ++tracePointCount_;
    } else {
        // Shift all points left and add new point at the end
        for (int i = 0; i < (traceVerts_.size() / 3 - 1) * 3; ++i) {
            traceVerts_[i] = traceVerts_[i + 3];
        }
        int idx = (traceVerts_.size() / 3 - 1) * 3;
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
    fillDisk(1, x1,   y1, circleMultiplier1_);
    fillDisk(2, x2,   y2, circleMultiplier2_);

    addTracePoint(x2, y2);

    // Upload to GPU
    lineBuffer_.bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lineVerts_), lineVerts_);
    lineBuffer_.unbind();

    circleBuffer_.bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, circles_.size() * sizeof(float), circles_.data());
    circleBuffer_.unbind();

    traceBuffer_.bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, tracePointCount_ * 3 * sizeof(float), traceVerts_.data());
    traceBuffer_.unbind();
}

void Renderer::draw(float scale, bool showTrace) {
    shader_.use();
    shader_.setFloat("scale", scale);
    shader_.setVec2("offset", 0.0f, 0.0f);

    glLineWidth(Config::LINE_WIDTH);     
    
    // Trace
    if (showTrace) {
        shader_.setVec4("uColor", Config::COLOR_TRACE.r, Config::COLOR_TRACE.g, Config::COLOR_TRACE.b, Config::COLOR_TRACE.a);
        glBindVertexArray(traceBuffer_.getVAO());
        glDrawArrays(GL_LINE_STRIP, 0, tracePointCount_);
    }

    // Lines
    shader_.setVec4("uColor", Config::COLOR_LINES.r, Config::COLOR_LINES.g, Config::COLOR_LINES.b, Config::COLOR_LINES.a);
    glBindVertexArray(lineBuffer_.getVAO());
    glDrawArrays(GL_LINES, 0, 4);

    // Circles
    shader_.setVec4("uColor", Config::COLOR_CIRCLES.r, Config::COLOR_CIRCLES.g, Config::COLOR_CIRCLES.b, Config::COLOR_CIRCLES.a);
    glBindVertexArray(circleBuffer_.getVAO());
    glDrawArrays(GL_TRIANGLE_FAN, 0 * vertsPerCircle_, vertsPerCircle_);
    glDrawArrays(GL_TRIANGLE_FAN, 1 * vertsPerCircle_, vertsPerCircle_);
    glDrawArrays(GL_TRIANGLE_FAN, 2 * vertsPerCircle_, vertsPerCircle_);

    glBindVertexArray(0);
}