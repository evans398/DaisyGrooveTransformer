#pragma once

using namespace daisy;
using namespace daisysp;

struct HardwareClock {
    /** Clock parameters */
    float bpm                       = 0.0f; // initial BPM is set by knob position
    const float ppqn                = 24.0f; // init pulses
    const float internal_clock_freq_hz = 200000000.0f; // can be confirmed with cv_clock_out.GetFreq()
    float period_freq_hz = 0.0f; //init to zero   


    /** Clock */
    TimerHandle* cv_clock_out;
    HardwareManager* hardware_manager;

    /** Clock Params */
    bool play_enabled = true;

    HardwareClock(TimerHandle* cv_clock_out, HardwareManager* hardware_manager){
        this->cv_clock_out = cv_clock_out;
        this->hardware_manager = hardware_manager;
    }

    void UpdatePeriod(float bpm){
        /** Period frequency at ppqn resolution (1/60) is hz/bpm (120bpm is 2hz). One beat is a q note so we multiply by ppqn. 
         We double this value because the timer interrupt puts clock low on one call and high the nex callt*/
        this->bpm = bpm;
        this->period_freq_hz = this->ppqn * this->bpm * (1.0f/60.0f) * 2;
        float period_freq_ticks = static_cast<uint32_t>(this->internal_clock_freq_hz/this->period_freq_hz);
        this->cv_clock_out->SetPeriod(period_freq_ticks);
    }
};
