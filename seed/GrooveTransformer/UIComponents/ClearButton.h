#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"
#include "../Managers/InputBufferManager.h"
#include "UIComponentHelper.h"

struct ClearButton {
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;
    InputBufferManager* input_buffer_manager;
    bool prev_state_pressed;

    ClearButton(UartLibreManager* uart_libre_manager, HardwareManager* hardware_manager, InputBufferManager* input_buffer_manager) {
        this->uart_libre_manager = uart_libre_manager;
        this->hardware_manager = hardware_manager;
        this->input_buffer_manager = input_buffer_manager;
        hardware_manager->clear_button.Debounce();
        prev_state_pressed = hardware_manager->clear_button.Pressed();
    }

    void TransmitNewValue(){
        hardware_manager->clear_button.Debounce();
        if (hardware_manager->clear_button.Pressed() && !prev_state_pressed) {
            hardware_manager->hw->PrintLine("CLEAR BUTTON PRESSED");
            uart_libre_manager->TransmitUIParameterState(ModelParameter::RESET_GROOVE);
            input_buffer_manager->ClearBuffer();
        }
        prev_state_pressed = hardware_manager->clear_button.Pressed();
    }
};