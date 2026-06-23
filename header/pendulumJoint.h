#pragma once

#include <cmath>

class pendulumJoint
{
    private:
        double length_;
        double mass_;
        double theta_;
        double omega_;
        double x_;
        double y_;
    
    public:
        pendulumJoint(double l, double m);
        pendulumJoint(double l, double m, double t, double o);

        double getLength() const { return length_; }
        double getMass() const { return mass_; }
        double getTheta() const { return theta_; }
        double getOmega() const { return omega_; }
        double getX() const { return x_; }
        double getY() const { return y_; }

        void setLength(double l);
        void setMass(double m);
        void setTheta(double t);
        void setOmega(double o);
        void setCartesianPosition(double x, double y);
};