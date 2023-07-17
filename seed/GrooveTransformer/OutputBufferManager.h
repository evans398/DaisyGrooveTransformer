#pragma once

/** Playback output_buffers */
const int output_buffer_size = ppqn * loop_length_qn; // output buffers have an idx for each tick in a 2 bar loop at defined ppqn
const int NUM_OUTPUT_VOICES = 6; // model generates HVO sequence for 6 voices
const int NUM_DAC_OUTPUT_VOICES = 6; // model generates HVO sequence for 6 voices

//** Enum class to swap which output buffer to read from */
enum class OutputBuffer { 
    OUTPUT_BUFFER_1, 
    OUTPUT_BUFFER_2
};

//** Struct for HVO steps received from the model /
struct ModelOutputStep {
    float velocity;
    float offset_value;
    int step_index;
    int voice_index;

    ModelOutputStep(float velocity, float offset_value, int step_index, int voice_index) {
        this->velocity = velocity;
        this->offset_value = offset_value;
        this->step_index = step_index;
        this->voice_index = voice_index;
    }
};

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

//** Struct to manage active and inactive output buffers
struct OutputBufferManager {
    OutputBuffer active_output_buffer = OutputBuffer::OUTPUT_BUFFER_1; // init to output buffer one
    OutputBuffer inactive_output_buffer = OutputBuffer::OUTPUT_BUFFER_2; // init to output buffer one

    // One buffer is actively being read from while the other is being populated with a new generation
    std::array<std::unique_ptr<OutputBufferVoice>, NUM_OUTPUT_VOICES> output_buffer_one{};
    std::array<std::unique_ptr<OutputBufferVoice>, NUM_OUTPUT_VOICES> output_buffer_two{};

    HardwareManager* hardware_manager;

    OutputBufferManager(HardwareManager* hardware_manager){
        this->hardware_manager = hardware_manager;
        for (int voice_idx = 0; voice_idx<NUM_OUTPUT_VOICES; voice_idx++) {
            output_buffer_one[voice_idx] = std::make_unique<OutputBufferVoice> ();
            output_buffer_two[voice_idx] = std::make_unique<OutputBufferVoice> ();
        }
        this->ClearAllVoices(OutputBuffer::OUTPUT_BUFFER_1);
        this->ClearAllVoices(OutputBuffer::OUTPUT_BUFFER_2);
    };

    void PrintOutputBuffer(int buff_idx) { // for testing
        OutputBufferVoice op1 = *output_buffer_one[buff_idx];
        OutputBufferVoice op2 = *output_buffer_two[buff_idx];

        int idx = 0;
        hardware_manager->hw->PrintLine("PRINTING OUTPUT BUFFER 1");
        for (float velocity_value : op1.GetBuffer()) {
            int velocity_value_int = int(velocity_value);
            hardware_manager->hw->PrintLine("Tick %d: %d", idx, velocity_value_int);
            idx++;
        }
        hardware_manager->hw->PrintLine("PRINTING OUTPUT BUFFER 1 END");
        int idx2 = 0;
        hardware_manager->hw->PrintLine("PRINTING OUTPUT BUFFER 2");
        for (float velocity_value2 : op2.GetBuffer()) {
            int velocity_value_int2 = int(velocity_value2);
            hardware_manager->hw->PrintLine("Tick %d: %d", idx2, velocity_value_int2);
            idx2++;
        }
        hardware_manager->hw->PrintLine("PRINTING OUTPUT BUFFER 2 END");
    }

    OutputBufferVoice * GetOutputBufferVoice(int voice_index) {
        auto ptr = this->output_buffer_one[voice_index].get();
        return ptr;
    }

    void AddModelOutputStepToOutputBuffer(ModelOutputStep model_output_step, OutputBuffer output_buffer) {
        switch(output_buffer) {
            case OutputBuffer::OUTPUT_BUFFER_1:
                output_buffer_one[model_output_step.voice_index].get()->AddModelOutputStepToBuffer(model_output_step);
                break;
            case OutputBuffer::OUTPUT_BUFFER_2:
                output_buffer_two[model_output_step.voice_index].get()->AddModelOutputStepToBuffer(model_output_step);
                break;
            
        }
    }

    void UpdateVoiceVelocityScale(int voice_index, float scale_amount) {
        output_buffer_one[voice_index].get()->velocity_scale_amount = scale_amount/100;
    }

    void ClearAllVoices(OutputBuffer output_buffer) {
        switch(output_buffer) {
            case OutputBuffer::OUTPUT_BUFFER_1:
                for (auto& output_buffer : output_buffer_one) {
                    output_buffer->ResetBuffer();
                }
                break;
            case OutputBuffer::OUTPUT_BUFFER_2:
                for (auto& output_buffer : output_buffer_two) {
                    output_buffer->ResetBuffer();
                }
                break;
        }
    }

    void ClearVoice(int voice_index) {
        output_buffer_one[voice_index].get()->ResetBuffer();
    }

    void ClearVoiceAtTimeStep(int step_index, int voice_index) {
        output_buffer_one[voice_index].get()->ClearStep(step_index);
    }
};
