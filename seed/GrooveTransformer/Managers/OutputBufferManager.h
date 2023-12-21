#include "../InputOutputBufferStructs/ModelOutputStep.h"
#include "../InputOutputBufferStructs/OutputBufferVoice.h"
#pragma once

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

    // void PrintOutputBuffer(int buff_idx) { // for testing
    //     OutputBufferVoice op1 = *output_buffer_one[buff_idx];
    //     OutputBufferVoice op2 = *output_buffer_two[buff_idx];

    //     int idx = 0;
    //     hardware_manager->hw->PrintLine("PRINTING OUTPUT BUFFER 1");
    //     for (float velocity_value : op1.GetBuffer()) {
    //         int velocity_value_int = int(velocity_value);
    //         hardware_manager->hw->PrintLine("Tick %d: %d", idx, velocity_value_int);
    //         idx++;
    //     }
    //     hardware_manager->hw->PrintLine("PRINTING OUTPUT BUFFER 1 END");
    //     int idx2 = 0;
    //     hardware_manager->hw->PrintLine("PRINTING OUTPUT BUFFER 2");
    //     for (float velocity_value2 : op2.GetBuffer()) {
    //         int velocity_value_int2 = int(velocity_value2);
    //         hardware_manager->hw->PrintLine("Tick %d: %d", idx2, velocity_value_int2);
    //         idx2++;
    //     }
    //     hardware_manager->hw->PrintLine("PRINTING OUTPUT BUFFER 2 END");
    // }

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
