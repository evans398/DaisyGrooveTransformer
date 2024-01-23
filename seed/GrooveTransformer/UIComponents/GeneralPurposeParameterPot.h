#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/UartLibreManager.h"
#include "GPCV.h"
#include "UIComponentHelper.h"

struct GeneralPurposeParameterPot {
    int mux_channel;
    int mux_idx;
    int general_purpose_pot_index;
    HardwareManager* hardware_manager;
    GPCV* general_purpose_cv_input;
    enum Model_Parameter model_parameter;
    float prev_scaled_value;

    GeneralPurposeParameterPot(
        int mux_channel, 
        int mux_idx, 
        HardwareManager* hardware_manager,
        GPCV* general_purpose_cv_input,
        enum Model_Parameter model_parameter) {
        this->mux_channel=mux_channel;
        this->mux_idx=mux_idx;
        this->hardware_manager=hardware_manager;
        this->general_purpose_cv_input=general_purpose_cv_input;
        this->model_parameter=model_parameter;
        this->general_purpose_pot_index = mux_idx;
        prev_scaled_value = GetScaledValue();
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetMuxFloat(this->mux_channel, this->mux_idx);
    }

    int GetScaledValue(){
        return ScalePotValue(this->GetValue());
    }

    // void UpdateValueWithCvInput(float scaled_value_from_cv_input) {
    //     this->cv_value = scaled_value_from_cv_input;
    //     hardware_manager->hw->Print("GEN PURP %d", general_purpose_pot_index);
    //     hardware_manager->hw->PrintLine(" UPDATING CV VALUE: " FLT_FMT3, FLT_VAR3(scaled_value_from_cv_input));
    //     this->TransmitNewValue(true);
    // }

    void TransmitNewValue(bool force_transmit){
        int scaled_pot_value = GetScaledValue();
        int combined_value = this->general_purpose_cv_input->GetScaledValue() + scaled_pot_value;
        if (combined_value > 100) {
            combined_value = 100;
        }
        if (combined_value != prev_scaled_value || force_transmit) {
            switch(this->model_parameter) {
                case Model_Parameter::GENERAL_PURPOSE:
                    break;
                case Model_Parameter::UNCERTAINTY:
                    break;
                default:
                    break;
            }
            
        }
        this->prev_scaled_value = combined_value;
    }
};