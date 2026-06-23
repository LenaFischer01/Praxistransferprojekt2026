#pragma once

#include "Calculator.h"
#include "CalcLeapFrog.h"
#include <memory>
#include <string>

class CalculationHandler {
    private:
        std::unique_ptr<Calculator> calculator_ = std::make_unique<CalcLeapFrog>();
        CalculationHandler() {}
    
    public:

        CalculationHandler(const CalculationHandler&) = delete;

        static CalculationHandler& getInstance() {
            static CalculationHandler instance;
            return instance;
        }

        void setCalculator(std::unique_ptr<Calculator> calculator) {
            calculator_ = std::move(calculator);
        }

        void timeStep(pendulumJoint& j1, pendulumJoint& j2, double dt, double g) {
            calculator_->timeStep(j1, j2, dt, g);
        }

        std::string getCalculatorString() {
            return calculator_->getName();
        }


};