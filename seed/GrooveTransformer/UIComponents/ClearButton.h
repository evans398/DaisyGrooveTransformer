#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"
#include "../Managers/InputBufferManager.h"

struct ClearButton {
    HardwareManager* hardware_manager;
    MidiManager* midi_manager;
    bool prev_state_pressed;

    ClearButton(HardwareManager* hardware_manager, MidiManager* midi_manager) {
        this->hardware_manager = hardware_manager;
        this->midi_manager = midi_manager;
        hardware_manager->clear_button.Debounce();
        prev_state_pressed = hardware_manager->clear_button.Pressed();
    }

    void TransmitNewValue(){
        hardware_manager->clear_button.Debounce();
        if (hardware_manager->clear_button.Pressed() && !prev_state_pressed) {
            midi_manager->SendMidiCC(10, CLEAR_CC, 127);
        }
        prev_state_pressed = hardware_manager->clear_button.Pressed();
    }
};