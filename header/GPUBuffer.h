#pragma once

#include <glad/glad.h>

class GPUBuffer {
    private:
        GLuint vao_ = 0, vbo_ = 0;
        
        void cleanup() {
            if (vbo_) glDeleteBuffers(1, &vbo_);
            if (vao_) glDeleteVertexArrays(1, &vao_);
        }
        
    public:
        GPUBuffer() { glGenVertexArrays(1, &vao_); glGenBuffers(1, &vbo_); }
        ~GPUBuffer() { cleanup(); }
        
        GPUBuffer(const GPUBuffer&) = delete;
        GPUBuffer& operator=(const GPUBuffer&) = delete;
        
        GPUBuffer(GPUBuffer&& other) noexcept : vao_(other.vao_), vbo_(other.vbo_) {
            other.vao_ = other.vbo_ = 0;
        }
        
        GLuint getVAO() const { return vao_; }
        GLuint getVBO() const { return vbo_; }

        void bind() const {
            glBindVertexArray(vao_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        }
        
        void unbind() const {
            glBindVertexArray(0);
        }
    };