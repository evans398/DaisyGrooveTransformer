#include "../UIComponents/VoiceDensityPot.h"
#include "../GlobalHelpers/Enums.h"
#pragma once
#include "../UIComponents/VoiceVelocityScalePot.h"
#include "../UIComponents/InputGrooveVelocityPot.h"
#include "../UIComponents/InputGrooveOffsetPot.h"
#include "../UIComponents/TempoPot.h"
#include "../UIComponents/InterpolationPot.h"
#include "../UIComponents/ShiftButton.h"
#include "../UIComponents/InterpolationButton.h"
#include "../UIComponents/PresetPot.h"
#include "../UIComponents/SaveButton.h"
#include "../UIComponents/ClearButton.h"
#include "../UIComponents/GPCV.h"
#include "../UIComponents/GeneralPurposeParameterPot.h"
#include "../UIComponents/GrooveGate.h"
#include "../UIComponents/GrooveCV.h"
#include "../UIComponents/CVClockInput.h"
#include "../UIComponents/RecordOverdubSwitch.h"
#include "../UIComponents/PlayStopSwitch.h"

struct UIComponentsManager {
    HardwareManager* hardware_manager;
    InputBufferManager* input_buffer_manager;
    OutputBufferManager* output_buffer_manager;
    ClockManager* clock_manager;
    MidiManager* midi_manager;

    std::array<std::unique_ptr<VoiceDensityPot>, NUM_OUTPUT_VOICES> voice_density_pots;
    std::array<std::unique_ptr<VoiceVelocityScalePot>, NUM_OUTPUT_VOICES> voice_velocity_scale_pots;
    std::unique_ptr<InputGrooveVelocityPot> input_groove_velocity_pot;
    std::unique_ptr<InputGrooveOffsetPot> input_groove_offset_pot;
    std::unique_ptr<TempoPot> tempo_pot;
    std::unique_ptr<InterpolationPot> interpolation_pot;
    std::unique_ptr<ShiftButton> shift_button;
    std::unique_ptr<InterpolationButton> interpolation_button_a;
    std::unique_ptr<InterpolationButton> interpolation_button_b;
    std::unique_ptr<PresetPot> preset_pot;
    std::unique_ptr<SaveButton> save_button;
    std::unique_ptr<ClearButton> clear_button;
    std::unique_ptr<GPCV> uncertainty_cv_input;
    std::unique_ptr<GeneralPurposeParameterPot> uncertainty_pot;
    std::unique_ptr<GeneralPurposeParameterPot> general_purpose_parameter_pot_1;
    std::unique_ptr<GeneralPurposeParameterPot> general_purpose_parameter_pot_2;
    std::unique_ptr<GeneralPurposeParameterPot> general_purpose_parameter_pot_3;
    std::unique_ptr<GPCV> gp_cv_1;
    std::unique_ptr<GPCV> gp_cv_2;
    std::unique_ptr<GPCV> gp_cv_3;
    std::unique_ptr<GrooveGate> groove_gate;
    std::unique_ptr<GrooveCV> groove_cv;
    std::unique_ptr<CVClockInput> cv_clock_input;
    std::unique_ptr<RecordOverdubSwitch> record_overdub_switch;
    std::unique_ptr<PlayStopSwitch> play_stop_switch;


    UIComponentsManager(
        HardwareManager* hardware_manager,
        InputBufferManager* input_buffer_manager,
        OutputBufferManager* output_buffer_manager,
        ClockManager* clock_manager,
        MidiManager* midi_manager
    ) {
        //** Managers */
        this->hardware_manager = hardware_manager;
        this->input_buffer_manager = input_buffer_manager;
        this->output_buffer_manager = output_buffer_manager;
        this->clock_manager = clock_manager;
        this->midi_manager = midi_manager;

        //** Components */
        for (int i=0; i<NUM_OUTPUT_VOICES; i++) {
            this->voice_density_pots[i] = std::make_unique<VoiceDensityPot>(i, 0, i, hardware_manager);
            this->voice_velocity_scale_pots[i] = std::make_unique<VoiceVelocityScalePot>(i, 1, i, hardware_manager, output_buffer_manager);
        }
        this->input_groove_velocity_pot = std::make_unique<InputGrooveVelocityPot>(0, INPUT_GROOVE_VEL_POT, hardware_manager);
        this->input_groove_offset_pot = std::make_unique<InputGrooveOffsetPot>(0, INPUT_GROOVE_OFF_POT, hardware_manager);
        this->tempo_pot = std::make_unique<TempoPot>(1, TEMPO_POT, clock_manager, hardware_manager);
        this->interpolation_pot = std::make_unique<InterpolationPot>(2, INTERPOLATION_POT, this);
        this->shift_button = std::make_unique<ShiftButton>(this->hardware_manager);
        this->interpolation_button_a = std::make_unique<InterpolationButton>(Interpolation_Button_Name::BUTTON_A, hardware_manager);
        this->interpolation_button_b = std::make_unique<InterpolationButton>(Interpolation_Button_Name::BUTTON_B, hardware_manager);
        this->preset_pot = std::make_unique<PresetPot>(PRESET_KNOB, hardware_manager);
        this->save_button = std::make_unique<SaveButton>(this->preset_pot.get(), hardware_manager);
        this->clear_button = std::make_unique<ClearButton>(hardware_manager, input_buffer_manager);
        this->uncertainty_cv_input = std::make_unique<GPCV>(2, UNCERTAINTY_CV, hardware_manager);
        this->uncertainty_pot = std::make_unique<GeneralPurposeParameterPot>(1, UNCERTAINTY_POT, hardware_manager, this->uncertainty_cv_input.get(), Model_Parameter::UNCERTAINTY);
        this->gp_cv_1 = std::make_unique<GPCV>(2, GP_CV_1, hardware_manager);
        this->gp_cv_2 = std::make_unique<GPCV>(2, GP_CV_2, hardware_manager);
        this->gp_cv_3 = std::make_unique<GPCV>(2, GP_CV_3, hardware_manager);
        this->general_purpose_parameter_pot_1 = std::make_unique<GeneralPurposeParameterPot>(2, GP_POT_1, hardware_manager, this->gp_cv_1.get(), Model_Parameter::GENERAL_PURPOSE);
        this->general_purpose_parameter_pot_2 = std::make_unique<GeneralPurposeParameterPot>(2, GP_POT_2, hardware_manager, this->gp_cv_2.get(), Model_Parameter::GENERAL_PURPOSE);
        this->general_purpose_parameter_pot_3 = std::make_unique<GeneralPurposeParameterPot>(2, GP_POT_3, hardware_manager, this->gp_cv_3.get(), Model_Parameter::GENERAL_PURPOSE);
        this->groove_gate = std::make_unique<GrooveGate>(GROOVE_TRIGGER_IN, hardware_manager);
        this->groove_cv = std::make_unique<GrooveCV>(GROOVE_VELOCITY_IN, hardware_manager);
        this->cv_clock_input = std::make_unique<CVClockInput>(CLOCK_IN, hardware_manager);
        this->record_overdub_switch = std::make_unique<RecordOverdubSwitch>(input_buffer_manager, hardware_manager);
        this->play_stop_switch = std::make_unique<PlayStopSwitch>(hardware_manager, clock_manager, midi_manager);
    };

    void PlayReadAndSetState() {
        this->play_stop_switch->ReadAndSetState();
        this->tempo_pot->ReadAndSetTempo();
        // this->cv_clock_input->CheckIfGateChange();
    }

    void ReadUIComponents (){
        for (auto&& voice_density_pot : this->voice_density_pots) {
            voice_density_pot->TransmitNewValue(false);
        }
        for (auto&& voice_velocity_scale_pot : this->voice_velocity_scale_pots) {
            voice_velocity_scale_pot->ReadScaleVoiceVelocityPot();
        }
        this->input_groove_velocity_pot->TransmitNewValue(false);
        this->input_groove_offset_pot->TransmitNewValue(false);
        this->interpolation_pot->TransmitNewValue(false);
        this->interpolation_button_a->TransmitNewValue();
        this->interpolation_button_b->TransmitNewValue();
        this->save_button->TransmitNewValue(false);
        this->clear_button->TransmitNewValue();
        this->uncertainty_pot->TransmitNewValue(false);
        this->general_purpose_parameter_pot_1->TransmitNewValue(false);
        this->general_purpose_parameter_pot_2->TransmitNewValue(false);
        this->general_purpose_parameter_pot_3->TransmitNewValue(false);
        // this->groove_gate->TransmitNewValue(false);
        // this->cv_clock_input->CheckIfGateChange();
        this->groove_cv->TransmitNewValue(false);
        this->record_overdub_switch->ReadAndSetState();
        this->play_stop_switch->ReadAndSetState();
        this->tempo_pot->ReadAndSetTempo();
        this->shift_button->Pressed();
    }

    void ObserveSAPMessage() {
        if(false) {
            for (auto&& voice_density_pot : this->voice_density_pots) {
                voice_density_pot->TransmitNewValue(true);
            }
            this->input_groove_velocity_pot->TransmitNewValue(true);
            this->input_groove_offset_pot->TransmitNewValue(true);
            this->uncertainty_pot->TransmitNewValue(true);
            this->interpolation_pot->TransmitNewValue(true);
            // this->save_button->TransmitNewValue(true);
            this->general_purpose_parameter_pot_1->TransmitNewValue(true);
            this->general_purpose_parameter_pot_2->TransmitNewValue(true);
            this->general_purpose_parameter_pot_3->TransmitNewValue(true);

        }
    }

    float GetMuxValue(int mux_channel, int mux_idx){
        return this->hardware_manager->hw->adc.GetMuxFloat(mux_channel, mux_idx);
    }

    int GetScaledValue(int mux_channel, int mux_idx){
        return ScalePotValue(this->GetMuxValue(mux_channel, mux_idx));
    }

    int ScalePotValue(float value) {
        float scaled_value = value * 100.;
        int scaled_value_int = (int) scaled_value;
        return RoundScaledValue(scaled_value_int);
    }

    int RoundScaledValue(int scaled_value) {
        int rounded_value;
        int remainder = scaled_value % 5;
        if (scaled_value == 99) {
            return 100;
        }
        if (scaled_value == 1) {
            return 0;
        }
        if (remainder == 0) {
            rounded_value = scaled_value;
        }
        if(remainder < 3){
            rounded_value = scaled_value - remainder;
        }
        if(remainder > 2){
            rounded_value = scaled_value + (5 - remainder);
        }
        return rounded_value;
    }

    // TODO: IMPLEMENT FOR INTERP POT
    int ScaleInterpolationPotValue(float value) {
        float scaled_value = value * 100.;
        int scaled_value_int = (int) scaled_value;
        if (scaled_value > 99) {
            return 100;
        }
        if (scaled_value < 1) {
            return 0;
        }
        return scaled_value_int;
    }

    void TransmitValue(uint8_t cc_number, uint8_t value){
        this->midi_manager->SendMidiCC(MIDI_CC::MIDI_CC_CHANNEL, cc_number, value);
    }
};
