#pragma once

struct UartMidiManager {
    MidiUartHandler* uart_midi;
    
    OutputBufferManager* output_buffer_manager;
    InputBufferManager* input_buffer_manager;
    UartLibreManager* uart_libre_manager;
    ClockManager* clock_manager;
    HardwareManager* hardware_manager;

    /** Incoming MIDI variables */
    bool is_onset                = true; // flag to only capture onset of a midi note
    bool midi_note_is_on         = false; // flag to know if MIDI notes is currently on

    UartMidiManager(
        MidiUartHandler* uart_midi,
        OutputBufferManager* output_buffer_manager, 
        InputBufferManager* input_buffer_manager,
        UartLibreManager* uart_libre_manager, 
        ClockManager* clock_manager,
        HardwareManager* hardware_manager){
            this->uart_midi = uart_midi;
            this->output_buffer_manager = output_buffer_manager;
            this->input_buffer_manager = input_buffer_manager;
            this->uart_libre_manager = uart_libre_manager;
            this->clock_manager = clock_manager;
            this->hardware_manager = hardware_manager;
    };

    void HandleMidiUart(){
        //** MIDI Listener */
        uart_midi->Listen(); // listen to MIDI for new changes
        while(uart_midi->HasEvents()) { // when there are messages waiting in the queue...
            auto midi_event = uart_midi->PopEvent();  // pull the oldest one from the list
            HandleMidiMessage(midi_event);
        }
    }

    void HandleMidiMessage(MidiEvent midi_event)
    {
        /** Pull the oldest one from the list... */
        switch(midi_event.type)
        {
            case NoteOn:
            {
                // midi information
                auto note_msg = midi_event.AsNoteOn();
                float input_velocity = note_msg.velocity/127.f;
                midi_note_is_on = note_msg.velocity != 0;

                // only handle midi notes if play is enabled
                if(clock_manager->play_enabled && (this->input_buffer_manager->rec_enabled || this->input_buffer_manager->overdub_enabled)) {
                    // if these conditions are true, write the MIDI data and the onset to their output_buffer_one
                    if (is_onset && midi_note_is_on) {
                        std::pair<int, int> current_step_index_and_offset_ticks = this->clock_manager->GetCurrentStepIndexAndOffsetTicks();
                        if (this->input_buffer_manager->CreateAndAddOnsetToInputBuffer(current_step_index_and_offset_ticks.first, input_velocity, current_step_index_and_offset_ticks.second)) {
                            Onset onset = this->input_buffer_manager->AddOnset(current_step_index_and_offset_ticks.first, input_velocity, current_step_index_and_offset_ticks.second);
                            this->uart_libre_manager->TransmitOnset(onset);
                            hardware_manager->hw->PrintLine("Transmitted NEW Onset");
                        }
                        is_onset = false; // reset is_onset to false after we process an onset here. it goes back to true once the midi note ends
                    }

                    // some hardware instead of sending NoteOff sends NoteOn velocity 0. This handles that
                    if (note_msg.velocity == 0) {
                        is_onset = true; // if there is no incoming MIDI note then the next midi note on is an onset
                    }
                } 
                is_onset = true; // if play is not enabled while receiving midi note then the next receive midi note with play enabaled is an onset
            }
            break;
            case NoteOff:
            {
                is_onset = true; // if there is no incoming MIDI note then the next message received is an onset
            }
            break;
            // For now ignore all other message types
            default: {}
            break;
        }
    }


    // void SendMidiOutputs(int voice_idx, int velocity_int) {
        
    //     uint8_t channel = voice_idx;
    //     uint8_t general_channel = 10;
    //     uint8_t velocity = velocity_int;
    //     uint8_t velocity_off = 0;
    //     uint8_t note = 0;
    //     uint8_t part_level_cc = 0;
    //     uint8_t data[3] = { 0 };
    //     uint8_t data_gen[3] = { 0 };

    //     if (voice_idx == 0) {
    //         //kick
    //         note = 36;
    //         part_level_cc = 40;
    //     } else if (voice_idx == 1) {
    //         //snare
    //         note = 38;
    //         part_level_cc = 41;
    //     } else if (voice_idx == 2) {
    //         //closed hat
    //         note = 42;
    //         part_level_cc = 44;
    //     } else if (voice_idx == 3) {
    //         //open hat
    //         note = 46;
    //         part_level_cc = 45;
    //     } else if (voice_idx == 4) {
    //         //tom
    //         note = 43;
    //         part_level_cc = 42;
    //     } else if (voice_idx == 5) {
    //         //ride
    //         note = 51;
    //     }

    //     // //** NOTE ON*//
    //     data[0] = (channel & 0x0F) + 0x90;  // limit channel byte, add status byte
    //     data[1] = note & 0x7F;              // remove MSB on data
    //     data[2] = velocity & 0x7F;
    //     uart_midi->SendMessage(data, 3);

    //     //** NOTE ON GENERAL MIDI MAPPING* //
    //     data_gen[0] = (general_channel & 0x0F) + 0x90;  // limit channel byte, add status byte
    //     data_gen[1] = note & 0x7F;              // remove MSB on data
    //     data_gen[2] = velocity & 0x7F;
    //     // TODO MAYBE NEED TO SEPARATE CC TO ITS OWN LIKE WE DO FOR NOTE ON OFF
    //     // data_gen[3] = 0xB0;              // CC
    //     // data_gen[4] = part_level_cc & 0x7F;
    //     // data_gen[5] = velocity & 0x7F;
    //     // uart_midi->SendMessage(data, 3);

    //     // //** NOTE OFF*//
    //     data[0] = (channel & 0x0F) + 0x80;  // limit channel byte, add status byte
    //     data[2] = velocity_off & 0x7F;
    //     uart_midi->SendMessage(data, 3);

    //     //** NOTE OFF GENERAL MIDI MAPPING*//
    //     data[0] = (general_channel & 0x0F) + 0x80;  // limit channel byte, add status byte
    //     data[2] = velocity_off & 0x7F;
    //     uart_midi->SendMessage(data, 3);
    // }

    void SendMidiOutputs(int voice_idx, int velocity_int) {
        
        uint8_t channel = voice_idx;
        uint8_t general_channel_idx = 9;
        uint8_t velocity = velocity_int;
        uint8_t velocity_off = 0;
        uint8_t note = 0;
        uint8_t part_level_cc;
        uint8_t data[3] = { 0 };
        uint8_t data_cc[3] = { 0 };

        if (voice_idx == 0) {
            //kick
            note = 36;
            part_level_cc = 40;
        } else if (voice_idx == 1) {
            //snare
            note = 38;
            part_level_cc = 41;
        } else if (voice_idx == 2) {
            //closed hat
            note = 42;
            part_level_cc = 44;
        } else if (voice_idx == 3) {
            //open hat
            note = 46;
            part_level_cc = 45;
        } else if (voice_idx == 4) {
            //volva lo tom
            note = 43;
            part_level_cc = 45;
        } else if (voice_idx == 5) {
            //ride
            //volca clap
            note = 39;
            part_level_cc = 46;
        }

        //** NOTE ON*//
        data[0] = (channel & 0x0F) + 0x90;  // limit channel byte, add status byte
        data[1] = note & 0x7F;              // remove MSB on data
        data[2] = velocity & 0x7F;
        uart_midi->SendMessage(data, 3);

        //** NOTE ON GENERAL MIDI MAPPING*//
        data[0] = (general_channel_idx & 0x0F) + 0x90;  // limit channel byte, add status byte
        uart_midi->SendMessage(data, 3);  

         // TODO MAYBE NEED TO SEPARATE CC TO ITS OWN LIKE WE DO FOR NOTE ON OFF
        data_cc[0] = (general_channel_idx & 0x0F) + 0xB0;  // limit channel byte, add status byte
        data_cc[1] = part_level_cc & 0x7F;
        data_cc[2] = velocity & 0x7F;
        uart_midi->SendMessage(data_cc, 3);

        // // TODO MAYBE NEED TO SEPARATE CC TO ITS OWN LIKE WE DO FOR NOTE ON OFF
        // data_cc[0] = (general_channel_idx & 0x0F) + 0xB0;  // limit channel byte, add status byte
        // data_cc[1] = 0xB0;              // CC
        // data_cc[2] = part_level_cc & 0x7F;
        // data_cc[3] = velocity & 0x7F;
        // uart_midi->SendMessage(data_cc, 4);

        //** NOTE OFF*//
        data[0] = (channel & 0x0F) + 0x80;  // limit channel byte, add status byte
        data[2] = velocity_off & 0x7F;
        uart_midi->SendMessage(data, 3);

        //** NOTE OFF GENERAL MIDI MAPPING*//
        data[0] = (general_channel_idx & 0x0F) + 0x80;  // limit channel byte, add status byte
        data[2] = velocity_off & 0x7F;
        uart_midi->SendMessage(data, 3);
    }
};
