#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/ClockManager.h"
#include "UIComponentHelper.h"

struct TempoPot {
    float tempo;
    int mux_channel;
    int mux_idx;
    ClockManager* clock_manager;
    HardwareManager* hardware_manager;
    int prev_value;

    TempoPot(
        int mux_channel,
        int mux_idx,
        ClockManager* clock_manager, 
        HardwareManager* hardware_manager
    ){
        this->mux_channel=mux_channel;
        this->mux_idx=mux_idx;
        this->clock_manager=clock_manager;
        this->hardware_manager=hardware_manager;
        ReadAndSetTempo();
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetMuxFloat(this->mux_channel, this->mux_idx);
    }

    int ScaleTempoPotValue(float value) {
        float scaled_value = value * 100.;
        int scaled_value_int = (int) scaled_value;
        if(scaled_value > 98) {
            return 100;
        }
        if(scaled_value < 3) {
            return 0;
        }
        return scaled_value_int;
    }

    void ReadAndSetTempo() {
        int scaled_value = ScaleTempoPotValue(this->GetValue());
        if (scaled_value != prev_value) {
            float bpm = ((scaled_value/100.f) * 120.f) + 60.f; // min tempo is 60, max is 180
            clock_period_freq_hz = bpm * (1.0f/60.f) * ppqn;
            // hardware_manager->hw->PrintLine("BPM: " FLT_FMT3, FLT_VAR3(bpm));
            // this->clock_manager->metro->SetFreq(clock_freq_hz);
            this->clock_manager->UpdatePeriod(bpm);
        }
        prev_value = scaled_value;
    }
};