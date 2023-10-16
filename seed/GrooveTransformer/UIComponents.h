enum class InterpolationButtonName {
    BUTTON_A,
    BUTTON_B
};

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

int ScalePotValue(float value) {
    float scaled_value = value * 100.;
    int scaled_value_int = (int) scaled_value;
    return RoundScaledValue(scaled_value_int);
}

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
            hardware_manager->hw->Print("VOICE DENSITY POT %d ", voice_idx);
            hardware_manager->hw->PrintLine("value: %d", scaled_value);
            int voice_number = voice_idx + 1;
            uart_libre_manager->TransmitUIIndexedParameterValue(ModelParameter::VOICE_DENSITY, voice_number, scaled_value);
        }
        this->prev_scaled_value = scaled_value;
    }
};

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
            hardware_manager->hw->Print("VOICE VEL POT %d ", voice_idx);
            hardware_manager->hw->PrintLine("value: " FLT_FMT3, FLT_VAR3(scaled_value));
            this->output_buffer_manager->UpdateVoiceVelocityScale(this->voice_idx, scaled_value);
        }
        this->prev_scaled_value = scaled_value;
    }
};

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

struct InputGrooveOffsetPot {
    int mux_channel;
    int mux_idx;
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;
    float prev_scaled_value;

    InputGrooveOffsetPot(int mux_channel, int mux_idx, UartLibreManager* uart_libre_manager, HardwareManager* hardware_manager) {
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
        // TOO JUMPY
        // int scaled_value = GetScaledValue();
        int scaled_value = 0;
        if (scaled_value != prev_scaled_value || force_transmit) {
            // hardware_manager->hw->PrintLine("INPUT GROOVE OFFSET value: " FLT_FMT3, FLT_VAR3(scaled_value));
            // this->uart_libre_manager->TransmitUIParameterValue(ModelParameter::INPUT_GROOVE_OFFSET, scaled_value);
            this->uart_libre_manager->TransmitUIParameterValue(ModelParameter::INPUT_GROOVE_OFFSET, 0);
        }
        this->prev_scaled_value = scaled_value;
    }
};

struct UncertaintyPot {
    int mux_channel;
    int mux_idx;
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;
    float prev_scaled_value;
    // add cv input to this struct

    UncertaintyPot(int mux_channel, int mux_idx, UartLibreManager* uart_libre_manager, HardwareManager* hardware_manager) {
        this->mux_channel = mux_channel;
        this->mux_idx = mux_idx;
        this->uart_libre_manager = uart_libre_manager;
        this->hardware_manager = hardware_manager;
        prev_scaled_value = GetScaledValue();
    }

    void UpdateValueWithCvInput(float value_from_cv_input){
        //TODO knob becomes attentuator
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
            hardware_manager->hw->PrintLine("UNCERTAINTY value: " FLT_FMT3, FLT_VAR3(scaled_value));
            this->uart_libre_manager->TransmitUIParameterValue(ModelParameter::UNCERTAINTY, scaled_value);
        }
        this->prev_scaled_value = scaled_value;
    }
};

struct InterpolationPot {
    int mux_channel;
    int mux_idx;
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;
    float prev_scaled_value;

    InterpolationPot(int mux_channel, int mux_idx, UartLibreManager* uart_libre_manager, HardwareManager* hardware_manager) {
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
            hardware_manager->hw->PrintLine("INTERPOLATION value: " FLT_FMT3, FLT_VAR3(scaled_value));
            this->uart_libre_manager->TransmitUIParameterValue(ModelParameter::INTERPOLATION_POSITION, scaled_value);
        }
        this->prev_scaled_value = scaled_value;
    }
};

struct ShiftButton {
    HardwareManager* hardware_manager;
    bool prev = false;

    ShiftButton(HardwareManager* hardware_manager) {
        this->hardware_manager = hardware_manager;
    }

    ShiftButton(){};

    bool Pressed(){
        hardware_manager->shift_button.Debounce();
        bool pressed = hardware_manager->shift_button.Pressed();
        if (!pressed && prev) {
            hardware_manager->hw->PrintLine("SHIFT PRESSED");
        }
        prev = pressed;
        return hardware_manager->shift_button.Pressed();
    }
};

struct InterpolationButton {
    enum InterpolationButtonName interpolation_button_name;
    bool prev_state_pressed;
    float value;
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;

    InterpolationButton(
        InterpolationButtonName interpolation_button_name, 
        UartLibreManager* uart_libre_manager,
        HardwareManager* hardware_manager
    ) {
        this->interpolation_button_name = interpolation_button_name;
        this->uart_libre_manager = uart_libre_manager;
        this->hardware_manager = hardware_manager;
    }

    void TransmitNewValue(){
        if(interpolation_button_name == InterpolationButtonName::BUTTON_A) {
            this->hardware_manager->interpolation_button_a.Debounce();
            hardware_manager->shift_button.Debounce();
            if (this->hardware_manager->interpolation_button_a.Pressed() && !this->prev_state_pressed && !this->hardware_manager->shift_button.Pressed()) {
                hardware_manager->hw->PrintLine("INTERP A SAVED");
                this->uart_libre_manager->TransmitUIParameterState(ModelParameter::INTERPOLATION_SAVE_A);
                hardware_manager->BlinkLed(&hardware_manager->b_led, false);
            }
            if (this->hardware_manager->interpolation_button_a.Pressed() && !this->prev_state_pressed && this->hardware_manager->shift_button.Pressed()) {
                hardware_manager->hw->PrintLine("INTERP A RANDOM");
                this->uart_libre_manager->TransmitUIParameterState(ModelParameter::INTERPOLATION_RANDOM_A);
                hardware_manager->BlinkLed(&hardware_manager->b_led, true);
            }       
            this->prev_state_pressed = this->hardware_manager->interpolation_button_a.Pressed();
        } 
        if(interpolation_button_name == InterpolationButtonName::BUTTON_B) {
            this->hardware_manager->interpolation_button_b.Debounce();
            if (this->hardware_manager->interpolation_button_b.Pressed() && !this->prev_state_pressed && !this->hardware_manager->shift_button.Pressed()) {
                hardware_manager->hw->PrintLine("INTERP B SAVED");
                this->uart_libre_manager->TransmitUIParameterState(ModelParameter::INTERPOLATION_SAVE_B);
                hardware_manager->BlinkLed(&hardware_manager->b_led, false);
            }
            if (this->hardware_manager->interpolation_button_b.Pressed() && !this->prev_state_pressed && this->hardware_manager->shift_button.Pressed()) {
                hardware_manager->hw->PrintLine("INTERP B RANDOM");
                this->uart_libre_manager->TransmitUIParameterState(ModelParameter::INTERPOLATION_RANDOM_B);
                hardware_manager->BlinkLed(&hardware_manager->b_led, true);
            }       
            this->prev_state_pressed = this->hardware_manager->interpolation_button_b.Pressed();
        } 
    }
};

struct PresetPot {
    int adc_channel;
    HardwareManager* hardware_manager;

    PresetPot(enum AdcChannel adc_channel, HardwareManager* hardware_manager) {
        this->adc_channel = adc_channel;
        this->hardware_manager = hardware_manager;
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetFloat(this->adc_channel);
    }

    int GetScaledValue(){
        return ScalePotValue(this->GetValue());
    }

    int GetCurrentPresetIndex() {
        int scaled_value = GetScaledValue();
        int preset_index = 0; 

        int preset_threshold_0 = 20;
        int preset_threshold_1 = 40;
        int preset_threshold_2 = 60;
        int preset_threshold_3 = 80;

        if (scaled_value <= preset_threshold_0) {
            preset_index = 0;
        } else if (scaled_value > preset_threshold_0 && scaled_value <= preset_threshold_1) {
            preset_index = 1;
        } else if (scaled_value > preset_threshold_1 && scaled_value <= preset_threshold_2) {
            preset_index = 2;
        } else if (scaled_value > preset_threshold_2 && scaled_value <= preset_threshold_3) {
            preset_index = 3;
        } else if (scaled_value > preset_threshold_3) {
            preset_index = 4;
        }
        hardware_manager->hw->PrintLine("Preset Index %d", preset_index);
        return preset_index;
    }
};

struct SaveButton {
    PresetPot* preset_pot;
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;
    bool prev_state_pressed;

    SaveButton(PresetPot* preset_pot, UartLibreManager* uart_libre_manager, HardwareManager* hardware_manager) {
        this->preset_pot = preset_pot;
        this->uart_libre_manager = uart_libre_manager;
        this->hardware_manager = hardware_manager;
        prev_state_pressed = hardware_manager->save_button.Pressed();
    }

    void TransmitNewValue(bool force_transmit){
        this->hardware_manager->save_button.Debounce();
        hardware_manager->shift_button.Debounce();
        if ((this->prev_state_pressed && !this->hardware_manager->save_button.Pressed()) || force_transmit) {
            hardware_manager->hw->PrintLine("SAVE BUTTON RELEASED");
            int preset_index = preset_pot->GetCurrentPresetIndex();
            if (hardware_manager->shift_button.Pressed() || force_transmit) {
                hardware_manager->hw->PrintLine("PRESET LOADED");
                this->uart_libre_manager->TransmitUIParameterValue(ModelParameter::LOAD_PRESET, preset_index);
                hardware_manager->BlinkLed(&hardware_manager->save_led, true);
            }
            if (!hardware_manager->shift_button.Pressed()) {
                hardware_manager->hw->PrintLine("PRESET SAVED");
                this->uart_libre_manager->TransmitUIParameterValue(ModelParameter::SAVE_PRESET, preset_index);
                hardware_manager->BlinkLed(&hardware_manager->save_led, false);
            }
        }
        this->prev_state_pressed = hardware_manager->save_button.Pressed();
    }
};

struct ClearButton {
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;
    InputBufferManager* input_buffer_manager;
    bool prev_state_pressed;

    ClearButton(UartLibreManager* uart_libre_manager, HardwareManager* hardware_manager, InputBufferManager* input_buffer_manager) {
        this->uart_libre_manager = uart_libre_manager;
        this->hardware_manager = hardware_manager;
        this->input_buffer_manager = input_buffer_manager;
        hardware_manager->clear_button.Debounce();
        prev_state_pressed = hardware_manager->clear_button.Pressed();
    }

    void TransmitNewValue(){
        hardware_manager->clear_button.Debounce();
        if (hardware_manager->clear_button.Pressed() && !prev_state_pressed) {
            hardware_manager->hw->PrintLine("CLEAR BUTTON PRESSED");
            uart_libre_manager->TransmitUIParameterState(ModelParameter::RESET_GROOVE);
            input_buffer_manager->ClearBuffer();
        }
        prev_state_pressed = hardware_manager->clear_button.Pressed();
    }
};

struct GeneralPurposeParameterPot {
    int mux_channel;
    int mux_idx;
    int general_purpose_pot_index;
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;
    float prev_scaled_value;
    //add cv input to this struct

    GeneralPurposeParameterPot(int mux_channel, int mux_idx, UartLibreManager* uart_libre_manager, HardwareManager* hardware_manager) {
        this->mux_channel=mux_channel;
        this->mux_idx=mux_idx;
        this->uart_libre_manager=uart_libre_manager;
        this->hardware_manager=hardware_manager;
        this->general_purpose_pot_index = mux_idx;
        prev_scaled_value = GetScaledValue();
    }

    void UpdateValueWithCvInput(float value_from_cv_input) {
        //TODO implement
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
            hardware_manager->hw->Print("GEN PURP %d", general_purpose_pot_index);
            hardware_manager->hw->PrintLine(" value: " FLT_FMT3, FLT_VAR3(scaled_value));
            this->uart_libre_manager->TransmitUIIndexedParameterValue(ModelParameter::GENERAL_PURPOSE, this->general_purpose_pot_index+1, scaled_value);
        }
        this->prev_scaled_value = scaled_value;
    }
};

struct RecordOverdubSwitch {
    UartLibreManager* uart_libre_manager;
    InputBufferManager* input_buffer_manager;
    HardwareManager* hardware_manager;
    bool rec_enabled;
    bool overdub_enabled;

    RecordOverdubSwitch(UartLibreManager* uart_libre_manager, InputBufferManager* input_buffer_manager, HardwareManager* hardware_manager) {
        this->uart_libre_manager = uart_libre_manager;
        this->input_buffer_manager = input_buffer_manager;
        this->hardware_manager = hardware_manager;
        hardware_manager->record_switch.Debounce();
        hardware_manager->overdub_switch.Debounce();
        rec_enabled = hardware_manager->record_switch.Pressed();
        overdub_enabled = hardware_manager->overdub_switch.Pressed();
    }

    void ReadAndSetState(){
        hardware_manager->record_switch.Debounce();
        hardware_manager->overdub_switch.Debounce();

        if ((overdub_enabled || rec_enabled) && !hardware_manager->record_switch.Pressed() && !hardware_manager->overdub_switch.Pressed()) {
            hardware_manager->hw->PrintLine("REC/OVR OFF");
            input_buffer_manager->rec_enabled = false;
            input_buffer_manager->overdub_enabled = false;
        }

        if (hardware_manager->record_switch.Pressed() && !rec_enabled) {
            hardware_manager->hw->PrintLine("REC ENABLED");
            input_buffer_manager->rec_enabled =true;
            input_buffer_manager->overdub_enabled = false;
        }

        if (hardware_manager->overdub_switch.Pressed() && !overdub_enabled) {
            hardware_manager->hw->PrintLine("OverDUB ENABLED");
            input_buffer_manager->rec_enabled = false;
            input_buffer_manager->overdub_enabled = true;
        }
        rec_enabled = hardware_manager->record_switch.Pressed();
        overdub_enabled = hardware_manager->overdub_switch.Pressed();
    }
};

struct PlayStopSwitch {
    HardwareManager* hardware_manager;
    ClockManager* clock_manager;
    UartMidiManager* uart_midi_manager;
    bool play_enabled;


    PlayStopSwitch(HardwareManager* hardware_manager, ClockManager* clock_manager, UartMidiManager* uart_midi_manager) {
        this->hardware_manager =  hardware_manager;
        this->clock_manager = clock_manager;
        this->uart_midi_manager = uart_midi_manager;
        hardware_manager->play_stop_switch.Debounce();
        play_enabled = hardware_manager->play_stop_switch.Pressed();
        if (play_enabled) {
            uart_midi_manager->SendMidiPlayStop(play_enabled);
            clock_manager->play_enabled = hardware_manager->play_stop_switch.Pressed();
        } else {
            clock_manager->play_enabled = hardware_manager->play_stop_switch.Pressed();
            uart_midi_manager->SendMidiPlayStop(play_enabled);
        }
    }

    void ReadAndSetState() {
        hardware_manager->play_stop_switch.Debounce();
        if (hardware_manager->play_stop_switch.Pressed() && !play_enabled) {
            // hardware_manager->hw->PrintLine("PLAY ENABLED");
            uart_midi_manager->SendMidiPlayStop(play_enabled);
            play_enabled = hardware_manager->play_stop_switch.Pressed();
            clock_manager->play_enabled = hardware_manager->play_stop_switch.Pressed();
        }
        if (!hardware_manager->play_stop_switch.Pressed() && play_enabled) {
            // hardware_manager->hw->PrintLine("PLAY DISABLED");
            play_enabled = hardware_manager->play_stop_switch.Pressed();
            clock_manager->play_enabled = hardware_manager->play_stop_switch.Pressed();
            uart_midi_manager->SendMidiPlayStop(play_enabled);
        }
    }
};

struct TempoPot {
    float tempo;
    int mux_channel;
    int mux_idx;
    ClockManager* clock_manager;
    HardwareManager* hardware_manager;
    HardwareClock* hardware_clock;
    int prev_value;

    TempoPot(
        int mux_channel,
        int mux_idx,
        ClockManager* clock_manager, 
        HardwareManager* hardware_manager,
        HardwareClock* hardware_clock
    ){
        this->mux_channel=mux_channel;
        this->mux_idx=mux_idx;
        this->clock_manager=clock_manager;
        this->hardware_manager=hardware_manager;
        this->hardware_clock=hardware_clock;
        ReadAndSetTempo();
    }

    float GetValue(){
        return this->hardware_manager->hw->adc.GetMuxFloat(this->mux_channel, this->mux_idx);
    }

    int ScaleTempoPotValue(float value) {
        float scaled_value = value * 100.;
        int scaled_value_int = (int) scaled_value;
        if(scaled_value > 98) {
            return 100;
        }
        if(scaled_value < 3) {
            return 0;
        }
        return scaled_value_int;
    }

    void ReadAndSetTempo() {
        int scaled_value = ScaleTempoPotValue(this->GetValue());
        if (scaled_value != prev_value) {
            float bpm = ((scaled_value/100.f) * 120.f) + 60.f; // min tempo is 60, max is 180
            clock_freq_hz = bpm * (1.0f/60.f) * ppqn;
            // hardware_manager->hw->PrintLine("BPM: " FLT_FMT3, FLT_VAR3(bpm));
            this->clock_manager->metro->SetFreq(clock_freq_hz);
            this->hardware_clock->UpdatePeriod(bpm);
        }
        prev_value = scaled_value;
    }
};