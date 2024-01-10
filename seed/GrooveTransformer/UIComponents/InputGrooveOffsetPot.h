#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"
#include "UIComponentHelper.h"

struct InputGrooveOffsetPot {
    int mux_channel;
    int mux_idx;
    HardwareManager* hardware_manager;
    float prev_scaled_value;

    InputGrooveOffsetPot(int mux_channel, int mux_idx, HardwareManager* hardware_manager) {
        this->mux_channel = mux_channel;
        this->mux_idx = mux_idx;
        this->hardware_manager = hardware_manager;
        prev_scaled_value = GetScaledValue();
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetMuxFloat(this->mux_channel, this->mux_idx);
    }

    int GetScaledValue(){
        return ScalePotValue(this->GetValue());
    }

    void TransmitNewValue(bool force_transmit){
        // TOO JUMPY
        // int scaled_value = GetScaledValue();
        int scaled_value = 0;
        if (scaled_value != prev_scaled_value || force_transmit) {
            return;
        }
        this->prev_scaled_value = scaled_value;
    }
};
