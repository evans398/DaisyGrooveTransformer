#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"
#include "UIComponentHelper.h"

struct InputGrooveVelocityPot {
    int mux_channel;
    int mux_idx;
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;
    float prev_scaled_value;

    InputGrooveVelocityPot(int mux_channel, int mux_idx, UartLibreManager* uart_libre_manager, HardwareManager* hardware_manager) {
        this->mux_channel = mux_channel;
        this->mux_idx = mux_idx;
        this->uart_libre_manager = uart_libre_manager;
        this->hardware_manager = hardware_manager;
        prev_scaled_value = GetScaledValue();
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetMuxFloat(this->mux_channel, this->mux_idx);
    }

    int GetScaledValue(){
        return ScalePotValue(this->GetValue());
    }

    void TransmitNewValue(bool force_transmit){
        int scaled_value = GetScaledValue();
        if (scaled_value != prev_scaled_value || force_transmit) {
            hardware_manager->hw->PrintLine("INPUT GROOVE VEL value: " FLT_FMT3, FLT_VAR3(scaled_value));
            this->uart_libre_manager->TransmitUIParameterValue(ModelParameter::INPUT_GROOVE_VELOCITY, scaled_value);
        }
        this->prev_scaled_value = scaled_value;
    }
};