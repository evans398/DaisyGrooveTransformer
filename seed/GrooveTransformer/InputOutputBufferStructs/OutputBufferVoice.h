#pragma once


struct OutputBufferVoice {
    float buffer[output_buffer_size] = {0};

    float velocity_scale_amount;
    
    OutputBufferVoice(){};

    auto& GetBuffer() {
        return this->buffer;
    }

    void ResetBuffer() {
        for (auto& tick : this->buffer) {
            tick = 0.f;
        }
    }

    void AddModelOutputStepToBuffer(ModelOutputStep model_output_step) {
        float velocity = model_output_step.velocity;
        float offset_value = model_output_step.offset_value;
        int step_index = model_output_step.step_index;
        int ticks_from_start = ConvertStepOffsetValueToTicksFromStart(step_index, offset_value);
        this->buffer[ticks_from_start] = velocity;
    }

    int ConvertStepOffsetValueToTicksFromStart(int step, float offset_value) {
        float offset_value_ticks = ticks_per_step * offset_value;
        float ticks_from_start;
        if (step == 0 && offset_value_ticks < 0) {
            ticks_from_start = buffer_size_ticks - offset_value_ticks;
        } else {
            ticks_from_start = (step * ticks_per_step) + offset_value_ticks;
        }
        return int(ticks_from_start);
    }

    void ClearStep(int step_index) {
        int ticks_index_at_step_no_offset = step_index * ticks_per_step;
        int tick_index_boundary_at_step_positive_offset = ticks_index_at_step_no_offset + ticks_per_half_step - 1;
        if (step_index == 0) {
            int tick_index_boundary_at_step_negative_offset = output_buffer_size - ticks_per_half_step;
            for (int i = tick_index_boundary_at_step_negative_offset; i < output_buffer_size; i++) {
                buffer[i] = 0.f;
            }
        } else {
            int tick_index_boundary_at_step_negative_offset = ticks_index_at_step_no_offset - ticks_per_half_step;
            for (int i = tick_index_boundary_at_step_negative_offset; i <= tick_index_boundary_at_step_positive_offset; i++) {
                buffer[i] = 0.f;
            }
        }
    }
};
