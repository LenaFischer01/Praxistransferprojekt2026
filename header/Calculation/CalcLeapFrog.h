#pragma once

#include "Calculator.h"

class CalcLeapFrog : public Calculator {
    public:
        void timeStep(pendulumJoint& j1, pendulumJoint& j2, double dt, double g) override;

        std::string getName() const override {
            return "Leapfrog";
        }
};