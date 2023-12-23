#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"
#include "UIComponentHelper.h"

struct VoiceDensityPot {
    int voice_idx;
    int mux_channel;
    int mux_idx;
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;
    float prev_scaled_value;

    VoiceDensityPot(
        int voice_idx, 
        int mux_channel, 
        int mux_idx, 
        UartLibreManager* uart_libre_manager, 
        HardwareManager* hardware_manager
    ) {
        this->voice_idx = voice_idx;
        this->mux_channel = mux_channel;
        this->mux_idx = mux_idx;
        this->uart_libre_manager = uart_libre_manager;
        this->hardware_manager = hardware_manager;
        prev_scaled_value = GetScaledValue();
        TransmitNewValue(true);
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
            int voice_number = voice_idx + 1;
            uart_libre_manager->TransmitUIIndexedParameterValue(ModelParameter::VOICE_DENSITY, voice_number, scaled_value);
        }
        this->prev_scaled_value = scaled_value;
    }
};