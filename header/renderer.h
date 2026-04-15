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

    void draw(float scale, bool showTrace);

    void clearTrace() { tracePointCount_ = 0; }

    void setCircleMultiplier(float multiplier1, float multiplier2) { circleMultiplier1_ = multiplier1; circleMultiplier2_ = multiplier2; }

    void resetRenderer(int newTraceLength = Config::TRACE_POINTS);

private:
    Shader shader_;

    // Linien
    unsigned int lineVAO_ = 0, lineVBO_ = 0;
    float lineVerts_[12] = {}; // 4 vertices * 3 floats

    // Kreise
    unsigned int circleVAO_ = 0, circleVBO_ = 0;
    int vertsPerCircle_ = 0;
    std::vector<float> circles_; // 3 circles * vertsPerCircle * 3 floats
    float circleMultiplier1_ = 1.00f; 
    float circleMultiplier2_ = 1.00f; 

    // Spur
    unsigned int traceVAO_ = 0, traceVBO_ = 0;
    std::vector<float> traceVerts_; // maxTracePoints * 3 floats
    int tracePointCount_ = 0;

private:
    void initLineBuffers();
    void initCircleBuffers();
    void initTraceBuffers(int TracePoints = Config::TRACE_POINTS);
    void fillDisk(int circleIndex, float cx, float cy);
    void addTracePoint(float x, float y);
};

#endif