#include "GrooveModule.h"
using namespace daisy;

TimerHandle         hardware_clock;
TimerHandle::Config hardware_clock_cfg;
Logger<LOGGER_SEMIHOST> logger;
bool clock_high = true;
int c_count = 0;

MidiUsbHandler usb_midi;
MidiUartHandler uart_midi;
DaisySeed hw;
I2CHandle i2c;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for(size_t i = 0; i < size; i++)
    {
        clock_manager->ObservePlayState();
        ui_components_manager->PlayReadAndSetState();

    }
}

void sendClockPulse(){
    // TODO make sure the rate that it sends clock is correct. currently it's divided 2 clock
    // TODO ties this with main clock start/stop
    if (clock_manager->play_enabled) {
        hardware_manager->clock_out.Write(clock_high);
        midi_manager->SendMidiClock(clock_high);
        // playback_manager->TriggerOutputs();
        clock_manager->AdvanceClockIndex();
        
        if(c_count % 4800 == 0) {
            c_count = 0;
        } else {
            c_count++;
        }
        clock_high = !clock_high;
    }
}

void ClockTimerCallback(void* data){ sendClockPulse(); }  

int main(void)
{
     // ** Init Hardware Manager */
    hardware_manager = std::make_unique<HardwareManager> (&hw, &i2c);

    // // ** Initialize logger */
	// hardware_manager->hw->StartLog(true);
    // hardware_manager->hw->PrintLine("DAISY ONLINE");

    MidiUartHandler::Config uart_midi_config;
    uart_midi.Init(uart_midi_config);
    uart_midi.StartReceive();

    // MidiUsbHandler::Config usb_midi_cfg;
    // usb_midi_cfg.transport_config.periph = MidiUsbTransport::Config::INTERNAL;
    // usb_midi.Init(usb_midi_cfg);

    // ** Init Managers */
    clock_manager = std::make_unique<ClockManager> (&hardware_clock, hardware_manager.get());
    output_buffer_manager = std::make_unique<OutputBufferManager> (hardware_manager.get());
    input_buffer_manager = std::make_unique<InputBufferManager> (clock_manager.get(), hardware_manager.get());
    
    midi_manager = std::make_unique<MidiManager> (
        &usb_midi,
        &uart_midi,
        output_buffer_manager.get(), 
        input_buffer_manager.get(), 
        clock_manager.get(),
        hardware_manager.get()
    );

    playback_manager = std::make_unique<PlaybackManager>(
        midi_manager.get(), 
        clock_manager.get(), 
        output_buffer_manager.get(), 
        hardware_manager.get()
    );

    // ** Set up hardware_clock for external syncing */
    hardware_clock_cfg.periph     = TimerHandle::Config::Peripheral::TIM_5;
    hardware_clock_cfg.dir        = TimerHandle::Config::CounterDir::UP;
    hardware_clock_cfg.enable_irq = true;
    float period                  = internal_clock_freq_hz/clock_period_freq_hz;
    hardware_clock_cfg.period     = static_cast<uint32_t>(period);
    hardware_clock.Init(hardware_clock_cfg);
    hardware_clock.SetCallback(ClockTimerCallback);
    
    ui_components_manager = std::make_unique<UIComponentsManager> (
        hardware_manager.get(),
        input_buffer_manager.get(),
        output_buffer_manager.get(),
        clock_manager.get(),
        midi_manager.get()
    );

    hardware_clock.Start();

    //** Initialize Audio Callback */
	hardware_manager->hw->SetAudioBlockSize(4); // number of samples handled per callback
	hardware_manager->hw->SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    // ** Start Audio Callback */
	hardware_manager->hw->StartAudio(AudioCallback);

    // loop forever
    while(1) {
        /** Update all cv inputs */
        if (clock_manager->ticks_from_start_idx % 20 == 0) {
            ui_components_manager->ReadUIComponents();
        }

        ui_components_manager->ObserveSAPMessage();
        input_buffer_manager->ObserveSAPMessage();
        input_buffer_manager->ObserveRecordBuffer(); //This must be called before Midi is Handled
        midi_manager->HandleIncomingMidi();

    }
}
