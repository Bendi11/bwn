#include "pll.hpp"
#include "qam.hpp"
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
    auto qam = qam_output<44100.f, 512.f>();
    auto demod = qam_demod<44100.f>();

    float buf[44100];
    qam.pilot_tone(buf);
    demod.lock(buf);

    qam.tone(3, buf);
    demod.decode(buf);

    pa_simple_write(pa_inst, buf, sizeof(buf), &ec);
    if(ec != PA_OK) {
        auto const error = pa_strerror(ec);
        fprintf(stderr, "Failed to write audio data: %s\n", error);
        return -1;
    }

    sleep(1);

    return 0;
}
