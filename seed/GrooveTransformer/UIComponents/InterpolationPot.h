#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"
#include "UIComponentHelper.h"

struct InterpolationPot {
    int mux_channel;
    int mux_idx;
    HardwareManager* hardware_manager;
    float prev_scaled_value;

    InterpolationPot(int mux_channel, int mux_idx, HardwareManager* hardware_manager) {
        this->mux_channel = mux_channel;
        this->mux_idx = mux_idx;
        this->hardware_manager = hardware_manager;
        prev_scaled_value = GetScaledValue();
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetMuxFloat(this->mux_channel, this->mux_idx);
    }

    int GetScaledValue(){
        return ScaleInterpolationPotValue(this->GetValue());
    }

    void TransmitNewValue(bool force_transmit){
        int scaled_value = GetScaledValue();
        if (scaled_value != prev_scaled_value || force_transmit) {
            // hardware_manager->hw->PrintLine("INTERPOLATION value: " FLT_FMT3, FLT_VAR3(scaled_value));
        }
        this->prev_scaled_value = scaled_value;
    }
};
