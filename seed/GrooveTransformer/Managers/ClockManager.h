#include "../GlobalHelpers/GlobalParameters.h"
#pragma once

using namespace daisy;

struct ClockManager {
    /** Managers */
    TimerHandle* hardware_clock;
    HardwareManager* hardware_manager;

    /** Clock Params */
    bool play_enabled        = true;
    int ticks_from_start_idx = 0; // index to keep track of where we are in the buffer. init to zero
    int loop_count           = 0; // counter for number of loops

    ClockManager(TimerHandle* hardware_clock, HardwareManager* hardware_manager){
        this->hardware_clock = hardware_clock;
        this->hardware_manager = hardware_manager;
    }

    void UpdatePeriod(float bpm){
        float clock_freq_hz = ppqn * bpm * (1.0f/60.0f);
        float period = static_cast<uint32_t>(internal_clock_freq_hz/clock_freq_hz);
        this->hardware_clock->SetPeriod(period);
    }

    // Advance the clock index
    void AdvanceClockIndex() {
        if (ticks_from_start_idx == buffer_size_ticks - 1) { // reset metro index at end of buffer
            ticks_from_start_idx = 0;
            loop_count++;
        } else {
            ticks_from_start_idx++;
        }
    }

    void ObservePlayState() {
        if(!play_enabled) {
            hardware_manager->clock_out.Write(false);
        }
    }

    std::pair<int, int> GetCurrentStepIndexAndOffsetTicks() {
        std::pair<int, int> pair_current_step_index_and_offset_ticks;

        // Add this so that there's no errors dividing by zero
        if (ticks_from_start_idx == 0) {
            pair_current_step_index_and_offset_ticks.first = 0;
            pair_current_step_index_and_offset_ticks.second = 0;
            return pair_current_step_index_and_offset_ticks;
        }

        // Calculate the last whole step and the remaining offset ticks from that step
        std::div_t step_ticks = std::div(ticks_from_start_idx, ticks_per_step);
        int last_whole_step = int(step_ticks.quot);
        int offset_ticks = int(step_ticks.rem);
        int next_step = last_whole_step + 1;

        // If the remainder is >= half a step length, it is a negative offset relative to q+1 (next step)
        if (offset_ticks >= ticks_per_half_step) {
            // if next step is greater than the number of steps in our buffer, the next step  is 0
            // in our default case, the quotient max value is 31 so next_step max is 32 (size of step buffer)
            if (next_step >= step_buffer_length) {
                pair_current_step_index_and_offset_ticks.first = 0;
            } else {
                pair_current_step_index_and_offset_ticks.first = next_step;
            }
            // use last_whole_step to calc offset to account for times when we have a neg offset from step 0
            pair_current_step_index_and_offset_ticks.second = -1 * ((next_step * ticks_per_step) - ticks_from_start_idx);
        }

        // If the remainder is < half a step length, it is a positive offset relative to q (step)
        if (offset_ticks < ticks_per_half_step) {
            pair_current_step_index_and_offset_ticks.first = last_whole_step;
            pair_current_step_index_and_offset_ticks.second = ticks_from_start_idx - (last_whole_step * ticks_per_step);
        }
        return pair_current_step_index_and_offset_ticks;
    }
};
