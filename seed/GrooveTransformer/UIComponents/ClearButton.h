#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"
#include "../Managers/InputBufferManager.h"
#include "UIComponentHelper.h"

struct ClearButton {
    HardwareManager* hardware_manager;
    InputBufferManager* input_buffer_manager;
    bool prev_state_pressed;

    ClearButton(HardwareManager* hardware_manager, InputBufferManager* input_buffer_manager) {
        this->hardware_manager = hardware_manager;
        this->input_buffer_manager = input_buffer_manager;
        hardware_manager->clear_button.Debounce();
        prev_state_pressed = hardware_manager->clear_button.Pressed();
    }

    void TransmitNewValue(){
        hardware_manager->clear_button.Debounce();
        if (hardware_manager->clear_button.Pressed() && !prev_state_pressed) {
            input_buffer_manager->ClearBuffer();
        }
        prev_state_pressed = hardware_manager->clear_button.Pressed();
    }
};