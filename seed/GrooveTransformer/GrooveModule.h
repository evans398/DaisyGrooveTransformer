#include "daisy_seed.h"
#include "daisysp.h"
#include <memory>

#include "Managers/HardwareManager.h"
#include "Managers/ClockManager.h"
#include "Managers/OutputBufferManager.h"
#include "InputOutputBufferStructs/Onset.h"
#include "Managers/UartLibreManager.h"
#include "Managers/InputBufferManager.h"
#include "Managers/MidiManager.h"
#include "Managers/UIComponentsManager.h"
#include "Managers/PlaybackManager.h"

bool LIBRE_IS_READY = false;

std::unique_ptr<HardwareManager> hardware_manager;
std::unique_ptr<ClockManager> clock_manager;
std::unique_ptr<OutputBufferManager> output_buffer_manager;
std::unique_ptr<UartLibreManager> uart_libre_manager;
std::unique_ptr<InputBufferManager> input_buffer_manager;
std::unique_ptr<UIComponentsManager> ui_components_manager;
std::unique_ptr<MidiManager> uart_midi_manager;
std::unique_ptr<PlaybackManager> playback_manager;
