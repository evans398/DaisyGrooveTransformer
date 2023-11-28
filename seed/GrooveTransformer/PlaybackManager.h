#pragma once

using namespace daisy;

enum DAC_CHANNEL {
    A=0,
    B,
    C,
    D
};

int MAX_DAC_VALUE = 4095;
float MAX_VELOCITY_VALUE = 127.f;

unsigned char DAC_1_ADDRESS {0x64};
unsigned char DAC_2_ADDRESS {0x60};
unsigned char DAC_3_ADDRESS {0x61};

#define DAC_BUFF_SIZE 8
static uint8_t DMA_BUFFER_MEM_SECTION dac_buffer[DAC_BUFF_SIZE];

struct DacOutput {
    int voice_index;
    unsigned char address;
    enum DAC_CHANNEL dac_channel;
    HardwareManager* hardware_manager;
    int reset_counter = 0;

    DacOutput(int voice_index, unsigned char address, DAC_CHANNEL dac_channel, HardwareManager* hardware_manager) {
        this->voice_index=voice_index;
        this->address = address;
        this->dac_channel = dac_channel;
        this->hardware_manager = hardware_manager;
        WriteToDac(0);
    }

    void WriteTriggerToDac(){
        WriteToDac(MAX_DAC_VALUE);
        reset_counter = 0;
    }

    void WriteZeroToDac(){
        WriteToDac(0);
    }

    void WriteVelocityToDac(float velocity_value){
        int dac_value = MAX_DAC_VALUE * (velocity_value/MAX_VELOCITY_VALUE);
        WriteToDac(dac_value);
    }

    void WriteToDac(uint16_t value) {
        switch(this->dac_channel) {
            case 0:
            {
                uint16_t channel_a_value {value};
                dac_buffer[0] = static_cast<uint8_t>(channel_a_value >> 8);
                dac_buffer[1] = static_cast<uint8_t>(channel_a_value & 0xFF);
                break;
            }
            case 1:
            {
                uint16_t channel_b_value {value};
                dac_buffer[2] = static_cast<uint8_t>(channel_b_value >> 8);
                dac_buffer[3] = static_cast<uint8_t>(channel_b_value & 0xFF);
                break;
            }
            case 2:
            {
                uint16_t channel_c_value {value};
                dac_buffer[4] = static_cast<uint8_t>(channel_c_value >> 8);
                dac_buffer[5] = static_cast<uint8_t>(channel_c_value & 0xFF);
                break;
            }
            case 3:
            {
                uint16_t channel_d_value {value};
                dac_buffer[6] = static_cast<uint8_t>(channel_d_value >> 8);
                dac_buffer[7] = static_cast<uint8_t>(channel_d_value & 0xFF);
                break;
            }
            default:
                break;
        }
        I2CHandle::Result i2cResult= hardware_manager->i2c->TransmitDma(address, &dac_buffer[0], DAC_BUFF_SIZE, NULL, NULL);
    }
};

struct PlaybackManager {
    UartMidiManager* uart_midi_manager;
    ClockManager* clock_manager;
    OutputBufferManager* output_buffer_manager;
    HardwareManager* hardware_manager;

    std::array<std::unique_ptr<DacOutput>, NUM_DAC_OUTPUT_VOICES> dac_output_triggers;
    std::array<std::unique_ptr<DacOutput>, NUM_DAC_OUTPUT_VOICES> dac_output_cv;

    PlaybackManager(
        UartMidiManager* uart_midi_manager, 
        ClockManager* clock_manager,
        OutputBufferManager* output_buffer_manager,
        HardwareManager* hardware_manager
    ) {
        this->uart_midi_manager = uart_midi_manager;
        this->clock_manager = clock_manager;
        this->output_buffer_manager = output_buffer_manager;
        this->hardware_manager = hardware_manager;

        this->dac_output_triggers[0] = std::make_unique<DacOutput>(0, DAC_1_ADDRESS, DAC_CHANNEL::A, hardware_manager);
        this->dac_output_triggers[1] = std::make_unique<DacOutput>(1, DAC_1_ADDRESS, DAC_CHANNEL::C, hardware_manager);
        this->dac_output_triggers[2] = std::make_unique<DacOutput>(2, DAC_2_ADDRESS, DAC_CHANNEL::A, hardware_manager);
        this->dac_output_triggers[3] = std::make_unique<DacOutput>(3, DAC_2_ADDRESS, DAC_CHANNEL::C, hardware_manager);
        // this->dac_output_triggers[4] = std::make_unique<DacOutput>(4, DAC_3_ADDRESS, DAC_CHANNEL::A, hardware_manager);
        // this->dac_output_triggers[5] = std::make_unique<DacOutput>(5, DAC_3_ADDRESS, DAC_CHANNEL::C, hardware_manager);

        this->dac_output_cv[0] = std::make_unique<DacOutput>(0, DAC_1_ADDRESS, DAC_CHANNEL::B, hardware_manager);
        this->dac_output_cv[1] = std::make_unique<DacOutput>(1, DAC_1_ADDRESS, DAC_CHANNEL::D, hardware_manager);
        this->dac_output_cv[2] = std::make_unique<DacOutput>(2, DAC_2_ADDRESS, DAC_CHANNEL::B, hardware_manager);
        this->dac_output_cv[3] = std::make_unique<DacOutput>(3, DAC_2_ADDRESS, DAC_CHANNEL::D, hardware_manager);
        // this->dac_output_cv[4] = std::make_unique<DacOutput>(4, DAC_3_ADDRESS, DAC_CHANNEL::B, hardware_manager);
        // this->dac_output_cv[5] = std::make_unique<DacOutput>(5, DAC_3_ADDRESS, DAC_CHANNEL::D, hardware_manager);
    };

    void WriteToDac1(int val) {
        uint16_t channel_a_value {val};
        uint16_t channel_b_value {val};
        uint16_t channel_c_value {val};
        uint16_t channel_d_value {val};

        dac_buffer[0] = static_cast<uint8_t>(channel_a_value >> 8);
        dac_buffer[1] = static_cast<uint8_t>(channel_a_value & 0xFF);
        dac_buffer[2] = static_cast<uint8_t>(channel_b_value >> 8);
        dac_buffer[3] = static_cast<uint8_t>(channel_b_value & 0xFF);
        dac_buffer[4] = static_cast<uint8_t>(channel_c_value >> 8);
        dac_buffer[5] = static_cast<uint8_t>(channel_c_value & 0xFF);
        dac_buffer[6] = static_cast<uint8_t>(channel_d_value >> 8);
        dac_buffer[7] = static_cast<uint8_t>(channel_d_value & 0xFF);
        hardware_manager->i2c->TransmitDma(0x64, &dac_buffer[0], 8, NULL, NULL);

        // uint16_t channel_0_value {0};


        // dac_buffer[0] = static_cast<uint8_t>(channel_0_value >> 8);
        // dac_buffer[1] = static_cast<uint8_t>(channel_0_value & 0xFF);
        // dac_buffer[2] = static_cast<uint8_t>(channel_0_value >> 8);
        // dac_buffer[3] = static_cast<uint8_t>(channel_0_value & 0xFF);
        // dac_buffer[4] = static_cast<uint8_t>(channel_0_value >> 8);
        // dac_buffer[5] = static_cast<uint8_t>(channel_0_value & 0xFF);
        // dac_buffer[6] = static_cast<uint8_t>(channel_0_value >> 8);
        // dac_buffer[7] = static_cast<uint8_t>(channel_0_value & 0xFF);
        // hardware_manager->i2c->TransmitDma(0x64, &dac_buffer[0], 8, NULL, NULL);

    }
    
    void  TriggerOutputs() {
        for (int voice_idx=0; voice_idx<NUM_OUTPUT_VOICES; voice_idx++) {
            switch(output_buffer_manager->active_output_buffer) { // only trigger from the active buffer
                case OutputBuffer::OUTPUT_BUFFER_1:
                // TODO: COLLECT ALL DAC VALUES AND SEND ONE MESSAGE PER DAC
                {
                    float velocity = output_buffer_manager->output_buffer_one[voice_idx]->buffer[this->clock_manager->ticks_from_start_idx];
                    float velocity_scale_amount = 1.0;
                    // float velocity_scale_amount = output_buffer_manager->output_buffer_one[voice_idx]->velocity_scale_amount;
                    float scaled_velocity = velocity * velocity_scale_amount;
                    if (scaled_velocity > 0) {
                        int scaled_velocity_int = (int) scaled_velocity;
                        this->uart_midi_manager->SendMidiOutputs(voice_idx, scaled_velocity_int);
                        if (voice_idx < NUM_DAC_OUTPUT_VOICES) {
                            // dac_output_triggers[voice_idx]->WriteTriggerToDac();
                            // dac_output_cv[voice_idx]->WriteVelocityToDac(scaled_velocity);
                        }
                    }
                }
                case OutputBuffer::OUTPUT_BUFFER_2:
                // TODO: COLLECT ALL DAC VALUES AND SEND ONE MESSAGE PER DAC
                {
                    float velocity = output_buffer_manager->output_buffer_two[voice_idx]->buffer[this->clock_manager->ticks_from_start_idx];
                    float velocity_scale_amount = 1.0;
                    // float velocity_scale_amount = output_buffer_manager->output_buffer_two[voice_idx]->velocity_scale_amount;
                    float scaled_velocity = velocity * velocity_scale_amount;
                    if (scaled_velocity > 0) {
                        int scaled_velocity_int = (int) scaled_velocity;
                        this->uart_midi_manager->SendMidiOutputs(voice_idx, scaled_velocity_int);
                        if (voice_idx < NUM_DAC_OUTPUT_VOICES) {
                            // dac_output_triggers[voice_idx]->WriteTriggerToDac();
                            // dac_output_cv[voice_idx]->WriteVelocityToDac(scaled_velocity);
                        }
                    }
                }
                default:
                break;
            }
        }
    }
};

