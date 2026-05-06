#pragma once

#include "pendulumJoint.h"

struct State {
    double theta1, omega1;
    double theta2, omega2;
};

void calculatePendulum(const pendulumJoint& j1, const pendulumJoint& j2,
                       double g, double& a1, double& a2);

State derivatives(const State& s, double g);

void timeStepEuler(pendulumJoint& joint1, pendulumJoint& joint2, double dt, double g);
void timeStepRK4(pendulumJoint& joint1, pendulumJoint& joint2, double dt, double g);
void timeStepLeapfrog(pendulumJoint& j1, pendulumJoint& j2, double dt, double g);

void getKartesianCoordinates(const pendulumJoint& joint1, const pendulumJoint& joint2,
                             float& x1, float& y1, float& x2, float& y2);
