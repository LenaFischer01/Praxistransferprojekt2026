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

        State derivatives(const State& s, double g) {
            State d;

            d.theta1 = s.omega1;
            d.theta2 = s.omega2;

            double alpha1, alpha2;

            pendulumJoint j1 = pendulumJoint(s.length1, s.mass1, s.theta1, s.omega1);
            pendulumJoint j2 = pendulumJoint(s.length2, s.mass2, s.theta2, s.omega2);

            calculatePendulum(j1, j2, g, alpha1, alpha2);

            d.omega1 = alpha1;
            d.omega2 = alpha2;

            return d;
        }

        void timeStep(pendulumJoint& joint1, pendulumJoint& joint2, double dt, double g)
        {
            State s{joint1.getTheta(), joint1.getOmega(), joint2.getTheta(), joint2.getOmega(), joint1.getLength(), joint1.getMass(), joint2.getLength(), joint2.getMass()};

            State k1 = derivatives(s, g);

            State s2 {
                s.theta1 + 0.5 * dt * k1.theta1,
                s.omega1 + 0.5 * dt * k1.omega1,
                s.theta2 + 0.5 * dt * k1.theta2,
                s.omega2 + 0.5 * dt * k1.omega2,
                s.length1,
                s.mass1,
                s.length2,
                s.mass2
            };
            State k2 = derivatives(s2, g);

            State s3 {
                s.theta1 + 0.5 * dt * k2.theta1,
                s.omega1 + 0.5 * dt * k2.omega1,
                s.theta2 + 0.5 * dt * k2.theta2,
                s.omega2 + 0.5 * dt * k2.omega2,
                s.length1,
                s.mass1,
                s.length2,
                s.mass2
            };
            State k3 = derivatives(s3, g);

            State s4 {
                s.theta1 + dt * k3.theta1,
                s.omega1 + dt * k3.omega1,
                s.theta2 + dt * k3.theta2,
                s.omega2 + dt * k3.omega2,
                s.length1,
                s.mass1,
                s.length2,
                s.mass2
            };
            
            State k4 = derivatives(s4, g);

            joint1.setTheta(joint1.getTheta() + (dt / 6.0) * (k1.theta1 + 2*k2.theta1 + 2*k3.theta1 + k4.theta1));
            joint1.setOmega(joint1.getOmega() + (dt / 6.0) * (k1.omega1 + 2*k2.omega1 + 2*k3.omega1 + k4.omega1));

            joint2.setTheta(joint2.getTheta() + (dt / 6.0) * (k1.theta2 + 2*k2.theta2 + 2*k3.theta2 + k4.theta2));
            joint2.setOmega(joint2.getOmega() + (dt / 6.0) * (k1.omega2 + 2*k2.omega2 + 2*k3.omega2 + k4.omega2));
    }

        std::string getName() const override {
            return "Runge Kutta 4";
        }
};