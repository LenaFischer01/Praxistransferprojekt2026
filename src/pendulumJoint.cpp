# include "pendulumJoint.h"

#include <stdexcept>

pendulumJoint::pendulumJoint(double l, double m) 
        : length_(l), mass_(m), theta_(0), omega_(0) {
    if (l <= 0 || m <= 0) {
        throw std::invalid_argument("Length and mass must be positive");
    }
}

pendulumJoint::pendulumJoint(double l, double m, double t, double o) 
        : length_(l), mass_(m), theta_(t), omega_(o) {
    if (l <= 0 || m <= 0) {
        throw std::invalid_argument("Length and mass must be positive");
    }
}

void pendulumJoint::setLength(double l) { 
    if (l <= 0) throw std::invalid_argument("Length must be positive");
    length_ = l; 
}

void pendulumJoint::setMass(double m) { 
    if (m <= 0) throw std::invalid_argument("Mass must be positive");
    mass_ = m; 
}

void pendulumJoint::setTheta(double t) { theta_ = t; }
void pendulumJoint::setOmega(double o) { omega_ = o; }