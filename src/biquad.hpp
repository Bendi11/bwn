
#include <array>
#include <cstring>


struct biquad {
public:
    biquad(std::array<float, 2> a, std::array<float, 3> b) : _outs{0.f}, _ins{0.f}, _a{a}, _b{b} {
    }

    inline constexpr float sample(float in) {
        float out = _b[0] * in + _b[1] * _ins[0] + _b[2] * _ins[1] - _a[0] * _outs[0] - _a[1] * _outs[1];
        _outs[1] = _outs[0];
        _outs[0] = out;

        _ins[1] = _ins[0];
        _ins[0] = in;

        return out;
    }

private:
    float _outs[2];
    float _ins[2];

    std::array<float, 2> _a;
    std::array<float, 3> _b;
};
