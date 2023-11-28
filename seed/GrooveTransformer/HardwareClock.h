#pragma once

using namespace daisy;
using namespace daisysp;

struct HardwareClock {
    /** Clock static parameters */
    float bpm                       = 120.0f; // initialize BPM to 120
    float bps                       = bpm / 60.0f; // convert to beats per second
    const float ppqn                = 48.0f; // init pulses
    float clock_freq_hz             = ppqn * bps; // clock frequency at ppqn resolution (1/60) is hz/bpm (so 120bpm is 2hz). One is a q note so we multiply by ppqn
    const float internal_clock_freq_hz = 200000000.0f; // can be confirmed with hardware_clock.GetFreq()   

    /** Clock */
    TimerHandle* hardware_clock;
    bool led_on = true;

    HardwareManager* hardware_manager;

    /** Clock Params */
    bool play_enabled = true;

    HardwareClock(TimerHandle* hardware_clock, HardwareManager* hardware_manager){
        this->hardware_clock = hardware_clock;
        this->hardware_manager = hardware_manager;
    }

    // void UpdatePeriod(float bpm){
    //     float clock_freq_hz = this->ppqn * bpm * (1.0f/60.0f);
    //     float period = static_cast<uint32_t>(this->internal_clock_freq_hz/clock_freq_hz);
    //     this->hardware_clock->SetPeriod(period);
    // }

   
};
