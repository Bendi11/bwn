#pragma once

#include <cmath>
#include <numbers>


/// Numerically-controlled oscillator with runtime-variable frequency
template<float SAMPLE_F>
struct nco {
public:
    nco() = delete;
    
    inline constexpr nco(float freq) : _rads{0} { 
        set_frequency(freq);
    }

    inline constexpr float sample() {
        _rads += _rad_sample;
        if(_rads > 2*std::numbers::pi_v<float>) {
            _rads -= 2*std::numbers::pi_v<float>;
        }

        return std::cos(_rads);
    }


    inline constexpr void set_frequency(float f) {
        _rad_sample = 2.f*std::numbers::pi_v<float>* f / SAMPLE_F;
    }

    inline constexpr void set_phase(float rads) { _rads = rads; }

    inline constexpr float get_frequency() const { return _rad_sample / (2.f * std::numbers::pi_v<float>) * SAMPLE_F; }
private:
    float _rads;
    float _rad_sample;
};

template<float SAMPLE_F, float FREQ>
struct cosine {
public:
    inline constexpr cosine() = default;

    inline constexpr float sample() {
        _rads += (2.f * std::numbers::pi_v<float> * FREQ / SAMPLE_F);
        if(_rads > 2 * std::numbers::pi_v<float>) {
            _rads -= 2 * std::numbers::pi_v<float>;
        }

        return std::cos(_rads);
    }
private:
    float _rads = 0.f;
};

template<float SAMPLE_F, float FREQ>
struct sine {
public:
    inline constexpr sine() = default;

    inline constexpr float sample() {
        _rads += (2.f * std::numbers::pi_v<float> * FREQ / SAMPLE_F);
        if(_rads > 2 * std::numbers::pi_v<float>) {
            _rads -= 2 * std::numbers::pi_v<float>;
        }

        return std::sin(_rads);
    }
private:
    float _rads = 0.f;
};
