#pragma once

#include "pendulumJoint.h"
#include <cmath>
#include <string>

class Calculator {
    public:

        virtual std::string getName() const = 0;

        void calculatePendulum(const pendulumJoint& j1, const pendulumJoint& j2,
                       double g, double& a1, double& a2) {
            double m1 = j1.getMass();
            double m2 = j2.getMass();
            double l1 = j1.getLength();
            double l2 = j2.getLength();

            double t1 = j1.getTheta();
            double t2 = j2.getTheta();

            double w1 = j1.getOmega();
            double w2 = j2.getOmega();

            double delta = t1 - t2;

            double denom = 2*m1 + m2 - m2 * cos(2*delta);

            // alpha1
            a1 = (
                -g * (2*m1 + m2) * sin(t1)
                - m2 * g * sin(t1 - 2*t2)
                - 2 * sin(delta) * m2 * (w2*w2*l2 + w1*w1*l1 * cos(delta))
            ) / (l1 * denom);

            // alpha2
            a2 = (
                2 * sin(delta) * (
                    w1*w1*l1*(m1 + m2)
                    + g*(m1 + m2)*cos(t1)
                    + w2*w2*l2*m2*cos(delta)
                )
            ) / (l2 * denom);
        }

        virtual void timeStep(pendulumJoint& j1, pendulumJoint& j2, double dt, double g) = 0;
};