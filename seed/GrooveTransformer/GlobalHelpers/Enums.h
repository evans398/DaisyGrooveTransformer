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
enum AdcChannel {
   MUX_1 = 0,
   MUX_2,
   MUX_3,
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
    GP_POT_1 = 0,
    GP_POT_2,
    GP_POT_3,
    INTERPOLATION_POT,
    GP_CV_1,
    GP_CV_2,
    GP_CV_3,
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

enum DAC_CHANNEL {
    A=0,
    B,
    C,
    D
};

//******************************************************************************************/
//** Model Parameters */
//******************************************************************************************/
enum class ModelParameter {
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
enum class InterpolationButtonName {
    BUTTON_A,
    BUTTON_B
};

