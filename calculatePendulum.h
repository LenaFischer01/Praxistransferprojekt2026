#ifndef PENDULUM_H
#define PENDULUM_H

#include <cmath>

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

void calculatePendulum(const Pendulumjoint& j1, const Pendulumjoint& j2, double g, double& a1, double& a2)
{
    // Mag umständlich sein, ist aber übersichtlicher
    double m1 = j1.mass;
    double m2 = j2.mass;
    double l1 = j1.length;
    double l2 = j2.length;

    double t1 = j1.theta;
    double t2 = j2.theta;

    double w1 = j1.omega;
    double w2 = j2.omega;

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

    // theta' = omega
    d.theta1 = s.omega1;
    d.theta2 = s.omega2;

    // Beschleunigungen berechnen
    double alpha1, alpha2;

    // temporäre Pendelobjekte erstellen
    Pendulumjoint j1 = {1.0, 1.0, s.theta1, s.omega1};
    Pendulumjoint j2 = {1.0, 1.0, s.theta2, s.omega2};

    calculatePendulum(j1, j2, g, alpha1, alpha2);

    d.omega1 = alpha1;
    d.omega2 = alpha2;

    return d;
}


// Zu grobe Integration, führt zu sichtbaren Fehlern und Energiezunahme
void timeStepEuler(Pendulumjoint& joint1, Pendulumjoint& joint2, double dt, double g) {
    double alpha1, alpha2;
    calculatePendulum(joint1, joint2, g, alpha1, alpha2);

    joint1.omega += alpha1 * dt;
    joint2.omega += alpha2 * dt;

    joint1.theta += joint1.omega * dt;
    joint2.theta += joint2.omega * dt;
    
}


// Bessere Integration, deutlich stabiler, aber aufwendiger zu implementieren und immer noch nicht perfekt (Energiezunahme über lange Zeiträume)
void timeStepRK4(Pendulumjoint& joint1, Pendulumjoint& joint2, double dt, double g) {

    State s;

    // aktueller Zustand
    s.theta1 = joint1.theta;
    s.omega1 = joint1.omega;
    s.theta2 = joint2.theta;
    s.omega2 = joint2.omega;

    // K1
    State k1 = derivatives(s, g);

    // K2
    State s2 = {
        s.theta1 + 0.5 * dt * k1.theta1,
        s.omega1 + 0.5 * dt * k1.omega1,
        s.theta2 + 0.5 * dt * k1.theta2,
        s.omega2 + 0.5 * dt * k1.omega2
    };
    State k2 = derivatives(s2, g);

    // K3
    State s3 = {
        s.theta1 + 0.5 * dt * k2.theta1,
        s.omega1 + 0.5 * dt * k2.omega1,
        s.theta2 + 0.5 * dt * k2.theta2,
        s.omega2 + 0.5 * dt * k2.omega2
    };
    State k3 = derivatives(s3, g);

    // K4
    State s4 = {
        s.theta1 + dt * k3.theta1,
        s.omega1 + dt * k3.omega1,
        s.theta2 + dt * k3.theta2,
        s.omega2 + dt * k3.omega2
    };
    State k4 = derivatives(s4, g);

    // Finale Kombination
    joint1.theta += (dt / 6.0) * (k1.theta1 + 2*k2.theta1 + 2*k3.theta1 + k4.theta1);
    joint1.omega += (dt / 6.0) * (k1.omega1 + 2*k2.omega1 + 2*k3.omega1 + k4.omega1);

    joint2.theta += (dt / 6.0) * (k1.theta2 + 2*k2.theta2 + 2*k3.theta2 + k4.theta2);
    joint2.omega += (dt / 6.0) * (k1.omega2 + 2*k2.omega2 + 2*k3.omega2 + k4.omega2);
}

// Noch stabilere Integration, da symplektisch, d.h. Energie bleibt über lange Zeiträume konstant (sofern dt klein genug ist)
void timeStepLeapfrog(Pendulumjoint& j1, Pendulumjoint& j2, double dt, double g)
{
    double a1, a2;

    // aktuelle Beschleunigung
    calculatePendulum(j1, j2, g, a1, a2);

    j1.omega += 0.5 * a1 * dt;
    j2.omega += 0.5 * a2 * dt;

    // Positionen aktualisieren
    j1.theta += j1.omega * dt;
    j2.theta += j2.omega * dt;

    // neue Beschleunigung
    calculatePendulum(j1, j2, g, a1, a2);

    j1.omega += 0.5 * a1 * dt;
    j2.omega += 0.5 * a2 * dt;
}

void getKartesianCoordinates(const Pendulumjoint& joint1, const Pendulumjoint& joint2, double& x1, double& y1, double& x2, double& y2) {
    x1 = joint1.length * sin(joint1.theta);
    y1 = -joint1.length * cos(joint1.theta);
    x2 = x1 + joint2.length * sin(joint2.theta);
    y2 = y1 - joint2.length * cos(joint2.theta);
}

#endif