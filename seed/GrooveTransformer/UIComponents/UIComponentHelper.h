#pragma once

int RoundScaledValue(int scaled_value) {
    int rounded_value;
    int remainder = scaled_value % 5;
    if (scaled_value == 99) {
        return 100;
    }
    if (scaled_value == 1) {
        return 0;
    }
    if (remainder == 0) {
        rounded_value = scaled_value;
    }
    if(remainder < 3){
        rounded_value = scaled_value - remainder;
    }
    if(remainder > 2){
        rounded_value = scaled_value + (5 - remainder);
    }
    return rounded_value;
}

int ScaleInterpolationPotValue(float value) {
    float scaled_value = value * 100.;
    int scaled_value_int = (int) scaled_value;
    if (scaled_value > 99) {
        return 100;
    }
    if (scaled_value < 1) {
        return 0;
    }
    return scaled_value_int;
}

int ScalePotValue(float value) {
    float scaled_value = value * 100.;
    int scaled_value_int = (int) scaled_value;
    return RoundScaledValue(scaled_value_int);
}
