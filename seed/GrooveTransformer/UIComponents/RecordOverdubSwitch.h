#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"
#include "../Managers/InputBufferManager.h"
#include "UIComponentHelper.h"

struct RecordOverdubSwitch {
    UartLibreManager* uart_libre_manager;
    InputBufferManager* input_buffer_manager;
    HardwareManager* hardware_manager;
    bool rec_enabled;
    bool overdub_enabled;

    RecordOverdubSwitch(UartLibreManager* uart_libre_manager, InputBufferManager* input_buffer_manager, HardwareManager* hardware_manager) {
        this->uart_libre_manager = uart_libre_manager;
        this->input_buffer_manager = input_buffer_manager;
        this->hardware_manager = hardware_manager;
        hardware_manager->record_switch.Debounce();
        hardware_manager->overdub_switch.Debounce();
        rec_enabled = hardware_manager->record_switch.Pressed();
        overdub_enabled = hardware_manager->overdub_switch.Pressed();
    }

    void ReadAndSetState(){
        hardware_manager->record_switch.Debounce();
        hardware_manager->overdub_switch.Debounce();

        if ((overdub_enabled || rec_enabled) && !hardware_manager->record_switch.Pressed() && !hardware_manager->overdub_switch.Pressed()) {
            input_buffer_manager->rec_enabled = false;
            input_buffer_manager->overdub_enabled = false;
        }

        if (hardware_manager->record_switch.Pressed() && !rec_enabled) {
            input_buffer_manager->rec_enabled =true;
            input_buffer_manager->overdub_enabled = false;
        }

        if (hardware_manager->overdub_switch.Pressed() && !overdub_enabled) {
            input_buffer_manager->rec_enabled = false;
            input_buffer_manager->overdub_enabled = true;
        }
        rec_enabled = hardware_manager->record_switch.Pressed();
        overdub_enabled = hardware_manager->overdub_switch.Pressed();
    }
};