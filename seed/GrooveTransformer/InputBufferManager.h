#pragma once

struct InputBufferManager {

    ClockManager* clock_manager;
    UartLibreManager* uart_libre_manager;
    HardwareManager* hardware_manager;

    /** Controls */
    bool rec_enabled;
    bool overdub_enabled;

    std::array<std::unique_ptr<Onset>, step_buffer_length> buffer; // input buffer is an array of unique ptrs to Onset objects

    InputBufferManager(UartLibreManager* uart_libre_manager, ClockManager* clock_manager, HardwareManager* hardware_manager){
        this->clock_manager = clock_manager;
        this->uart_libre_manager = uart_libre_manager;
        this->hardware_manager = hardware_manager;
    };


    Onset AddOnset(int step_index, float input_velocity, int offset_ticks) {
        this->buffer[step_index] = std::make_unique<Onset> (step_index, input_velocity, offset_ticks, this->clock_manager);
        return *this->buffer[step_index]; // after onset is added to input buffer, transmit it to model
    }

    bool CreateAndAddOnsetToInputBuffer(int current_step_index, float input_velocity, int offset_ticks) {
        // Add onset if no onset exists on this step, or if the loop_count is not the same, or if this input velocity
        // is greater than the existing onset at this step
        if (this->GetOnset(current_step_index) == nullptr ||
            this->GetOnset(current_step_index)->onset_loop_count != clock_manager->loop_count ||
            input_velocity > this->GetOnset(current_step_index)->input_velocity
        ) {
            return true;
        } else {
            return false;
        }
    }

    bool OverwriteOnset(int current_step_index, int offset_ticks) {
        // Remove onset if one exists at this step
        if (this->GetOnset(current_step_index) == nullptr || this->GetOnset(current_step_index)->input_velocity == 0 || 
            this->GetOnset(current_step_index)->onset_loop_count == clock_manager->loop_count
        ) {
            return false;
        } else {
            return true;
        }
    }

    void TransmitAllOnsets() { // for testing
        int step_idx = 0;
        for (auto&& onset : buffer) {
            if (onset) {
                this->uart_libre_manager->TransmitOnset(
                    onset->step, 
                    onset->scaled_velocity,
                    onset->step_offset_value
                );
            } else {
                this->uart_libre_manager->TransmitOnset(
                    step_idx, 
                    0.f,
                    0.f
                );
            }
            step_idx++;
        }
    }

    void ObserveSAPMessage() {
        if(uart_libre_manager->SEND_ALL_ONSETS) {
            uart_libre_manager->SEND_ALL_ONSETS = false;
            // this->TransmitAllOnsets();
        }
    }

    Onset * GetOnset(int step_index) {
        auto ptr = this->buffer[step_index].get();
        return ptr;
    }

    void ClearOnset(int step) {
        this->buffer[step].reset();
    }

    void ClearBuffer() {
        for (auto& step : buffer) {
            step = nullptr;
        }
    }

    void ObserveRecordBuffer() {
        if (rec_enabled) {
            std::pair<int, int> current_step_index_and_offset_ticks = this->clock_manager->GetCurrentStepIndexAndOffsetTicks();
            if(OverwriteOnset(current_step_index_and_offset_ticks.first, current_step_index_and_offset_ticks.second)){
                float velocity = 0.;
                Onset onset = AddOnset(current_step_index_and_offset_ticks.first, velocity, current_step_index_and_offset_ticks.second);
                this->uart_libre_manager->TransmitOnset(onset);
            }
        }
    }
};
