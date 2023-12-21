#pragma once
#include "../Managers/HardwareManager.h"
#include "UIComponentHelper.h"

struct CVClockInput {
    int adc_channel;
    HardwareManager* hardware_manager;
    float prev_scaled_value;

    CVClockInput(enum AdcChannel adc_channel, HardwareManager* hardware_manager) {
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
            // hardware_manager->hw->PrintLine("CV CLOCK INPUT: " FLT_FMT3, FLT_VAR3(scaled_value));
            // this->uart_libre_manager->TransmitUIParameterValue(ModelParameter::UNCERTAINTY, scaled_value);
        }
        this->prev_scaled_value = scaled_value;
    }
};




// struct CVClockInput {
//     int mux_channel;
//     int mux_idx;
//     UartLibreManager* uart_libre_manager;
//     HardwareManager* hardware_manager;
//     // float prev_scaled_value;
//     bool gate_is_high = false;
//     int gate_counter = 0;
//     int gate_period = 24;
//     float prev_ms = 0;
//     // add cv input to this struct

//     CVClockInput(int mux_channel, int mux_idx, UartLibreManager* uart_libre_manager, HardwareManager* hardware_manager) {
//         this->mux_channel = mux_channel;
//         this->mux_idx = mux_idx;
//         this->uart_libre_manager = uart_libre_manager;
//         this->hardware_manager = hardware_manager;
//         // prev_scaled_value = GetScaledValue();
//     }

//     bool GateIsHigh(){
//         float v_level = this->hardware_manager->hw->adc.GetMuxFloat(this->mux_channel, this->mux_idx);
//         hardware_manager->hw->PrintLine(" v level " FLT_FMT3, FLT_VAR3(v_level));
//         return v_level > 0.2;
//     }

//     void CheckIfGateChange(){
//         if (!GateIsHigh() && gate_is_high) {
//             ReceivedTrigger();
//             hardware_manager->hw->PrintLine("trig received");
//         } 
//         gate_is_high = GateIsHigh();
//     }

//     float ms_to_bpm(float ms)
//     {
//         return 60000 / ms;
//     }   

//     void ReceivedTrigger() {
//         gate_counter++;
//         if(gate_counter == 24)
//         {
//             uint32_t ms   = System::GetNow();
//             uint32_t diff = ms - prev_ms;
//             uint32_t bpm  = ms_to_bpm(diff);
//             clock_man_bpm = bpm;
//             prev_ms = ms;
//             gate_counter = 0;
//         }
//     }


//     // float GetValue(){
//     //     return this->hardware_manager->hw->adc.GetFloat(this->adc_channel);
//     // }

//     // int GetScaledValue(){
//     //     return ScalePotValue(this->GetValue());
//     // }

//     // void TransmitNewValue(bool force_transmit){
//     //     int scaled_value = GetScaledValue();
//     //     // hardware_manager->hw->PrintLine("CV GP 1: " FLT_FMT3, FLT_VAR3(scaled_value));
//     //     // int scaled_value = GetScaledValue();
//     //     if (scaled_value != prev_scaled_value || force_transmit) {
//     //         hardware_manager->hw->PrintLine("CLOCK IN: " FLT_FMT3, FLT_VAR3(scaled_value));
//     //     }
//     //     this->prev_scaled_value = scaled_value;
//     // }



// };
