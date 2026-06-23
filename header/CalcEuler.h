#pragma once

#include "Calculator.h"


class CalcEuler : public Calculator {
    public:
        void timeStep(pendulumJoint& joint1, pendulumJoint& joint2, double dt, double g) {
            double alpha1, alpha2;
            calculatePendulum(joint1, joint2, g, alpha1, alpha2);

            joint1.setOmega(joint1.getOmega() + alpha1 * dt);
            joint2.setOmega(joint2.getOmega() + alpha2 * dt);

            joint1.setTheta(joint1.getTheta() + joint1.getOmega() * dt);
            joint2.setTheta(joint2.getTheta() + joint2.getOmega() * dt);
        }

        std::string getName() const override {
            return "Euler";
        }
};