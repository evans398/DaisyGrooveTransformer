#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/OutputBufferManager.h"

struct VoiceVelocityScalePot {
    int voice_idx;
    int mux_channel;
    int mux_idx;
    HardwareManager* hardware_manager;
    OutputBufferManager* output_buffer_manager;
    float prev_scaled_value;

    VoiceVelocityScalePot(
        int voice_idx, 
        int mux_channel, 
        int mux_idx,
        HardwareManager* hardware_manager,
        OutputBufferManager* output_buffer_manager
    ){
        this->voice_idx=voice_idx;
        this->mux_channel=mux_channel;
        this->mux_idx=mux_idx;
        this->hardware_manager=hardware_manager;
        this->output_buffer_manager=output_buffer_manager;
        prev_scaled_value =GetScaledValue();
        ReadScaleVoiceVelocityPot();
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetMuxFloat(this->mux_channel, this->mux_idx);
    }

    int GetScaledValue(){
        return ScalePotValue(this->GetValue());
    }

    void ReadScaleVoiceVelocityPot() {
        int scaled_value = GetScaledValue();
        if (scaled_value != prev_scaled_value) {
            this->output_buffer_manager->UpdateVoiceVelocityScale(this->voice_idx, scaled_value);
        }
        this->prev_scaled_value = scaled_value;
    }
};