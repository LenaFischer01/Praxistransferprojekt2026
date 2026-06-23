#pragma once

#include "Calculator.h"

class CalcRK4 : public Calculator {
    public:

        struct State {
            double theta1, omega1;
            double theta2, omega2;
            double length1, mass1;
            double length2, mass2;
        };

        State derivatives(const State& s, double g) {};

        void timeStep(pendulumJoint& joint1, pendulumJoint& joint2, double dt, double g){};

        std::string getName() const override {
            return "Runge Kutta 4";
        }
};