#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <vector>
#include <cmath>

#include "shader.h"
#include "config.h"

class Renderer {
public:
    explicit Renderer(const Shader& shader);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // Update GPU-Daten
    void update(float x1, float y1, float x2, float y2);

    void draw(float scale);

private:
    Shader shader_;

    // Linien
    unsigned int lineVAO_ = 0, lineVBO_ = 0;
    float lineVerts_[12] = {}; // 4 vertices * 3 floats

    // Kreise
    unsigned int circleVAO_ = 0, circleVBO_ = 0;
    int vertsPerCircle_ = 0;
    std::vector<float> circles_; // 3 circles * vertsPerCircle * 3 floats

    // Spur
    unsigned int traceVAO_ = 0, traceVBO_ = 0;
    std::vector<float> traceVerts_; // maxTracePoints * 3 floats
    int tracePointCount_ = 0;

private:
    void initLineBuffers();
    void initCircleBuffers();
    void initTraceBuffers();
    void fillDisk(int circleIndex, float cx, float cy);
    void addTracePoint(float x, float y);
};

#endif