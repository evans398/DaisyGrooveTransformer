#include "daisy_seed.h"
#include "daisysp.h"
#include <memory>

#include "HardwareManager.h"
#include "ClockManager.h"
#include "HardwareClock.h"
#include "OutputBufferManager.h"
#include "Onset.h"
#include "UartLibreManager.h"
#include "InputBufferManager.h"
#include "UIComponentsManager.h"
#include "UartMidiManager.h"
#include "PlaybackManager.h"


bool LIBRE_IS_READY = false;

std::unique_ptr<HardwareManager> hardware_manager;
std::unique_ptr<ClockManager> clock_manager;
// std::unique_ptr<HardwareClock> hardware_clock;
std::unique_ptr<OutputBufferManager> output_buffer_manager;
std::unique_ptr<UartLibreManager> uart_libre_manager;
std::unique_ptr<InputBufferManager> input_buffer_manager;
std::unique_ptr<UIComponentsManager> ui_components_manager;
std::unique_ptr<UartMidiManager> uart_midi_manager;
std::unique_ptr<PlaybackManager> playback_manager;
