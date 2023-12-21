#pragma once

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