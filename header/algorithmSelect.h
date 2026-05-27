#pragma once

class AlgorithmSelect {
    public:
        enum Method {
            Euler,
            RK4,
            Leapfrog
        };

        AlgorithmSelect() : method_(Method::Leapfrog) {}
        Method getMethod() const { return method_; }
        void setMethod(Method method) { method_ = method; }

    private:
        Method method_;
};