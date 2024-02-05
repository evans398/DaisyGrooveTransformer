#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/MidiManager.h"
#include "Potentiometer.h"
#include "CVInput.h"

struct PotentiometerWithCVInput : Potentiometer {
    int midi_cc;
    int mux_channel;
    int mux_idx;
    CVInput* cv_input;
    HardwareManager* hardware_manager;
    MidiManager* midi_manager;
    int prev_rounded_scaled_value;

    PotentiometerWithCVInput(
        int midi_cc, 
        int mux_channel, 
        int mux_idx, 
        HardwareManager* hardware_manager,
        MidiManager* midi_manager,
        CVInput* cv_input
    ) : Potentiometer(midi_cc, mux_channel, mux_idx, hardware_manager, midi_manager) {
        this->midi_cc = midi_cc;
        this->mux_channel = mux_channel;
        this->mux_idx = mux_idx;
        this->cv_input = cv_input;
        this->hardware_manager = hardware_manager;
        this->midi_manager = midi_manager;
        float input_value = GetValue() + cv_input->GetValue();
        int scaled_value = GetScaledValue(input_value);
        this->prev_rounded_scaled_value = GetRoundedScaledValue(scaled_value);
        TransmitNewValue(true);
    }

    void TransmitNewValue(bool force_transmit){
        float input_value = GetValue() + cv_input->GetValue();
        int scaled_value = GetScaledValue(input_value);
        int rounded_scaled_value = GetRoundedScaledValue(scaled_value);
        if (rounded_scaled_value != prev_rounded_scaled_value || force_transmit) {
            midi_manager->SendMidiCC(10, midi_cc, rounded_scaled_value);
        }
        this->prev_rounded_scaled_value = rounded_scaled_value;
    }
};