#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"
#include "UIComponentHelper.h"

struct GPCV {
    int mux_channel;
    int mux_idx;
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;
    float prev_scaled_value;
    // add cv input to this struct

    GPCV(int mux_channel, int mux_idx, UartLibreManager* uart_libre_manager, HardwareManager* hardware_manager) {
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
};