#include "../GlobalHelpers/GlobalParameters.h"
#pragma once

struct MidiManager {
    MidiUsbHandler* usb_midi;
    MidiUartHandler* uart_midi;
    OutputBufferManager* output_buffer_manager;
    InputBufferManager* input_buffer_manager;
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
        ClockManager* clock_manager,
        HardwareManager* hardware_manager){
            this->usb_midi = usb_midi;
            this->uart_midi = uart_midi;
            this->output_buffer_manager = output_buffer_manager;
            this->input_buffer_manager = input_buffer_manager;
            this->clock_manager = clock_manager;
            this->hardware_manager = hardware_manager;
    };

    void HandleIncomingMidi(){
        // //** USB MIDI Listener */
        // usb_midi->Listen(); // listen to MIDI for new changes
        // while(usb_midi->HasEvents()) { // when there are messages waiting in the queue...
        //     HandleMidiMessage();
        // }
        //** UUART MIDI Listener */
        uart_midi->Listen(); // listen to MIDI for new changes
        while(uart_midi->HasEvents()) { // when there are messages waiting in the queue...
            HandleMidiMessage();
        }
    }

    void HandleMidiMessage()
    {
        auto msg = uart_midi->PopEvent();  // pull the oldest message from the list
        switch(msg.type)
        {
            case NoteOn:
            {
                auto note_msg = msg.AsNoteOn();
                if (note_msg.velocity != 0) {

                    MIDISendNoteOn(12, note_msg.note, note_msg.velocity);

                    // Send MIDI CC message on channel 1, controller 1, value 64
                    // SendMidiCC(2, 10, 99);
                    // // System::Delay(1);
                    // SendMidiCC(5, 43, 81);
                    // MIDISendNoteOn(12, 50, note_msg.velocity);
                    // MIDISendNoteOn(3, 89, note_msg.velocity);
                } 
            }
            break;
            case NoteOff:
            {
                auto note_msg = msg.AsNoteOff();
                MIDISendNoteOff(12, note_msg.note, note_msg.velocity);
            }
            break;
            case SystemRealTime:
            {
                HandleSystemRealTime(msg.srt_type);
            }
            break;
            // For now ignore all other message types
            default: break;
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

    void SendMidiCC(uint8_t channel, uint8_t cc_number, uint8_t value) {
        // Ensure that the channel is within the valid MIDI channel range (1-16)
        if (channel < 1 || channel > 16) {
            // Handle invalid channel
            return;
        }

        // Ensure that the cc_number and value are within valid MIDI ranges (0-127)
        if (cc_number > 127 || value > 127) {
            // Handle invalid cc_number or value
            return;
        }

        // Calculate the status byte for the MIDI CC message
        uint8_t statusByte = 0xB4; // 0xB0 is the status byte for MIDI CC on channel 1

        // Construct the MIDI CC message
        uint8_t data[3] = { statusByte, cc_number, value };

        // Send the MIDI CC message
        uart_midi->SendMessage(data, 3);
    }

    void SendMidiClock(bool clock_high) {
         uint8_t data[1] = { 0xF8 };
         if (clock_high) {
            uart_midi->SendMessage(data, 1);
         }
    }
    
    void MIDISendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
        // Ensure that the channel is within the valid MIDI channel range (1-16)
        if (channel < 1 || channel > 16) {
            return;
        }

        // Ensure that the note and velocity are within valid MIDI ranges (0-127)
        if (note > 127 || velocity > 127) {
            return;
        }

        uint8_t data[3] = { 0 };
        
        data[0] = ((channel - 1) & 0x0F) + 0x90;  // limit channel byte, add status byte
        data[1] = note & 0x7F;              // remove MSB on data
        data[2] = velocity & 0x7F;

        uart_midi->SendMessage(data, 3);
    }

    void MIDISendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
        // Ensure that the channel is within the valid MIDI channel range (1-16)
        if (channel < 1 || channel > 16) {
            return;
        }

        // Ensure that the note and velocity are within valid MIDI ranges (0-127)
        if (note > 127 || velocity > 127) {
            return;
        }

        uint8_t data[3] = { 0 };

        data[0] = ((channel - 1) & 0x0F) + 0x80;  // limit channel byte, add status byte
        data[1] = note & 0x7F;              // remove MSB on data
        data[2] = velocity & 0x7F;

        uart_midi->SendMessage(data, 3);
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

};
