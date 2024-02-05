#pragma once
#include "../UIComponents/Potentiometer.h"
#include "../UIComponents/PotentiometerWithCVInput.h"
#include "../GlobalHelpers/Enums.h"
#include "../UIComponents/InterpolationButton.h"
#include "../UIComponents/ClearButton.h"
#include "../UIComponents/PlayStopSwitch.h"
// #include "../UIComponents/VoiceVelocityScalePot.h"
// #include "../UIComponents/TempoPot.h"
// #include "../UIComponents/InterpolationPot.h"
// #include "../UIComponents/ShiftButton.h"
// #include "../UIComponents/PresetPot.h"
// #include "../UIComponents/SaveButton.h"
// #include "../UIComponents/GPCV.h"
// #include "../UIComponents/GeneralPurposeParameterPot.h"
// #include "../UIComponents/GrooveGate.h"
// #include "../UIComponents/GrooveCV.h"
// #include "../UIComponents/CVClockInput.h"
// #include "../UIComponents/RecordOverdubSwitch.h"

struct UIComponentsManager {
    HardwareManager* hardware_manager;
    InputBufferManager* input_buffer_manager;
    OutputBufferManager* output_buffer_manager;
    ClockManager* clock_manager;
    MidiManager* midi_manager;

    // TODO: IMPLEMENT PRESETS LATER
    // std::unique_ptr<PresetPot> preset_pot; 
    // std::unique_ptr<SaveButton> save_button;
    std::array<std::unique_ptr<Potentiometer>, NUM_OUTPUT_VOICES> voice_density_pots;
    std::array<std::unique_ptr<Potentiometer>, NUM_OUTPUT_VOICES> voice_velocity_scale_pots;
    std::unique_ptr<Potentiometer> input_groove_velocity_pot;
    std::unique_ptr<Potentiometer> input_groove_offset_pot;
    std::unique_ptr<Potentiometer> tempo_pot;
    std::unique_ptr<CVInput> crossfader_a_b_cv;
    std::unique_ptr<Potentiometer> crossfader_a_b; // Eventually change to PotentiometerWithCV/AttenInput
    std::unique_ptr<CVInput> uncertainty_cv;
    std::unique_ptr<PotentiometerWithCVInput> uncertainty_pot;
    std::unique_ptr<CVInput> follow_cv;
    std::unique_ptr<Potentiometer> follow_pot;
    std::unique_ptr<InterpolationButton> interpolation_button_a;
    std::unique_ptr<InterpolationButton> interpolation_button_b;
    std::unique_ptr<ClearButton> clear_button;
    std::unique_ptr<PlayStopSwitch> play_stop_switch;
    // std::unique_ptr<GeneralPurposeParameterPot> general_purpose_parameter_pot_2;
    // std::unique_ptr<GeneralPurposeParameterPot> general_purpose_parameter_pot_3;
    // std::unique_ptr<GPCV> gp_cv_1;
    // std::unique_ptr<GPCV> gp_cv_2;
    // std::unique_ptr<GPCV> gp_cv_3;
    // std::unique_ptr<GrooveGate> groove_gate;
    // std::unique_ptr<GrooveCV> groove_cv;
    // std::unique_ptr<CVClockInput> cv_clock_input;
    // std::unique_ptr<RecordOverdubSwitch> record_overdub_switch;


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
            int midi_cc_voice_density = VOICE_DENSITY_SCALE_1_CC + i;
            int midi_cc_voice_velocity = VOICE_VELOCITY_SCALE_1_CC + i;
            this->voice_density_pots[i] = std::make_unique<Potentiometer>(midi_cc_voice_density, MUX_0, i, hardware_manager, midi_manager);
            this->voice_velocity_scale_pots[i] = std::make_unique<Potentiometer>(midi_cc_voice_velocity, MUX_1, i, hardware_manager, midi_manager);
        }
        this->input_groove_velocity_pot = std::make_unique<Potentiometer>(GROOVE_OFFSET_CC, MUX_0, INPUT_GROOVE_OFF_POT, hardware_manager, midi_manager);
        this->input_groove_offset_pot = std::make_unique<Potentiometer>(GROOVE_VELOCITY_CC, MUX_0, INPUT_GROOVE_VEL_POT, hardware_manager, midi_manager);
        this->tempo_pot = std::make_unique<Potentiometer>(TEMPO_CC, MUX_1, TEMPO_POT, hardware_manager, midi_manager);
        this->crossfader_a_b_cv = std::make_unique<CVInput>(ASSIGNABLE_CV_2_CC, MUX_2, ASSIGNABLE_CV_2, hardware_manager, midi_manager);
        this->crossfader_a_b = std::make_unique<Potentiometer>(CROSSFADER_POS_CC, MUX_2, CROSSFADER, hardware_manager, midi_manager);
        this->uncertainty_cv = std::make_unique<CVInput>(UNCERTAINTY_CV_CC, MUX_2, UNCERTAINTY_CV, hardware_manager, midi_manager);
        this->uncertainty_pot = std::make_unique<PotentiometerWithCVInput>(UNCERTAINTY_CC, MUX_1, UNCERTAINTY_POT, hardware_manager, midi_manager, uncertainty_cv.get());
        this->follow_cv = std::make_unique<CVInput>(ASSIGNABLE_CV_1_CC, MUX_2, ASSIGNABLE_CV_1, hardware_manager, midi_manager);
        this->follow_pot = std::make_unique<PotentiometerWithCVInput>(ASSIGNABLE_POT_1_CC, MUX_2, ASSIGNABLE_POT_1, hardware_manager, midi_manager, follow_cv.get());
        this->interpolation_button_a = std::make_unique<InterpolationButton>(Interpolation_Button_Name::BUTTON_A, hardware_manager, midi_manager);
        this->interpolation_button_b = std::make_unique<InterpolationButton>(Interpolation_Button_Name::BUTTON_B, hardware_manager, midi_manager);
        // this->preset_pot = std::make_unique<PresetPot>(PRESET_KNOB, hardware_manager);
        // this->save_button = std::make_unique<SaveButton>(this->preset_pot.get(), hardware_manager);
        // this->record_overdub_switch = std::make_unique<RecordOverdubSwitch>(input_buffer_manager, hardware_manager);
        // this->groove_gate = std::make_unique<GrooveGate>(GROOVE_TRIGGER_IN, hardware_manager);
        // this->groove_cv = std::make_unique<GrooveCV>(GROOVE_VELOCITY_IN, hardware_manager);
        // this->cv_clock_input = std::make_unique<CVClockInput>(CLOCK_IN, hardware_manager);
        this->clear_button = std::make_unique<ClearButton>(hardware_manager, midi_manager);
        this->play_stop_switch = std::make_unique<PlayStopSwitch>(hardware_manager, midi_manager, clock_manager);
        // this->gp_cv_1 = std::make_unique<GPCV>(2, ASSIGNABLE_CV_1, hardware_manager);
        // this->gp_cv_2 = std::make_unique<GPCV>(2, ASSIGNABLE_CV_2, hardware_manager);
        // this->gp_cv_3 = std::make_unique<GPCV>(2, ASSIGNABLE_CV_3, hardware_manager);
        // this->general_purpose_parameter_pot_1 = std::make_unique<GeneralPurposeParameterPot>(2, ASSIGNABLE_POT_1, hardware_manager, this->gp_cv_1.get(), Model_Parameter::GENERAL_PURPOSE);
        // this->general_purpose_parameter_pot_2 = std::make_unique<GeneralPurposeParameterPot>(2, ASSIGNABLE_POT_2, hardware_manager, this->gp_cv_2.get(), Model_Parameter::GENERAL_PURPOSE);
        // this->general_purpose_parameter_pot_3 = std::make_unique<GeneralPurposeParameterPot>(2, ASSIGNABLE_POT_3, hardware_manager, this->gp_cv_3.get(), Model_Parameter::GENERAL_PURPOSE);
    };

    void ReadAndSetState() {
        this->play_stop_switch->ReadAndSetState();
        // this->tempo_pot->ReadAndSetTempo();
        // this->cv_clock_input->CheckIfGateChange();
    }

    void ReadUIComponents (){
        for (auto&& voice_density_pot : this->voice_density_pots) {
            voice_density_pot->TransmitNewValue(false);
        }
        for (auto&& voice_velocity_scale_pot : this->voice_velocity_scale_pots) {
            voice_velocity_scale_pot->TransmitNewValue(false);
        }
        this->input_groove_velocity_pot->TransmitNewValue(false);
        this->input_groove_offset_pot->TransmitNewValue(false);
        this->tempo_pot->TransmitNewValue(false);
        this->crossfader_a_b->TransmitNewValue(false);
        this->uncertainty_pot->TransmitNewValue(false);
        this->follow_pot->TransmitNewValue(false);
        this->interpolation_button_a->TransmitNewValue();
        this->interpolation_button_b->TransmitNewValue();
        // this->save_button->TransmitNewValue(false);
        // // this->groove_gate->TransmitNewValue(false);
        // // this->cv_clock_input->CheckIfGateChange();
        // this->groove_cv->TransmitNewValue(false);
        // this->record_overdub_switch->ReadAndSetState();
        this->clear_button->TransmitNewValue();
        this->play_stop_switch->ReadAndSetState();
        // this->general_purpose_parameter_pot_1->TransmitNewValue(false);
        // this->general_purpose_parameter_pot_2->TransmitNewValue(false);
        // this->general_purpose_parameter_pot_3->TransmitNewValue(false);
    }
};
