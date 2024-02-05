#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"


struct SaveButton {
    PresetPot* preset_pot;
    HardwareManager* hardware_manager;
    bool prev_state_pressed;

    SaveButton(PresetPot* preset_pot, HardwareManager* hardware_manager) {
        this->preset_pot = preset_pot;
        this->hardware_manager = hardware_manager;
        prev_state_pressed = hardware_manager->save_button.Pressed();
    }

    void TransmitNewValue(bool force_transmit){
        this->hardware_manager->save_button.Debounce();
        hardware_manager->shift_button.Debounce();
        if ((this->prev_state_pressed && !this->hardware_manager->save_button.Pressed()) || force_transmit) {
            int preset_index = preset_pot->GetCurrentPresetIndex();
            if (hardware_manager->shift_button.Pressed() || force_transmit) {
                hardware_manager->BlinkLed(&hardware_manager->save_led, true);
            }
            if (!hardware_manager->shift_button.Pressed()) {
                hardware_manager->BlinkLed(&hardware_manager->save_led, false);
            }
        }
        this->prev_state_pressed = hardware_manager->save_button.Pressed();
    }
};