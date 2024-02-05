#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/MidiManager.h"

struct InterpolationButton {
    enum Interpolation_Button_Name interpolation_button_name;
    bool prev_state_pressed;
    float value;
    HardwareManager* hardware_manager;
    MidiManager* midi_manager;

    InterpolationButton(
        Interpolation_Button_Name interpolation_button_name, 
        HardwareManager* hardware_manager,
        MidiManager* midi_manager
    ) {
        this->interpolation_button_name = interpolation_button_name;
        this->hardware_manager = hardware_manager;
        this->midi_manager = midi_manager;
    }

    void TransmitNewValue(){
        if(interpolation_button_name == Interpolation_Button_Name::BUTTON_A) {
            this->hardware_manager->interpolation_button_a.Debounce();
            hardware_manager->shift_button.Debounce();
            if (this->hardware_manager->interpolation_button_a.Pressed() && !this->prev_state_pressed && !this->hardware_manager->shift_button.Pressed()) {
                midi_manager->SendMidiCC(10, A_BUTTON_SAVE_CC, 127);
            }
            if (this->hardware_manager->interpolation_button_a.Pressed() && !this->prev_state_pressed && this->hardware_manager->shift_button.Pressed()) {
                midi_manager->SendMidiCC(10, A_BUTTON_RANDOMIZE_CC, 127);
            }       
            this->prev_state_pressed = this->hardware_manager->interpolation_button_a.Pressed();
        } 
        if(interpolation_button_name == Interpolation_Button_Name::BUTTON_B) {
            this->hardware_manager->interpolation_button_b.Debounce();
            hardware_manager->shift_button.Debounce();
            if (this->hardware_manager->interpolation_button_b.Pressed() && !this->prev_state_pressed && !this->hardware_manager->shift_button.Pressed()) {
                midi_manager->SendMidiCC(10, B_BUTTON_SAVE_CC, 127);
            }
            if (this->hardware_manager->interpolation_button_b.Pressed() && !this->prev_state_pressed && this->hardware_manager->shift_button.Pressed()) {
                midi_manager->SendMidiCC(10, B_BUTTON_RANDOMIZE_CC, 127);
            }       
            this->prev_state_pressed = this->hardware_manager->interpolation_button_b.Pressed();
        } 
    }
};