#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/MidiManager.h"
#include "MuxInput.h"

struct CVInput : MuxInput {
    CVInput(
        int midi_cc, 
        int mux_channel, 
        int mux_idx, 
        HardwareManager* hardware_manager,
        MidiManager* midi_manager
    ) : MuxInput(midi_cc, mux_channel, mux_idx, hardware_manager, midi_manager){
        // default constructor
    }
};