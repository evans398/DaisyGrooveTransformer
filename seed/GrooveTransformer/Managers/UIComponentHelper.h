#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/MidiManager.h"
#include "UIComponentHelper.h"

struct UIComponentHelper {
    HardwareManager* hardware_manager;
    MidiManager* midi_manager;

    UIComponentHelper(HardwareManager* hardware_manager, MidiManager* midi_manager) {
        this->hardware_manager = hardware_manager;
        this->midi_manager = midi_manager;
    }

    float GetValue(int mux_channel, int mux_idx){
        return this->hardware_manager->hw->adc.GetMuxFloat(mux_channel, mux_idx);
    }

    int GetScaledValue(int mux_channel, int mux_idx){
        return ScaleInterpolationPotValue(this->GetValue(mux_channel, mux_idx));
    }

    int RoundScaledValue(int scaled_value) {
        int rounded_value;
        int remainder = scaled_value % 5;
        if (scaled_value == 99) {
            return 100;
        }
        if (scaled_value == 1) {
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

    int ScaleInterpolationPotValue(float value) {
        float scaled_value = value * 100.;
        int scaled_value_int = (int) scaled_value;
        if (scaled_value > 99) {
            return 100;
        }
        if (scaled_value < 1) {
            return 0;
        }
        return scaled_value_int;
    }

    int ScalePotValue(float value) {
        float scaled_value = value * 100.;
        int scaled_value_int = (int) scaled_value;
        return RoundScaledValue(scaled_value_int);
    }

    void TransmitValue(int cc_number, int value){
        midi_manager->SendMidiCC(MIDI_CC::MIDI_CC_CHANNEL, cc_number, value);
    }
};
