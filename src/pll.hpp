#pragma once

#include "digitize.hpp"
#include "nco.hpp"
#include <cmath>
#include <iostream>

template<float SAMPLE_F>
struct dpll {
public:
    inline constexpr dpll() : _osc{1.f} {}

    inline constexpr float get_frequency() { return _osc.get_frequency(); } 

    float sample(float in_sample) {
        float osc_sample = _osc.sample();

        float in = _input.sample(in_sample);
        float nco = _local.sample(osc_sample);

        float err = in - nco; 

        float sig = alpha * err + (1.f - alpha) * _prev_err;
        _prev_err = sig;
        
        float P = _kp * sig;
        float I = _ki * sig + _integ;
        _integ = I;

        _osc.set_frequency(_osc.get_frequency() + P + I);
        return osc_sample;
    }

    inline constexpr float osc() { return _osc.sample(); }

private:
    float _prev_err = 0.f;
    zero_cross<SAMPLE_F> _input;
    zero_cross<SAMPLE_F> _local;
    nco<SAMPLE_F> _osc;
    float _integ = 0.f;
    static inline constexpr float alpha = 0.001f;
    static inline constexpr float _kp = 55.f;
    static inline constexpr float _ki = 0.01f;
};
