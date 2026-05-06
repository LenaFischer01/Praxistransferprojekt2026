#pragma once

#include "pendulumJoint.h"
#include "calculatePendulum.h"
#include <cmath>

class Simulation {
    private:
        pendulumJoint joint1_, joint2_;
        double gravity_;
        double scale_;
    
    public:
        Simulation() : joint1_(1.0, 1.0), joint2_(1.0, 1.0), gravity_(9.81), scale_(0.75f) {}

        void update(double dt) { 
            timeStepRK4(joint1_, joint2_, dt, gravity_); 
        }

        void getCoordinates(float& x1, float& y1, float& x2, float& y2) const {
            getKartesianCoordinates(joint1_, joint2_, x1, y1, x2, y2);
        }

        void setParameters(float l1, float l2, float m1, float m2, float g) {
            joint1_.setLength(l1); joint1_.setMass(m1);
            joint2_.setLength(l2); joint2_.setMass(m2);
            gravity_ = g;
            updateScale();
        }

        double getScale() const { return scale_; }
        
        void reset(float angle1, float angle2) {
            joint1_.setTheta(angle1 * M_PI / 180.0);
            joint2_.setTheta(angle2 * M_PI / 180.0);
            joint1_.setOmega(0);
            joint2_.setOmega(0);
        }
    
    private:
        void updateScale() {
            scale_ = 0.75f / (joint1_.getLength() + joint2_.getLength());
        }
};