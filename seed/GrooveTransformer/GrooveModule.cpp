#include "GrooveModule.h"
using namespace daisy;

TimerHandle         hardware_clock;
TimerHandle::Config hardware_clock_cfg;
bool clock_high = true;
int c_count = 0;

MidiUartHandler uart_midi;
UartHandler uart_libre;
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
        uart_midi_manager->SendMidiClock(clock_high);
        playback_manager->TriggerOutputs();
        clock_manager->AdvanceClockIndex();
        // TODO: This needs to be implemented on this clock. at the moment it crashed the program
        
        if(c_count % 48 == 0) {
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
    // Configure the Uart Peripheral for Pi Communication
    UartHandler::Config uart_conf;
    uart_conf.periph        = UartHandler::Config::Peripheral::USART_2;
    uart_conf.mode          = UartHandler::Config::Mode::TX_RX;
    uart_conf.pin_config.tx = Pin(PORTA, 2);
    uart_conf.pin_config.rx = Pin(PORTA, 3);

    MidiUartHandler::Config uart_midi_config;
    uart_midi.Init(uart_midi_config); // Initialize the uart_libre peripheral and start the DMA transmit
    uart_midi.StartReceive(); // Start the FIFO Receive
    
    // // Initialize the uart_libre peripheral and start the DMA transmit
    // uart_libre.Init(uart_conf); 

    // /** Start the FIFO Receive */
    // uart_libre.DmaReceiveFifo();


    // // ** Initialize logger */
	// hardware_manager->hw->StartLog(true);
    // hardware_manager->hw->PrintLine("DAISY ONLINE");

    // ** Init Managers */
    clock_manager = std::make_unique<ClockManager> (&hardware_clock, hardware_manager.get());
    output_buffer_manager = std::make_unique<OutputBufferManager> (hardware_manager.get());
    uart_libre_manager = std::make_unique<UartLibreManager> (&uart_libre, output_buffer_manager.get(), hardware_manager.get());
    input_buffer_manager = std::make_unique<InputBufferManager> (uart_libre_manager.get(), clock_manager.get(), hardware_manager.get());
    
    uart_midi_manager = std::make_unique<UartMidiManager> (
        &uart_midi,
        output_buffer_manager.get(), 
        input_buffer_manager.get(), 
        uart_libre_manager.get(), 
        clock_manager.get(),
        hardware_manager.get()
    );

    playback_manager = std::make_unique<PlaybackManager>(
        uart_midi_manager.get(), 
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
        uart_libre_manager.get(), 
        input_buffer_manager.get(),
        output_buffer_manager.get(),
        clock_manager.get(),
        uart_midi_manager.get()
    );

    hardware_clock.Start();

    //** Initialize Audio Callback */
	hardware_manager->hw->SetAudioBlockSize(4); // number of samples handled per callback
	hardware_manager->hw->SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
    // ** Start Audio Callback */
	hardware_manager->hw->StartAudio(AudioCallback);

    std::string ng_string = "NG/";
    std::string gd_string = "GD/";
    uart_libre_manager->TestHandleLibreFifoMessage(ng_string);
    for (int idx = 0; idx<3; idx++) {
        std::string voice_idx = std::to_string(idx);
        std::string hit_string_2 = "H/" + voice_idx + "/7/12/0";
        std::string hit_string_3 = "H/" + voice_idx + "/13/127/0";
        std::string hit_string_4 = "H/" + voice_idx + "/19/80/0";
        std::string hit_string_1 = "H/" + voice_idx + "/1/45/0";
        std::string hit_string_5 = "H/" + voice_idx + "/20/90/0";
        std::string hit_string_6 = "H/" + voice_idx + "/21/10/0";
        std::string hit_string_7 = "H/" + voice_idx + "/28/127/0";
        std::string hit_string_8 = "H/" + voice_idx + "/29/110/0";
        uart_libre_manager->TestHandleLibreFifoMessage(hit_string_1);
        uart_libre_manager->TestHandleLibreFifoMessage(hit_string_2);
        uart_libre_manager->TestHandleLibreFifoMessage(hit_string_3);
        uart_libre_manager->TestHandleLibreFifoMessage(hit_string_4);
        uart_libre_manager->TestHandleLibreFifoMessage(hit_string_5);
        uart_libre_manager->TestHandleLibreFifoMessage(hit_string_6);
        uart_libre_manager->TestHandleLibreFifoMessage(hit_string_7);
        uart_libre_manager->TestHandleLibreFifoMessage(hit_string_8);
    }
    uart_libre_manager->TestHandleLibreFifoMessage(gd_string);


    // loop forever
    while(1) {
        /** Update all cv inputs */
        if (clock_manager->ticks_from_start_idx % 12 == 0) {
            ui_components_manager->ReadUIComponents();
        }

        // uart_libre_manager->HandleLibreUart();
        ui_components_manager->ObserveSAPMessage();
        input_buffer_manager->ObserveSAPMessage();
        input_buffer_manager->ObserveRecordBuffer(); //This must be called before Midi is Handled
        uart_midi_manager->HandleMidiUart();
    }
}
