#pragma once
#include "UIComponentHelper.h"
#include "../Managers/HardwareManager.h"
#include "../Managers/MidiManager.h"

struct Potentiometer {
    int midi_cc;
    int mux_channel;
    int mux_idx;
    HardwareManager* hardware_manager;
    MidiManager* midi_manager;
    float prev_scaled_value;

    Potentiometer(
        int midi_cc, 
        int mux_channel, 
        int mux_idx, 
        HardwareManager* hardware_manager,
        MidiManager* midi_manager
    ) {
        this->midi_cc = midi_cc;
        this->mux_channel = mux_channel;
        this->mux_idx = mux_idx;
        this->hardware_manager = hardware_manager;
        this->midi_manager = midi_manager;
        prev_scaled_value = GetScaledValue();
        TransmitNewValue(true);
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetMuxFloat(this->mux_channel, this->mux_idx);
    }

    int GetScaledValue(){
        return ScalePotValue(this->GetValue());
    }

    void TransmitNewValue(bool force_transmit){
        int scaled_value = GetScaledValue();
        if (scaled_value != prev_scaled_value || force_transmit) {
            midi_manager->SendMidiCC(10, midi_cc, scaled_value);
        }
        this->prev_scaled_value = scaled_value;
    }
};