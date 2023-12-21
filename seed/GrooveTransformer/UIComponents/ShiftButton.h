#pragma once
#include "../Managers/HardwareManager.h"
#include "UIComponentHelper.h"

struct ShiftButton {
    HardwareManager* hardware_manager;
    bool prev = false;

    ShiftButton(HardwareManager* hardware_manager) {
        this->hardware_manager = hardware_manager;
    }

    ShiftButton(){};

    bool Pressed(){
        hardware_manager->shift_button.Debounce();
        bool pressed = hardware_manager->shift_button.Pressed();
        if (!pressed && prev) {
            hardware_manager->hw->PrintLine("SHIFT PRESSED");
        }
        prev = pressed;
        return hardware_manager->shift_button.Pressed();
    }
};