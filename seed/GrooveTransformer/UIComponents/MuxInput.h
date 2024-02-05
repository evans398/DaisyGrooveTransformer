#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/MidiManager.h"

struct MuxInput {
    int midi_cc;
    int mux_channel;
    int mux_idx;
    HardwareManager* hardware_manager;
    MidiManager* midi_manager;
    int prev_rounded_scaled_value;

    MuxInput(
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
        float input_value = GetValue();
        int scaled_value = GetScaledValue(input_value);
        this->prev_rounded_scaled_value = GetRoundedScaledValue(scaled_value);
        TransmitNewValue(true);
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetMuxFloat(this->mux_channel, this->mux_idx);
    }

    int GetScaledValue(float value){
        float scaled_value = value * 100.;
        int scaled_value_int = (int) scaled_value;
        return scaled_value_int;
    }

    int GetRoundedScaledValue(int scaled_value) {
        int rounded_value;
        int remainder = scaled_value % 5;
        if (scaled_value == 99 || scaled_value > 100) {
            return 100;
        }
        if (scaled_value == 1 || scaled_value < 0) {
            return 0;
        }
        if (remainder == 0) {
            rounded_value = scaled_value;
        }
        if(remainder < 3){
            rounded_value = scaled_value - remainder;
        }
        if(remainder > 2){
            rounded_value = scaled_value + (5 - remainder);
        }
        return rounded_value;
    }

    void TransmitNewValue(bool force_transmit){
        float input_value = GetValue();
        int scaled_value = GetScaledValue(input_value);
        int rounded_scaled_value = GetRoundedScaledValue(scaled_value);
        if (rounded_scaled_value != prev_rounded_scaled_value || force_transmit) {
            midi_manager->SendMidiCC(10, midi_cc, rounded_scaled_value);
        }
        this->prev_rounded_scaled_value = rounded_scaled_value;
    }
};