#pragma once

/** Clock Params */
//******************************************************************************************/
int clock_out_tick_count = 0;
int clock_out_tick_rate  = 4; //one clock out every 4 ticks is 24ppqn at global 96ppqn

/** Tempo parameters */
//******************************************************************************************/
float bpm                       = 120.0f; // initialize BPM to 120
float bps                       = bpm / 60.0f; // convert to beats per second
const float ppqn                = 48.0f; // init pulses
float clock_period_freq_hz             = ppqn * bpm * (1.0f/60.f); // clock frequency at ppqn resolution (1/60) is hz/bpm (so 120bpm is 2hz). One is a q note so we multiply by ppqn
const float internal_clock_freq_hz = 200000000; // can be confirmed with hardware_clock.GetFreq()

/** Rec loop buffer parameters */
//******************************************************************************************/
const int loop_length_qn     = 8; // number of quarter notes in loop (8 for 2 bars)
const int step_buffer_length = loop_length_qn * 4; // number of 16th note steps in loop
const int buffer_size_ticks  = ppqn * loop_length_qn; // number of ticks in the loop buffer
const int ticks_per_step           = ppqn / 4; // a step is a 16th note so we need pulses per sixteenth note
int ticks_per_half_step      = ticks_per_step / 2; // ticks per half step is used for calculating offset amounts

/** Playback output_buffers */
//******************************************************************************************/
const int output_buffer_size = ppqn * loop_length_qn; // output buffers have an idx for each tick in a 2 bar loop at defined ppqn
const int NUM_OUTPUT_VOICES = 6; // model generates HVO sequence for 6 voices
const int NUM_DAC_OUTPUT_VOICES = 6; // model generates HVO sequence for 6 voices