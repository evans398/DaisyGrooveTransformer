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

<<<<<<< HEAD
=======

>>>>>>> merge-branch
//******************************************************************************************/
//** MIDI CC */
//******************************************************************************************/
enum MIDI_CC {
    MIDI_CC_CHANNEL = 3,
    A_BUTTON_SAVE = 4,
    B_BUTTON_SAVE = 5,
    A_BUTTON_RANDOMIZE = 6,
    B_BUTTON_RANDOMIZE = 7,
    CROSSFADER_POS = 8,
    CLEAR = 9,
    PLAY = 10, 
    STOP = 11,
    RECORD = 12,
    RECORD_OFF = 13,
    OVERDUB = 14,
    TEMPO = 15,
    UNCERTAINTY = 16,
    GENERAL_PURPOSE_1 = 17,
    GENERAL_PURPOSE_2 = 18,
    GENERAL_PURPOSE_3 = 19,
    GROOVE_VELOCITY = 20,
    GROOVE_OFFSET = 21,
    VOICE_VELOCITY_SCALE_1 = 22,
    VOICE_VELOCITY_SCALE_2 = 23,
    VOICE_VELOCITY_SCALE_3 = 24,
    VOICE_VELOCITY_SCALE_4 = 25,
    VOICE_VELOCITY_SCALE_5 = 26,
    VOICE_VELOCITY_SCALE_6 = 27,
    VOICE_DENSITY_SCALE_1 = 28,
    VOICE_DENSITY_SCALE_2 = 29,
    VOICE_DENSITY_SCALE_3 = 30,
    VOICE_DENSITY_SCALE_4 = 31,
    VOICE_DENSITY_SCALE_5 = 32,
    VOICE_DENSITY_SCALE_6 = 33,
    SAVE_1 = 34,
    SAVE_2 = 35,
    SAVE_3 = 36,
    SAVE_4 = 37,
    SAVE_5 = 38,
    SAVE_6 = 39,
    LOAD_1 = 40,
    LOAD_2 = 41,
    LOAD_3 = 42,
    LOAD_4 = 43,
    LOAD_5 = 44,
    LOAD_6 = 45,
<<<<<<< HEAD
};

=======
};
>>>>>>> merge-branch
