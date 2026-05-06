#include "calculatePendulum.h"
#include <cmath>
#include "pendulumJoint.h"

void calculatePendulum(const pendulumJoint& j1, const pendulumJoint& j2, double g, double& a1, double& a2)
{
    // Mag umständlich sein, ist aber übersichtlicher
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

State derivatives(const State& s, double g) {
    State d;

    d.theta1 = s.omega1;
    d.theta2 = s.omega2;

    double alpha1, alpha2;

    pendulumJoint j1 = pendulumJoint(1.0, 1.0, s.theta1, s.omega1);
    pendulumJoint j2 = pendulumJoint(1.0, 1.0, s.theta2, s.omega2);

    calculatePendulum(j1, j2, g, alpha1, alpha2);

    d.omega1 = alpha1;
    d.omega2 = alpha2;

    return d;
}

void timeStepEuler(pendulumJoint& joint1, pendulumJoint& joint2, double dt, double g) {
    double alpha1, alpha2;
    calculatePendulum(joint1, joint2, g, alpha1, alpha2);

    joint1.setOmega(joint1.getOmega() + alpha1 * dt);
    joint2.setOmega(joint2.getOmega() + alpha2 * dt);

    joint1.setTheta(joint1.getTheta() + joint1.getOmega() * dt);
    joint2.setTheta(joint2.getTheta() + joint2.getOmega() * dt);
}

void timeStepRK4(pendulumJoint& joint1, pendulumJoint& joint2, double dt, double g) {
    State s{joint1.getTheta(), joint1.getOmega(), joint2.getTheta(), joint2.getOmega()};

    State k1 = derivatives(s, g);

    State s2 {
        s.theta1 + 0.5 * dt * k1.theta1,
        s.omega1 + 0.5 * dt * k1.omega1,
        s.theta2 + 0.5 * dt * k1.theta2,
        s.omega2 + 0.5 * dt * k1.omega2
    };
    State k2 = derivatives(s2, g);

    State s3 {
        s.theta1 + 0.5 * dt * k2.theta1,
        s.omega1 + 0.5 * dt * k2.omega1,
        s.theta2 + 0.5 * dt * k2.theta2,
        s.omega2 + 0.5 * dt * k2.omega2
    };
    State k3 = derivatives(s3, g);

    State s4 {
        s.theta1 + dt * k3.theta1,
        s.omega1 + dt * k3.omega1,
        s.theta2 + dt * k3.theta2,
        s.omega2 + dt * k3.omega2
    };
    
    State k4 = derivatives(s4, g);

    joint1.setTheta(joint1.getTheta() + (dt / 6.0) * (k1.theta1 + 2*k2.theta1 + 2*k3.theta1 + k4.theta1));
    joint1.setOmega(joint1.getOmega() + (dt / 6.0) * (k1.omega1 + 2*k2.omega1 + 2*k3.omega1 + k4.omega1));

    joint2.setTheta(joint2.getTheta() + (dt / 6.0) * (k1.theta2 + 2*k2.theta2 + 2*k3.theta2 + k4.theta2));
    joint2.setOmega(joint2.getOmega() + (dt / 6.0) * (k1.omega2 + 2*k2.omega2 + 2*k3.omega2 + k4.omega2));
}

void timeStepLeapfrog(pendulumJoint& j1, pendulumJoint& j2, double dt, double g) {
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

void getKartesianCoordinates(const pendulumJoint& joint1, const pendulumJoint& joint2,
                             float& x1, float& y1, float& x2, float& y2) {
    x1 = joint1.getLength() * std::sin(joint1.getTheta());
    y1 = -joint1.getLength() * std::cos(joint1.getTheta());
    x2 = x1 + joint2.getLength() * std::sin(joint2.getTheta());
    y2 = y1 - joint2.getLength() * std::cos(joint2.getTheta());
}