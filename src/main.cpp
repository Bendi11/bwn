#include "pll.hpp"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <numbers>
#include <pulse/def.h>
#include <pulse/pulseaudio.h>
#include <pulse/sample.h>
#include <pulse/simple.h>
#include <unistd.h>

static inline constexpr size_t SAMPLE_RATE = 44100;

template<float FREQ>
static inline float cosine(int sample) {
    return std::cos(2.f*std::numbers::pi * (float)sample * (FREQ / (float)SAMPLE_RATE));
}

int main(int argc, char *argv[]) {
    auto audio_spec = pa_sample_spec {
        .format = PA_SAMPLE_FLOAT32,
        .rate = 44100,
        .channels = 1,
    };

    int ec = 0;

    pa_simple *pa_inst = pa_simple_new(
        nullptr,
        "Bingo's White Noise Generator",
        PA_STREAM_PLAYBACK,
        nullptr,
        "Noise",
        &audio_spec,
        nullptr,
        nullptr,
        &ec
    );

    if(pa_inst == nullptr) {
        auto const error = pa_strerror(ec);
        fprintf(stderr, "Failed to connect to PulseAudio server: %s\n", error);
        return -1;
    }

    auto pll = dpll<44100.f>();

    float buf[88200];
    for(int i = 0; i < 88200; ++i) {
        buf[i] = pll.sample(cosine<300.5f>(i));
        if(i % 1000 == 0) std::cout << "Frequency is " << pll.get_frequency() << std::endl;
    }

    pa_simple_write(pa_inst, buf, sizeof(buf), &ec);
    if(ec != PA_OK) {
        auto const error = pa_strerror(ec);
        fprintf(stderr, "Failed to write audio data: %s\n", error);
        return -1;
    }

    sleep(1);

    return 0;
}
