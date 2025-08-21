#pragma once

template<float SAMPLE_F>
struct zero_cross {
public:
    inline constexpr zero_cross() = default;
    
    inline constexpr float sample(float in) {
        bool out = (in > 0.f) != (_prev > 0.f);
        _prev = in;
        return (float)out;
    }
private:
    float _prev = 0.f;
};
