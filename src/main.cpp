#include <cmath>
#include <cstdio>
#include <numbers>
#include <pulse/def.h>
#include <pulse/pulseaudio.h>
#include <pulse/sample.h>
#include <pulse/simple.h>
#include <unistd.h>

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
        fprintf(stderr, "Failed to connect to PulseAudo server: %s\n", error);
        return -1;
    }

    float buf[44100];
    for(int i = 0; i < 44100; ++i) {
        buf[i] = std::cos(2.f*std::numbers::pi * (float)i * (240.f / 44100.f));
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
