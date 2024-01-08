#include "../GlobalHelpers/GlobalParameters.h"
#pragma once

struct MidiManager {
    MidiUsbHandler* usb_midi;
    MidiUartHandler* uart_midi;
    
    OutputBufferManager* output_buffer_manager;
    InputBufferManager* input_buffer_manager;
    UartLibreManager* uart_libre_manager;
    ClockManager* clock_manager;
    HardwareManager* hardware_manager;

    uint32_t prev_ms = 0;
    uint16_t tt_count = 0;

    // float    bpm_to_freq(uint32_t tempo);
    // uint32_t ms_to_bpm(uint32_t ms);
    // void HandleSystemRealTime(SystemRealTimeType srt_type);

    /** Incoming MIDI variables */
    bool is_onset                = true; // flag to only capture onset of a midi note
    bool midi_note_is_on         = false; // flag to know if MIDI notes is currently on

    MidiManager(
        MidiUsbHandler* usb_midi,
        MidiUartHandler* uart_midi,
        OutputBufferManager* output_buffer_manager, 
        InputBufferManager* input_buffer_manager,
        UartLibreManager* uart_libre_manager, 
        ClockManager* clock_manager,
        HardwareManager* hardware_manager){
            this->usb_midi = usb_midi;
            this->uart_midi = uart_midi;
            this->output_buffer_manager = output_buffer_manager;
            this->input_buffer_manager = input_buffer_manager;
            this->uart_libre_manager = uart_libre_manager;
            this->clock_manager = clock_manager;
            this->hardware_manager = hardware_manager;
    };

    void HandleMidiUart(){
        //** MIDI Listener */
        usb_midi->Listen(); // listen to MIDI for new changes
        while(usb_midi->HasEvents()) { // when there are messages waiting in the queue...
            auto midi_event = usb_midi->PopEvent();  // pull the oldest one from the list
            HandleMidiMessage(midi_event);
        }
    }

    float bpm_to_freq(uint32_t tempo)
    {
        return tempo / 60.0f;
    }

    uint32_t ms_to_bpm(uint32_t ms)
    {
        return 60000 / ms;
    }   

    void HandleSystemRealTime(SystemRealTimeType srt_type){
        switch(srt_type)
        {
            // 0xFA - start
            case Start: break;

            // 0xFC - stop
            case Stop: break;

    // #ifdef DEBUG
    //         default:
    //             hw.seed.PrintLine("MIDI SystemRealTime: %x", m.srt_type);
    //             break;
    // #endif

            // MIDI Clock -  24 clicks per quarter note
            case TimingClock:
                tt_count++;
                if(tt_count == 24)
                {
                    uint32_t ms   = System::GetNow();
                    uint32_t diff = ms - prev_ms;
                    uint32_t new_bpm  = ms_to_bpm(diff);
                    bpm = new_bpm;
    // #ifdef DEBUG
    //                 hw.seed.PrintLine("msec=%d, diff=%d, BPM=%d", ms, diff, bpm);
    // #endif
                    // if(bpm >= TTEMPO_MIN && bpm <= TTEMPO_MAX)
                    // {
                    //     lfo.SetFreq(bpm_to_freq(bpm));
                    // }

                    prev_ms = ms;
                    tt_count = 0;
                }
                break;
        }
    }

    void HandleMidiMessage(MidiEvent midi_event)
    {
        /** Pull the oldest one from the list... */
        int midi_type_enum_value = static_cast<int>(midi_event.type);
        hardware_manager->hw->PrintLine("midi_event.type: %d", midi_event.type);
        switch(midi_event.type)
        {
            case NoteOn:
            {
                // hardware_manager->hw->Print("RECEIVED MIDI NOTE");
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
            case SystemRealTime:
            {
                HandleSystemRealTime(midi_event.srt_type);
            }
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

    void SendMidiClock(bool clock_high) {
         uint8_t data[1] = { 0xF8 };
         if (clock_high) {
            usb_midi->SendMessage(data, 1);
         }
    }

    void SendMidiPlayStop(bool play_enabled) {
        // if (play_enabled) {
        //     uint8_t data[1] = { 0xFA };
        //     uart_midi->SendMessage(data, 1);
        // } else {
        //     uint8_t data[1] = { 0xFC };
        //     uart_midi->SendMessage(data, 1);
        // }     
    }

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
        // data[0] = (general_channel_idx & 0x0F) + 0x90;  // limit channel byte, add status byte
        // uart_midi->SendMessage(data, 3);  

        //  // TODO MAYBE NEED TO SEPARATE CC TO ITS OWN LIKE WE DO FOR NOTE ON OFF
        // data_cc[0] = (general_channel_idx & 0x0F) + 0xB0;  // limit channel byte, add status byte
        // data_cc[1] = part_level_cc & 0x7F;
        // data_cc[2] = velocity & 0x7F;
        // uart_midi->SendMessage(data_cc, 3);

        // // // TODO MAYBE NEED TO SEPARATE CC TO ITS OWN LIKE WE DO FOR NOTE ON OFF
        // // data_cc[0] = (general_channel_idx & 0x0F) + 0xB0;  // limit channel byte, add status byte
        // // data_cc[1] = 0xB0;              // CC
        // // data_cc[2] = part_level_cc & 0x7F;
        // // data_cc[3] = velocity & 0x7F;
        // // uart_midi->SendMessage(data_cc, 4);

        // //** NOTE OFF*//
        data[0] = (channel & 0x0F) + 0x80;  // limit channel byte, add status byte
        data[2] = velocity_off & 0x7F;
        uart_midi->SendMessage(data, 3);

        //** NOTE OFF GENERAL MIDI MAPPING*//
        // data[0] = (general_channel_idx & 0x0F) + 0x80;  // limit channel byte, add status byte
        // data[2] = velocity_off & 0x7F;
        // uart_midi->SendMessage(data, 3);
    }
};
