#pragma once
#include "UIComponents.h"

struct UIComponentsManager {
    HardwareManager* hardware_manager;
    UartLibreManager* uart_libre_manager;
    InputBufferManager* input_buffer_manager;
    OutputBufferManager* output_buffer_manager;
    ClockManager* clock_manager;
    HardwareClock* hardware_clock;
    UartMidiManager* uart_midi_manager;

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
        UartLibreManager* uart_libre_manager,
        InputBufferManager* input_buffer_manager,
        OutputBufferManager* output_buffer_manager,
        ClockManager* clock_manager,
        HardwareClock* hardware_clock,
        UartMidiManager* uart_midi_manager
    ) {
        //** Managers */
        this->hardware_manager = hardware_manager;
        this->uart_libre_manager = uart_libre_manager;
        this->input_buffer_manager = input_buffer_manager;
        this->output_buffer_manager = output_buffer_manager;
        this->clock_manager = clock_manager;
        this->hardware_clock = hardware_clock;
        this->uart_midi_manager = uart_midi_manager;

        //** Components */
        for (int i=0; i<NUM_OUTPUT_VOICES; i++) {
            this->voice_density_pots[i] = std::make_unique<VoiceDensityPot>(i, 0, i, uart_libre_manager, hardware_manager);
            this->voice_velocity_scale_pots[i] = std::make_unique<VoiceVelocityScalePot>(i, 1, i, hardware_manager, output_buffer_manager);
        }
        this->input_groove_velocity_pot = std::make_unique<InputGrooveVelocityPot>(0, INPUT_GROOVE_VEL_POT, uart_libre_manager, hardware_manager);
        this->input_groove_offset_pot = std::make_unique<InputGrooveOffsetPot>(0, INPUT_GROOVE_OFF_POT, uart_libre_manager, hardware_manager);
        this->tempo_pot = std::make_unique<TempoPot>(1, TEMPO_POT, clock_manager, hardware_manager, hardware_clock);
        this->interpolation_pot = std::make_unique<InterpolationPot>(2, INTERPOLATION_POT, uart_libre_manager, hardware_manager);
        this->shift_button = std::make_unique<ShiftButton>(this->hardware_manager);
        this->interpolation_button_a = std::make_unique<InterpolationButton>(InterpolationButtonName::BUTTON_A, uart_libre_manager, hardware_manager);
        this->interpolation_button_b = std::make_unique<InterpolationButton>(InterpolationButtonName::BUTTON_B, uart_libre_manager, hardware_manager);
        this->preset_pot = std::make_unique<PresetPot>(PRESET_KNOB, hardware_manager);
        this->save_button = std::make_unique<SaveButton>(this->preset_pot.get(), uart_libre_manager, hardware_manager);
        this->clear_button = std::make_unique<ClearButton>(uart_libre_manager, hardware_manager, input_buffer_manager);
        this->uncertainty_cv_input = std::make_unique<GPCV>(2, UNCERTAINTY_CV, uart_libre_manager, hardware_manager);
        this->uncertainty_pot = std::make_unique<GeneralPurposeParameterPot>(1, UNCERTAINTY_POT, uart_libre_manager, hardware_manager, this->uncertainty_cv_input.get(), ModelParameter::UNCERTAINTY);
        this->gp_cv_1 = std::make_unique<GPCV>(2, GP_CV_1, uart_libre_manager, hardware_manager);
        this->gp_cv_2 = std::make_unique<GPCV>(2, GP_CV_2, uart_libre_manager, hardware_manager);
        this->gp_cv_3 = std::make_unique<GPCV>(2, GP_CV_3, uart_libre_manager, hardware_manager);
        this->general_purpose_parameter_pot_1 = std::make_unique<GeneralPurposeParameterPot>(2, GP_POT_1, uart_libre_manager, hardware_manager, this->gp_cv_1.get(), ModelParameter::GENERAL_PURPOSE);
        this->general_purpose_parameter_pot_2 = std::make_unique<GeneralPurposeParameterPot>(2, GP_POT_2, uart_libre_manager, hardware_manager, this->gp_cv_2.get(), ModelParameter::GENERAL_PURPOSE);
        this->general_purpose_parameter_pot_3 = std::make_unique<GeneralPurposeParameterPot>(2, GP_POT_3, uart_libre_manager, hardware_manager, this->gp_cv_3.get(), ModelParameter::GENERAL_PURPOSE);
        this->groove_gate = std::make_unique<GrooveGate>(GROOVE_TRIGGER_IN, hardware_manager);
        this->groove_cv = std::make_unique<GrooveCV>(GROOVE_VELOCITY_IN, hardware_manager);
        this->cv_clock_input = std::make_unique<CVClockInput>(CLOCK_IN, hardware_manager);
        this->record_overdub_switch = std::make_unique<RecordOverdubSwitch>(uart_libre_manager, input_buffer_manager, hardware_manager);
        this->play_stop_switch = std::make_unique<PlayStopSwitch>(hardware_manager, clock_manager, uart_midi_manager);
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
        if(this->uart_libre_manager->SEND_ALL_UI_PARAMS) {
            this->uart_libre_manager->SEND_ALL_UI_PARAMS = false;
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
};
