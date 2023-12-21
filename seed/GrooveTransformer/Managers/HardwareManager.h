#include "../GlobalHelpers/Enums.h"
#pragma once

using namespace daisy;

struct HardwareManager {
    DaisySeed* hw;
    I2CHandle* i2c;

    Switch save_button;
    Switch shift_button;
    Switch clear_button;
    Switch play_stop_switch;
    Switch record_switch;
    Switch overdub_switch;
    Switch interpolation_button_a;
    Switch interpolation_button_b;
    Switch3 rec_off_dub_switch;

    GPIO clock_out;
    GPIO a_led; 
    GPIO b_led;
    GPIO save_led;

    Pin clock_out_pin = seed::D23;
    Pin a_led_pin = seed::D30;
    Pin b_led_pin = seed::D4;
    Pin shift_led_pin = seed::D26;

    HardwareManager(
        DaisySeed* hw,
        I2CHandle* i2c
    ) {
        this->hw = hw;
        this->i2c = i2c;
        InitHardwareComponents();
    }

    void InitHardwareComponents() {
        hw->Init();
        InitI2c();
        InitAdc();
        InitSwitches();
        InitGPIOOutputs();
    }

    void InitI2c() {
        I2CHandle::Config i2c_config;
        i2c_config.periph = I2CHandle::Config::Peripheral::I2C_1;
        i2c_config.speed  = I2CHandle::Config::Speed::I2C_400KHZ;
        i2c_config.mode   = I2CHandle::Config::Mode::I2C_MASTER;
        i2c_config.pin_config.scl  = {DSY_GPIOB, 8};
        i2c_config.pin_config.sda  = {DSY_GPIOB, 9};
        
        // initialise the peripheral
        i2c->Init(i2c_config);
    }

    void InitSwitches() {
        save_button.Init(hw->GetPin(SAVE_BUTTON));
        shift_button.Init(hw->GetPin(SHIFT_BUTTON));
        clear_button.Init(hw->GetPin(CLEAR_BUTTON));
        play_stop_switch.Init(hw->GetPin(ON_OFF_SWITCH));
        record_switch.Init(hw->GetPin(RECORD_SWITCH));
        overdub_switch.Init(hw->GetPin(OVERDUB_SWITCH));
        interpolation_button_a.Init(hw->GetPin(A_BUTTON));
        interpolation_button_b.Init(hw->GetPin(B_BUTTON));
    }

    void InitGPIOOutputs() {
        clock_out.Init(clock_out_pin, GPIO::Mode::OUTPUT);
        a_led.Init(a_led_pin, GPIO::Mode::OUTPUT);
        b_led.Init(b_led_pin, GPIO::Mode::OUTPUT);
        save_led.Init(shift_led_pin, GPIO::Mode::OUTPUT);
        a_led.Write(true);
        b_led.Write(true);
        save_led.Write(true);
    }

    void BlinkLed(GPIO* led, bool long_blink){
        int delay_time;
        if (long_blink) {
            delay_time = 100;
        } else {
            delay_time = 50;
        }
        led->Write(false);
        System::Delay(delay_time);
        led->Write(true);
        System::Delay(delay_time);
        led->Write(false);
        System::Delay(delay_time);
        led->Write(true);
    }

    void InitAdc() {
        /** Configure the ADC */
        AdcChannelConfig adc_cfg[NUM_ADC_CHANNELS];
        adc_cfg[MUX_1].InitMux(seed::A0, 8, seed::D0, seed::D1, seed::D2);
        adc_cfg[MUX_2].InitMux(seed::A2, 8, seed::D0, seed::D1, seed::D2);
        adc_cfg[MUX_3].InitMux(seed::A5, 8, seed::D0, seed::D1, seed::D2);
        adc_cfg[GROOVE_TRIGGER_IN].InitSingle(seed::A9);
        adc_cfg[GROOVE_VELOCITY_IN].InitSingle(seed::A10);
        adc_cfg[CLOCK_IN].InitSingle(seed::A4);
        adc_cfg[PRESET_KNOB].InitSingle(seed::A3);

        /** Initialize the ADC with our configuration */
        hw->adc.Init(adc_cfg, AdcChannel::NUM_ADC_CHANNELS);
        /** Start the ADC conversions in the background */
        hw->adc.Start();
    }
};
