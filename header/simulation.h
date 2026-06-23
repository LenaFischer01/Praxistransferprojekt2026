#pragma once

#include "pendulumJoint.h"
#include <cmath>
#include "CalculationHanlder.h"

class Simulation {
    private:
        pendulumJoint joint1_, joint2_;
        double gravity_;
        double scale_;
        CalculationHandler& calcHandler_ = CalculationHandler::getInstance();
    public:
        Simulation() : joint1_(1.0, 1.0), joint2_(1.0, 1.0), gravity_(9.81), scale_(0.75f) {
            updateCartesianCoordinates();
        }

        void update(double dt) { 
            calcHandler_.timeStep(joint1_, joint2_, dt, gravity_);
            updateCartesianCoordinates();
        }

        void setParameters(float l1, float l2, float m1, float m2, float g) {
            joint1_.setLength(l1); joint1_.setMass(m1);
            joint2_.setLength(l2); joint2_.setMass(m2);
            gravity_ = g;
            updateCartesianCoordinates();
            updateScale();
        }

        double getScale() const { return scale_; }
        
        void reset(float angle1, float angle2) {
            joint1_.setTheta(angle1 * M_PI / 180.0);
            joint2_.setTheta(angle2 * M_PI / 180.0);
            joint1_.setOmega(0);
            joint2_.setOmega(0);
            updateCartesianCoordinates();
        }

        pendulumJoint& getJoint1() { return joint1_; }
        pendulumJoint& getJoint2() { return joint2_; }
    
    private:
        void updateCartesianCoordinates() {
            const double x1 = joint1_.getLength() * std::sin(joint1_.getTheta());
            const double y1 = -joint1_.getLength() * std::cos(joint1_.getTheta());
            const double x2 = x1 + joint2_.getLength() * std::sin(joint2_.getTheta());
            const double y2 = y1 - joint2_.getLength() * std::cos(joint2_.getTheta());

            joint1_.setCartesianPosition(x1, y1);
            joint2_.setCartesianPosition(x2, y2);
        }

        void updateScale() {
            scale_ = 0.75f / (joint1_.getLength() + joint2_.getLength());
        }
};