#pragma once

class UiState {
public:
    struct Parameters {
        bool showTrace = true;
        float length1 = 1.0f;
        float length2 = 1.0f;
        float mass1 = 1.0f;
        float mass2 = 1.0f;
        float gravity = 9.81f;
        float angle1 = 0.0f;
        float angle2 = 0.0f;

        int traceLength = 1000;
        bool traceLengthChanged = false;

        bool run = false;
        bool resetRequested = false;
    };

    Parameters& getParams() { return params_; }
    const Parameters& getParams() const { return params_; }

    void resetParameters() { params_ = Parameters(); }

private:
    Parameters params_;
};