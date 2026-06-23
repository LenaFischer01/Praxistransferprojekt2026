#pragma once

#include "Calculator.h"

class CalcLeapFrog : public Calculator {
    public:
        void timeStep(pendulumJoint& j1, pendulumJoint& j2, double dt, double g) {
            double a1, a2;

            calculatePendulum(j1, j2, g, a1, a2);

            j1.setOmega(j1.getOmega() + 0.5 * a1 * dt);
            j2.setOmega(j2.getOmega() + 0.5 * a2 * dt);

            j1.setTheta(j1.getTheta() + j1.getOmega() * dt);
            j2.setTheta(j2.getTheta() + j2.getOmega() * dt);

            calculatePendulum(j1, j2, g, a1, a2);

            j1.setOmega(j1.getOmega() + 0.5 * a1 * dt);
            j2.setOmega(j2.getOmega() + 0.5 * a2 * dt);
        }

        std::string getName() const override {
            return "Leapfrog";
        }
};