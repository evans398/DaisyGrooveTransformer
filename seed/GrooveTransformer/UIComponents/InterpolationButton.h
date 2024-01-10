#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"
#include "UIComponentHelper.h"

struct InterpolationButton {
    enum InterpolationButtonName interpolation_button_name;
    bool prev_state_pressed;
    float value;
    HardwareManager* hardware_manager;

    InterpolationButton(
        InterpolationButtonName interpolation_button_name, 
        HardwareManager* hardware_manager
    ) {
        this->interpolation_button_name = interpolation_button_name;
        this->hardware_manager = hardware_manager;
    }

    void TransmitNewValue(){
        if(interpolation_button_name == InterpolationButtonName::BUTTON_A) {
            this->hardware_manager->interpolation_button_a.Debounce();
            hardware_manager->shift_button.Debounce();
            if (this->hardware_manager->interpolation_button_a.Pressed() && !this->prev_state_pressed && !this->hardware_manager->shift_button.Pressed()) {
                hardware_manager->BlinkLed(&hardware_manager->b_led, false);
            }
            if (this->hardware_manager->interpolation_button_a.Pressed() && !this->prev_state_pressed && this->hardware_manager->shift_button.Pressed()) {
                hardware_manager->BlinkLed(&hardware_manager->b_led, true);
            }       
            this->prev_state_pressed = this->hardware_manager->interpolation_button_a.Pressed();
        } 
        if(interpolation_button_name == InterpolationButtonName::BUTTON_B) {
            this->hardware_manager->interpolation_button_b.Debounce();
            if (this->hardware_manager->interpolation_button_b.Pressed() && !this->prev_state_pressed && !this->hardware_manager->shift_button.Pressed()) {
                hardware_manager->BlinkLed(&hardware_manager->b_led, false);
            }
            if (this->hardware_manager->interpolation_button_b.Pressed() && !this->prev_state_pressed && this->hardware_manager->shift_button.Pressed()) {
                hardware_manager->BlinkLed(&hardware_manager->b_led, true);
            }       
            this->prev_state_pressed = this->hardware_manager->interpolation_button_b.Pressed();
        } 
    }
};