#pragma once

//******************************************************************************************/
//** OUTPUT BUFFER - enums to swap which output buffer to read from */
//******************************************************************************************/
enum class OutputBuffer { 
    OUTPUT_BUFFER_1, 
    OUTPUT_BUFFER_2
};

//******************************************************************************************/
//** HARDWARE PINS */
//******************************************************************************************/
enum Adc_Channel {
   MUX_0 = 0,
   MUX_1,
   MUX_2,
   GROOVE_TRIGGER_IN,
   GROOVE_VELOCITY_IN,
   CLOCK_IN,
   PRESET_KNOB,
   NUM_ADC_CHANNELS
};

enum Mux_0_Index {
   VOICE_DENSITY_POT_0 = 0,
   VOICE_DENSITY_POT_1,
   VOICE_DENSITY_POT_2,
   VOICE_DENSITY_POT_3,
   VOICE_DENSITY_POT_4,
   VOICE_DENSITY_POT_5,
   INPUT_GROOVE_VEL_POT,
   INPUT_GROOVE_OFF_POT
};

enum Mux_1_Index {
   VOICE_SCALE_POT_0 = 0,
   VOICE_SCALE_POT_1,
   VOICE_SCALE_POT_2,
   VOICE_SCALE_POT_3,
   VOICE_SCALE_POT_4,
   VOICE_SCALE_POT_5,
   UNCERTAINTY_POT,
   TEMPO_POT
};

enum Mux_2_Index {
    ASSIGNABLE_POT_1 = 0,
    ASSIGNABLE_POT_2,
    ASSIGNABLE_POT_3,
    CROSSFADER,
    ASSIGNABLE_CV_1,
    ASSIGNABLE_CV_2,
    ASSIGNABLE_CV_3,
    UNCERTAINTY_CV
};

enum Switch_Daisy_Pin_Index {
    SAVE_BUTTON = 5,
    SHIFT_BUTTON = 6,
    CLEAR_BUTTON = 7,
    ON_OFF_SWITCH = 8,
    RECORD_SWITCH = 9,
    OVERDUB_SWITCH = 10,
    A_BUTTON = 29,
    B_BUTTON = 3
};

enum DAC_Channel {
    A=0,
    B,
    C,
    D
};

//******************************************************************************************/
//** Model Parameters */
//******************************************************************************************/
enum class Model_Parameter {
    VOICE_DENSITY, 
    INPUT_GROOVE_VELOCITY,
    INPUT_GROOVE_OFFSET,
    UNCERTAINTY,
    INTERPOLATION_SAVE_A,
    INTERPOLATION_SAVE_B,
    INTERPOLATION_RANDOM_A,
    INTERPOLATION_RANDOM_B,
    INTERPOLATION_POSITION,
    LOAD_PRESET,
    SAVE_PRESET,
    GENERAL_PURPOSE,
    FREE_RECORDING,
    OVERDUB_RECORDING,
    RESET_GROOVE
};

//******************************************************************************************/
//** UI Components */
//******************************************************************************************/
enum class Interpolation_Button_Name {
    BUTTON_A,
    BUTTON_B
};

//******************************************************************************************/
//** MIDI CC */
//******************************************************************************************/
enum MIDI_CC {
    MIDI_CC_CHANNEL = 3,
    A_BUTTON_SAVE_CC,
    B_BUTTON_SAVE_CC,
    A_BUTTON_RANDOMIZE_CC,
    B_BUTTON_RANDOMIZE_CC,
    CROSSFADER_POS_CC,
    CLEAR_CC,
    PLAY_CC, 
    STOP_CC,
    RECORD_CC,
    RECORD_OFF_CC,
    OVERDUB_CC,
    TEMPO_CC,
    UNCERTAINTY_CC,
    UNCERTAINTY_CV_CC,
    ASSIGNABLE_POT_1_CC,
    ASSIGNABLE_POT_2_CC,
    ASSIGNABLE_POT_3_CC,
    ASSIGNABLE_CV_1_CC,
    ASSIGNABLE_CV_2_CC,
    ASSIGNABLE_CV_3_CC,
    GROOVE_VELOCITY_CC,
    GROOVE_OFFSET_CC,
    VOICE_VELOCITY_SCALE_1_CC,
    VOICE_VELOCITY_SCALE_2_CC,
    VOICE_VELOCITY_SCALE_3_CC,
    VOICE_VELOCITY_SCALE_4_CC,
    VOICE_VELOCITY_SCALE_5_CC,
    VOICE_VELOCITY_SCALE_6_CC,
    VOICE_DENSITY_SCALE_1_CC,
    VOICE_DENSITY_SCALE_2_CC,
    VOICE_DENSITY_SCALE_3_CC,
    VOICE_DENSITY_SCALE_4_CC,
    VOICE_DENSITY_SCALE_5_CC,
    VOICE_DENSITY_SCALE_6_CC,
    SAVE_1_CC,
    SAVE_2_CC,
    SAVE_3_CC,
    SAVE_4_CC,
    SAVE_5_CC,
    SAVE_6_CC,
    LOAD_1_CC,
    LOAD_2_CC,
    LOAD_3_CC,
    LOAD_4_CC,
    LOAD_5_CC,
    LOAD_6_CC
};
