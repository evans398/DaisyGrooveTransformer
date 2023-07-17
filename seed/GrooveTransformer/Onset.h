#pragma once

struct Onset {

    ClockManager* clock_manager;

    float step_offset_value;
    int onset_loop_count;
    
    //** Velocities */
    float input_velocity;
    float scaled_velocity;

    //** Onset time information */
    int ticks_from_start;
    int step;
    int step_offset_ticks;

    Onset(int current_step, float input_velocity, int offset_ticks, ClockManager* clock_manager){
        //** Timing params */
        this->step              = current_step;
        this->step_offset_ticks = offset_ticks;
        //** Velocity params */
        this->input_velocity    = input_velocity; // onset velocity
        this->scaled_velocity   = input_velocity; // initialize scaled velocity to input
        float ticks_per_step    = ppqn / 4.f; // a step is a 16th note so we need pulses per sixteenth note
        this->ticks_from_start  = clock_manager->ticks_from_start_idx;
        this->step_offset_value = step_offset_ticks / ticks_per_step;
        this->onset_loop_count  = clock_manager->loop_count;
        this->clock_manager = clock_manager;
    }
};