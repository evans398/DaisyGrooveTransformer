#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/MidiManager.h"
#include "UIComponentHelper.h"

struct InterpolationPot {
    int mux_channel;
    int mux_idx;
    HardwareManager* hardware_manager;
    MidiManager* midi_manager;
    float prev_scaled_value;

    InterpolationPot(int mux_channel, int mux_idx, HardwareManager* hardware_manager, MidiManager* midi_manager) {
        this->mux_channel = mux_channel;
        this->mux_idx = mux_idx;
        this->hardware_manager = hardware_manager;
        prev_scaled_value = GetScaledValue(mux_channel, mux_idx);
    }

    void TransmitNewValue(bool force_transmit){
        int scaled_value = helper.GetScaledValue();
        if (scaled_value != prev_scaled_value || force_transmit) {
            helper.TransmitValue()
            // hardware_manager->hw->PrintLine("INTERPOLATION value: " FLT_FMT3, FLT_VAR3(scaled_value));
        }
        this->prev_scaled_value = scaled_value;
    }
};
