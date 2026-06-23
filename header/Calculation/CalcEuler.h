#pragma once

#include "Calculator.h"


class CalcEuler : public Calculator {
    public:
        void timeStep(pendulumJoint& joint1, pendulumJoint& joint2, double dt, double g) {};

        std::string getName() const override {
            return "Euler";
        }
};