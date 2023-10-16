// #pragma once

// using namespace daisy;
// using namespace daisysp;

// // /** Clock static parameters */
// // float bpm                       = 120.0f; // initialize BPM to 120
// // float bps                       = bpm / 60.0f; // convert to beats per second
// // const float ppqn                = 8; // init pulses
// // float clock_freq_hz             = ppqn * bpm * (1.0f/60.f); // clock frequency at ppqn resolution (1/60) is hz/bpm (so 120bpm is 2hz). One is a q note so we multiply by ppqn
// // const float internal_clock_freq_hz = 200000000; // can be confirmed with internal_clock.GetFreq()

// // /** Rec loop buffer static parameters */
// // const int loop_length_qn     = 8; // number of quarter notes in loop (8 for 2 bars)
// // const int step_buffer_length = loop_length_qn * 4; // number of 16th note steps in loop
// // const int buffer_size_ticks  = ppqn * loop_length_qn; // number of ticks in the loop buffer
// // int ticks_per_step           = ppqn / 4; // a step is a 16th note so we need pulses per sixteenth note
// // int ticks_per_half_step      = ticks_per_step / 2; // ticks per half step is used for calculating offset amounts
// // int clock_out_tick_rate      = 4; //one clock out every 4 ticks is 24ppqn at global 96ppqn

// struct HardwareClock {
//     /** Clock */
//     TimerHandle internal_clock;
//     bool led_on = true;

//     HardwareManager* hardware_manager;

//     /** Clock Params */
//     bool play_enabled = true;

//     HardwareClock(TimerHandle* internal_clock, HardwareManager* hardware_manager){
//         this->internal_clock = internal_clock;
//         this->hardware_manager = hardware_manager;

//         // ** Set up internal_clock for external syncing */
//         TimerHandle::Config internal_clock_cfg;
//         internal_clock_cfg.periph     = TimerHandle::Config::Peripheral::TIM_5;
//         internal_clock_cfg.enable_irq = true;
//         float period                  = internal_clock_freq_hz/clock_freq_hz;
//         internal_clock_cfg.period     = static_cast<uint32_t>(period);
//         this->internal_clock.Init(internal_clock_cfg);
//         this->internal_clock.SetCallback(ClockTimerCallback);
//         this->internal_clock.Start();
//     }

//     void sendClockPulse(){
//         // TODO make sure the rate that it sends clock is correct. currently it's divided 2 clock
//         // TODO ties this with main clock start/stop
//         if (this->play_enabled) {
//             hardware_manager->clock_out.Write(led_on);
//             led_on = !led_on;
//         }
//     }

//     void ClockTimerCallback(void* data){ sendClockPulse(); }  

//     void UpdatePeriod(){
//         // float period = static_cast<uint32_t>(internal_clock_freq_hz/clock_freq_hz);
//         this->internal_clock->SetPeriod(period);
//     }

   
// };
