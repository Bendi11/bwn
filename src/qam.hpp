#pragma once

#include "nco.hpp"
#include "pll.hpp"
#include "biquad.hpp"
#include <cmath>
#include <cstddef>
#include <cstdint>


template<float SAMPLE_F, float CARRIER_F>
struct qam_output {
public:
    template<size_t SZ>
    void pilot_tone(float (&buf)[SZ]) {
        for(size_t i = 0; i < SZ; ++i) {
            _quadrature.sample();
            buf[i] = _in_phase.sample();
        }
    }

    template<size_t SZ>
    void tone(uint8_t byte, float (&buf)[SZ]) {
        for(size_t j = 0; j < SZ; ++j) {
            float i = _in_phase.sample() * I_CONSTELLATION[byte];
            float q = _quadrature.sample() * Q_CONSTELLATION[byte];
            buf[j] = i + q;
        }
    }
private:
    cosine<SAMPLE_F, CARRIER_F> _in_phase;
    sine<SAMPLE_F, CARRIER_F> _quadrature;

    static inline constexpr float I_CONSTELLATION[4] = {1.f, -1.f, -1.f, 1.f};
    static inline constexpr float Q_CONSTELLATION[4] = {1.f, 1.f, -1.f, -1.f};
};

template<float SAMPLE_F>
struct qam_demod {
public:
    template<size_t SZ>
    void lock(float (&buf)[SZ]) {
        for(size_t i = 0; i < SZ; ++i) {
            _pll.sample(buf[i]);
        }

        _in_phase.set_frequency(_pll.get_frequency());
        _quadrature.set_frequency(_pll.get_frequency());
        _quadrature.set_phase(-std::numbers::pi_v<float> / 2.f);
    }

    template<size_t SZ>
    uint8_t decode(float (&buf)[SZ]) {
        float i = 0.f;
        float q = 0.f;
        for(size_t j = 0; j < SZ; ++j) {
            float si = buf[j] * _in_phase.sample();
            float sq = buf[j] * _quadrature.sample();

            i += _i_lpf.sample(si);
            q += _q_lpf.sample(sq);
        }
        
        float m = sqrt(i*i + q*q) / SZ;
        i /= m;
        q /= m;

        std::cout << "theta " << static_cast<int>(std::atan2(q, i) * 180.f / std::numbers::pi_v<float>) << "  mag " << m << std::endl;
        return 0;
    }

private:
    nco<SAMPLE_F> _in_phase{0.f};
    nco<SAMPLE_F> _quadrature{0.f};
    dpll<SAMPLE_F> _pll;

    biquad _i_lpf{
        {-1.89931967, 0.90414926},
        {0.00120740, 0.00241479, 0.00120740}
    };
    biquad _q_lpf{
       {-1.89931967, 0.90414926},
        {0.00120740, 0.00241479, 0.00120740}
    };
};
