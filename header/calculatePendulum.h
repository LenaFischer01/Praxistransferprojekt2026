#ifndef PENDULUM_H
#define PENDULUM_H

struct Pendulumjoint {
    double length;
    double mass;
    double theta;
    double omega;
};

struct State {
    double theta1, omega1;
    double theta2, omega2;
};

// kommt irgendwo anders her (oder auch in pendulum.cpp implementieren)
void calculatePendulum(const Pendulumjoint& j1, const Pendulumjoint& j2,
                       double g, double& a1, double& a2);

State derivatives(const State& s, double g);

void timeStepEuler(Pendulumjoint& joint1, Pendulumjoint& joint2, double dt, double g);
void timeStepRK4(Pendulumjoint& joint1, Pendulumjoint& joint2, double dt, double g);
void timeStepLeapfrog(Pendulumjoint& j1, Pendulumjoint& j2, double dt, double g);

void getKartesianCoordinates(const Pendulumjoint& joint1, const Pendulumjoint& joint2,
                             double& x1, double& y1, double& x2, double& y2);

#endif