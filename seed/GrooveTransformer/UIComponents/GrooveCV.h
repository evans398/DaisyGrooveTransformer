#pragma once
#include "../Managers/HardwareManager.h"
#include "UIComponentHelper.h"

struct GrooveCV {
    int adc_channel;
    HardwareManager* hardware_manager;
    float prev_scaled_value;

    GrooveCV(enum AdcChannel adc_channel, HardwareManager* hardware_manager) {
        this->adc_channel = adc_channel;
        this->hardware_manager = hardware_manager;
        this->prev_scaled_value = GetScaledValue();
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetFloat(this->adc_channel);
    }

    int GetScaledValue(){
        return ScalePotValue(this->GetValue());
    }

    void TransmitNewValue(bool force_transmit){
        int scaled_value = GetScaledValue();
        // hardware_manager->hw->PrintLine("CV GP 1: " FLT_FMT3, FLT_VAR3(scaled_value));
        // int scaled_value = GetScaledValue();
        if (scaled_value != prev_scaled_value || force_transmit) {
            // hardware_manager->hw->PrintLine("Groove CV: " FLT_FMT3, FLT_VAR3(scaled_value));
            // this->uart_libre_manager->TransmitUIParameterValue(ModelParameter::UNCERTAINTY, scaled_value);
        }
        this->prev_scaled_value = scaled_value;
    }
};