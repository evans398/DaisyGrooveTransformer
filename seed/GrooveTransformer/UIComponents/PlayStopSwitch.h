#pragma once
#include "../Managers/HardwareManager.h"
#include "../Managers/ClockManager.h"
#include "../Managers/MidiManager.h"

struct PlayStopSwitch {
    HardwareManager* hardware_manager;
    ClockManager* clock_manager;
    MidiManager* midi_manager;
    bool play_enabled;


    PlayStopSwitch(HardwareManager* hardware_manager, MidiManager* midi_manager, ClockManager* clock_manager) {
        this->hardware_manager =  hardware_manager;
        this->clock_manager = clock_manager;
        this->midi_manager = midi_manager;
        hardware_manager->play_stop_switch.Debounce();
        play_enabled = hardware_manager->play_stop_switch.Pressed();
        if (play_enabled) {
            midi_manager->SendMidiPlayStop(play_enabled);
            clock_manager->play_enabled = hardware_manager->play_stop_switch.Pressed();
        } else {
            clock_manager->play_enabled = hardware_manager->play_stop_switch.Pressed();
            midi_manager->SendMidiPlayStop(play_enabled);
        }
    }

    void ReadAndSetState() {
        hardware_manager->play_stop_switch.Debounce();
        if (hardware_manager->play_stop_switch.Pressed() && !play_enabled) {
            midi_manager->SendMidiPlayStop(play_enabled);
            play_enabled = hardware_manager->play_stop_switch.Pressed();
            clock_manager->play_enabled = hardware_manager->play_stop_switch.Pressed();
        }
        if (!hardware_manager->play_stop_switch.Pressed() && play_enabled) {
            play_enabled = hardware_manager->play_stop_switch.Pressed();
            clock_manager->play_enabled = hardware_manager->play_stop_switch.Pressed();
            midi_manager->SendMidiPlayStop(play_enabled);
        }
    }
};
