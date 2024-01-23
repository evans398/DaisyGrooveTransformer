#pragma once
#include "../Managers/HardwareManager.h"
#include "UIComponentHelper.h"

struct PresetPot {
    int adc_channel;
    HardwareManager* hardware_manager;

    PresetPot(enum Adc_Channel adc_channel, HardwareManager* hardware_manager) {
        this->adc_channel = adc_channel;
        this->hardware_manager = hardware_manager;
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetFloat(this->adc_channel);
    }

    int GetScaledValue(){
        return ScalePotValue(this->GetValue());
    }

    int GetCurrentPresetIndex() {
        int scaled_value = GetScaledValue();
        int preset_index = 0; 

        int preset_threshold_0 = 20;
        int preset_threshold_1 = 40;
        int preset_threshold_2 = 60;
        int preset_threshold_3 = 80;

        if (scaled_value <= preset_threshold_0) {
            preset_index = 0;
        } else if (scaled_value > preset_threshold_0 && scaled_value <= preset_threshold_1) {
            preset_index = 1;
        } else if (scaled_value > preset_threshold_1 && scaled_value <= preset_threshold_2) {
            preset_index = 2;
        } else if (scaled_value > preset_threshold_2 && scaled_value <= preset_threshold_3) {
            preset_index = 3;
        } else if (scaled_value > preset_threshold_3) {
            preset_index = 4;
        }
        return preset_index;
    }
};