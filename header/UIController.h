#pragma once

class UIController {
    public:
        struct Parameters {
            bool  showTrace = true; 
            float length1 = 1.0f;
            float length2 = 1.0f;
            float mass1 = 1.0f;
            float mass2 = 1.0f;
            float gravity = 9.81f;
            float angle1 = 0.0f;
            float angle2 = 0.0f;

            int traceLength = 1000;
            bool traceLengthChanged = false;

            bool run = false;
        };

        
    
    private:
        static UIController* instance_;
        UIController() = default;

        Parameters params_;
    
    public:

        UIController(const UIController&) = delete;
        UIController& operator=(const UIController&) = delete;

        static UIController& getInstance() {
            if (!instance_) {
                instance_ = new UIController();
            }
            return *instance_;
        }

        Parameters& getParams() { return params_; }
        void resetParameters() { params_ = Parameters(); }

        bool pollTraceChangedFlag() {
            bool changed = params_.traceLengthChanged;
            params_.traceLengthChanged = false;
            return changed;
        }
};