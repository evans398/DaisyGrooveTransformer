#pragma once

using namespace daisy;

// bool LIBRE_IS_READY = false;

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

/** UART Communication */
#define LIBRE_BUFF_SIZE 4096
static uint8_t DMA_BUFFER_MEM_SECTION tx_buff[LIBRE_BUFF_SIZE]; // buffer to be dma transmitted

struct UartLibreManager {
    //** Handlers */
    UartHandler* uart_libre;
    HardwareManager* hardware_manager;
    bool SEND_ALL_UI_PARAMS = true;
    bool SEND_ALL_ONSETS = true;

    OutputBufferManager* output_buffer_manager;

    //** UART Communication FIFO Rx Strings */
    std::string fifo_char_string;
    std::string first_char;

    UartLibreManager(UartHandler* uart_libre, OutputBufferManager* output_buffer_manager, HardwareManager* hardware_manager){
        this->uart_libre = uart_libre;
        this->output_buffer_manager = output_buffer_manager;
        this->hardware_manager = hardware_manager;
        // InitUartLibre();
    };

    void InitUartLibre() {
        // Configure the Uart Peripheral for Pi Communication
        UartHandler::Config uart_conf;
        uart_conf.periph        = UartHandler::Config::Peripheral::USART_2;
        uart_conf.mode          = UartHandler::Config::Mode::TX_RX;
        uart_conf.pin_config.tx = Pin(PORTA, 2);
        uart_conf.pin_config.rx = Pin(PORTA, 3);

        // Initialize the uart_libre peripheral and start the DMA transmit
        this->uart_libre->Init(uart_conf);

        /** Start the FIFO Receive */
        this->uart_libre->DmaReceiveFifo();
    }

    void HandleLibreUart(){
        // ** UART Receiver */
        // if there's data, pop it from the FIFO
        if(this->uart_libre->ReadableFifo()){
            this->HandleLibreFifoMessage();
        } 
    }

    void HandleLibreFifoMessage() {
        // receives messages one char at a time

        uint8_t top_fifo_item = this->uart_libre->PopFifo(); // get top fifo item

        if (top_fifo_item != 0x0a) { // if top is not \n char, add it to the char string
            fifo_char_string += char(top_fifo_item);
        } else {
            // hardware_manager->hw->PrintLine("Received: %s", fifo_char_string.c_str());
            if (fifo_char_string.size() > 20) {
                fifo_char_string.clear();
            }
            first_char = fifo_char_string[0];
            if (fifo_char_string.compare("NG/") == 0) { // if NG/, clear the previous buffer and prepare to populate it with new generation
                switch(output_buffer_manager->inactive_output_buffer) {
                    case OutputBuffer::OUTPUT_BUFFER_1:
                        this->output_buffer_manager->ClearAllVoices(OutputBuffer::OUTPUT_BUFFER_1);
                        fifo_char_string.clear();
                        break;
                    case OutputBuffer::OUTPUT_BUFFER_2:
                        this->output_buffer_manager->ClearAllVoices(OutputBuffer::OUTPUT_BUFFER_2);
                        fifo_char_string.clear();
                        break;
                }
            }
            if (first_char.compare("H") == 0) { // if the first char in the message is H, there is hit message data that follows
                this->ParseHitStringAndPopulateInactiveBuffer(fifo_char_string); // parse the hit message and populate the next output buffer

            }
            if (first_char.compare("P") == 0) { // if the first char in the message is H, there is hit message data that follows
                // TODO: METHOD TO PARSE PROBABILITY

            }
            if (fifo_char_string.compare("GD/") == 0) { // if generation done, change the active buffer to the newly populated buffer
                // hw.PrintLine("INSIDE GD");  
                switch(output_buffer_manager->inactive_output_buffer) {
                    case OutputBuffer::OUTPUT_BUFFER_1:
                        output_buffer_manager->active_output_buffer = OutputBuffer::OUTPUT_BUFFER_1;
                        output_buffer_manager->inactive_output_buffer = OutputBuffer::OUTPUT_BUFFER_2;
                        fifo_char_string.clear();
                        break;
                    case OutputBuffer::OUTPUT_BUFFER_2:
                        output_buffer_manager->active_output_buffer = OutputBuffer::OUTPUT_BUFFER_2;
                        output_buffer_manager->inactive_output_buffer = OutputBuffer::OUTPUT_BUFFER_1;
                        fifo_char_string.clear();
                        break;
                }
            }
            if (fifo_char_string.compare("SAP/") == 0) { // if generation done, change the active buffer to the newly populated buffer
                SEND_ALL_UI_PARAMS = true;
                SEND_ALL_ONSETS = true;
            }
            fifo_char_string.clear();
            
        }
    }

    // **** TEST METHOD **** ////
    void TestHandleLibreFifoMessage(std::string fifo_char_string) {
        first_char = fifo_char_string[0];
        if (fifo_char_string.compare("NG/") == 0) { // if NG/, clear the previous buffer and prepare to populate it with new generation
            switch(output_buffer_manager->inactive_output_buffer) {
                case OutputBuffer::OUTPUT_BUFFER_1:
                    this->output_buffer_manager->ClearAllVoices(OutputBuffer::OUTPUT_BUFFER_1);
                    fifo_char_string.clear();
                    break;
                case OutputBuffer::OUTPUT_BUFFER_2:
                    this->output_buffer_manager->ClearAllVoices(OutputBuffer::OUTPUT_BUFFER_2);
                    fifo_char_string.clear();
                    break;
            }
        }
        if (first_char.compare("H") == 0) { // if the first char in the message is H, there is hit message data that follows
            this->ParseHitStringAndPopulateInactiveBuffer(fifo_char_string); // parse the hit message and populate the next output buffer

        }
        if (first_char.compare("P") == 0) { // if the first char in the message is H, there is hit message data that follows
            // TODO: METHOD TO PARSE PROBABILITY

        }
        if (fifo_char_string.compare("GD/") == 0) { // if generation done, change the active buffer to the newly populated buffer
            switch(output_buffer_manager->inactive_output_buffer) {
                case OutputBuffer::OUTPUT_BUFFER_1:
                    output_buffer_manager->active_output_buffer = OutputBuffer::OUTPUT_BUFFER_1;
                    output_buffer_manager->inactive_output_buffer = OutputBuffer::OUTPUT_BUFFER_2;
                    fifo_char_string.clear();
                    break;
                case OutputBuffer::OUTPUT_BUFFER_2:
                    output_buffer_manager->active_output_buffer = OutputBuffer::OUTPUT_BUFFER_2;
                    output_buffer_manager->inactive_output_buffer = OutputBuffer::OUTPUT_BUFFER_1;
                    fifo_char_string.clear();
                    break;
            }
        }
        if (fifo_char_string.compare("SAP/") == 0) { // if generation done, change the active buffer to the newly populated buffer
            SEND_ALL_UI_PARAMS = true;
            SEND_ALL_ONSETS = true;
        }
        fifo_char_string.clear();
    }

    bool HitStringHasCorrectNumber(std::string hit_string) {
        int del_count = 0;
        char del = '/'; // delimiter around which string is to be split

        for(int i=0; i<(int)hit_string.size(); i++){
            if(hit_string[i] == del) {
                del_count++;
            }
        }
        return del_count == 4;
    }

    bool IsNumber(const std::string& str) {
        // Empty string is not a float
        if (str.empty())
            return false;

        // Check for leading sign (+/-)
        size_t i = 0;
        if (str[i] == '+' || str[i] == '-')
            ++i;

        bool hasDecimal = false;
        bool hasDigits = false;

        // Check for digits or decimal point
        for (; i < str.length(); ++i) {
            if (std::isdigit(str[i])) {
                hasDigits = true;
            } else if (str[i] == '.') {
                // Only one decimal point allowed
                if (hasDecimal)
                    return false;

                hasDecimal = true;
            } else {
                // Invalid character encountered
                return false;
            }
        }

        // At least one digit is required
        if (!hasDigits)
            return false;

        return true;
    }

    void ParseHitStringAndPopulateInactiveBuffer(std::string hit_string) {
        // hit string format is hit/{voice}/{vel}/{offset}/{step}
        // H/0/15/68/-0.03

        char del = '/'; // delimiter around which string is to be split
        std::string temp = ""; // declaring temp string to append characters up to delimitier
        std::vector<std::string> hit_vector; // vector to store complete words/values
        int vector_size = (int)hit_string.size();

        if (!HitStringHasCorrectNumber(hit_string)) {
            return;
        }

        for(int i=0; i<(int)hit_string.size(); i++){
            // If the char is not delimiter, then append it to the temp string, otherwise
            // add the full temp string to the vector and reset it
            if(hit_string[i] != del) {
                temp += hit_string[i];
                if (i == (vector_size - 1)) {
                    hit_vector.push_back(temp);
                    temp = "";
                }
            }
            else {
                hit_vector.push_back(temp);
                temp = "";
            }
        }

        if (!IsNumber(hit_vector[1]) ||
            !IsNumber(hit_vector[2]) ||
            !IsNumber(hit_vector[3]) ||
            !IsNumber(hit_vector[4])
        ) {
            return;
        }

        // convert strings to floats and ints and create ModelOutputStep object
        int voice = std::stoi(hit_vector[1]);
        int step = std::stoi(hit_vector[2]);
        float velocity = std::stof(hit_vector[3]);
        float offset = std::stof(hit_vector[4]);

        // ******* FOR TESTING **************
        // hardware_manager->hw->PrintLine("Received Voice: %d", voice);
        // hardware_manager->hw->PrintLine("Received Step: %d", step);
        // hardware_manager->hw->PrintLine("Received Velocity: " FLT_FMT3, FLT_VAR3(velocity));
        // hardware_manager->hw->PrintLine("Received Offset value: " FLT_FMT3, FLT_VAR3(offset));
        // ******* FOR TESTING **************

        ModelOutputStep model_output_step = {velocity, offset, step, voice};

        switch(output_buffer_manager->inactive_output_buffer) {
            // populate the buffer that is not currently active
            case OutputBuffer::OUTPUT_BUFFER_1:
                this->output_buffer_manager->AddModelOutputStepToOutputBuffer(model_output_step, OutputBuffer::OUTPUT_BUFFER_1);
                break;
            case OutputBuffer::OUTPUT_BUFFER_2: 
                this->output_buffer_manager->AddModelOutputStepToOutputBuffer(model_output_step, OutputBuffer::OUTPUT_BUFFER_2);
                break;
        }
    }

    void TransmitOnset(int step, float scaled_velocity, float step_offset_float_value) {
        /** Transmission Message: "gr/{vel}/{off_scaled}/{step}" */

        /** Get and convert onset values to int before string conversion and transmit */
        int velocity_int = (int)scaled_velocity; // convert float velocity to int
        float scaled_offset = step_offset_float_value * 1000.0; // scale up offset value
        int scaled_offset_int = (int)scaled_offset; // convert scaled offset value to int

        /** Convert int values to string */
        std::string step_str = std::to_string(step);
        std::string input_velocity = std::to_string(velocity_int);
        std::string step_offset_value = std::to_string(scaled_offset_int);

        /** Construct groove message to transmit */
        std::string transmit_message("G/" + step_str + '/' + input_velocity + '/' + step_offset_value + "\n");

        /** Add each char to tx buffer */
        int transmit_message_length = (int) transmit_message.length();
        for (int i = 0; i < transmit_message_length; i++) {
            tx_buff[i] = transmit_message[i];
        }
        this->uart_libre->BlockingTransmit(&tx_buff[0], transmit_message.length()); // trasmit tx buffer
        
    }

    void TransmitOnset(Onset onset) {
        /** Transmission Message: "gr/{vel}/{off_scaled}/{step}" */

        /** Get and convert onset values to int before string conversion and transmit */
        int step = onset.step;
        int velocity_int = (int)onset.scaled_velocity; // convert float velocity to int
        float scaled_offset = onset.step_offset_value * 1000.0; // scale up offset value
        int scaled_offset_int = (int)scaled_offset; // convert scaled offset value to int


        /** Convert int values to string */
        std::string step_str = std::to_string(step);
        std::string input_velocity = std::to_string(velocity_int);
        std::string step_offset_value = std::to_string(scaled_offset_int);

        /** Construct groove message to transmit */
        std::string transmit_message("G/" + step_str + '/' + input_velocity + '/' + step_offset_value + "\n");

        /** Add each char to tx buffer */
        int transmit_message_length = (int) transmit_message.length();
        for (int i = 0; i < transmit_message_length; i++) {
            tx_buff[i] = transmit_message[i];
        }
        this->uart_libre->BlockingTransmit(&tx_buff[0], transmit_message.length()); // trasmit tx buffer
        
    }

    void TransmitUIIndexedParameterValue(enum ModelParameter model_parameter, int index_int, int value_int) {
        /** Convert int values to string */
        std::string value = std::to_string(value_int);
        std::string index = std::to_string(index_int);
        std::string transmit_message;

        /** Construct message to transmit */
        switch(model_parameter) {
            case ModelParameter::VOICE_DENSITY:
            {
                transmit_message = ("DP/" + index + '/' + value + "\n");
                break; 
            }
            case ModelParameter::GENERAL_PURPOSE:
            {
                transmit_message = ("GP/" + index + '/' + value + "\n");
                break;
            }    
            default:
                return;
        }
        /** Add each char to tx buffer */
        int transmit_message_length = (int) transmit_message.length();
        for (int i = 0; i < transmit_message_length; i++) {
            tx_buff[i] = transmit_message[i];
        }
        this->uart_libre->BlockingTransmit(&tx_buff[0], transmit_message.length()); // trasmit tx buffer
    }

    void TransmitUIParameterValue(enum ModelParameter model_parameter, int value_int) {
        /** Convert int values to string */
        std::string value = std::to_string(value_int);
        std::string transmit_message;

        switch(model_parameter) {
            case ModelParameter::INPUT_GROOVE_VELOCITY:
            {
                transmit_message = ("GV/" + value + "\n");
                break;
            }
            case ModelParameter::INPUT_GROOVE_OFFSET:
            {
                transmit_message = ("GO/" + value + "\n");
                break;
            }
            case ModelParameter::UNCERTAINTY:
            {
                transmit_message = ("RP/" + value + "\n");
                break;
            }
            case ModelParameter::INTERPOLATION_POSITION:
            {
                transmit_message = ("IP/" + value + "\n");
                break;
            }
            case ModelParameter::LOAD_PRESET:
            {
                transmit_message = ("LP/" + value + "\n");
                break;
            }
            case ModelParameter::SAVE_PRESET:
            {
                transmit_message = ("SP/" + value + "\n");
                break;
            }
            default:
                return;
        }
        /** Add each char to tx buffer */
        int transmit_message_length = (int) transmit_message.length();
        for (int i = 0; i < transmit_message_length; i++) {
            tx_buff[i] = transmit_message[i];
        }
        this->uart_libre->BlockingTransmit(&tx_buff[0], transmit_message.length()); // trasmit tx buffer
    }

    void TransmitUIParameterState(enum ModelParameter model_parameter) {
        std::string transmit_message;

        switch(model_parameter) {
            case ModelParameter::INTERPOLATION_SAVE_A:
            {
                transmit_message = ("SA/\n");
                break;
            }
            case ModelParameter::INTERPOLATION_SAVE_B:
            {
                transmit_message = ("SB/\n");
                break;
            }
            case ModelParameter::INTERPOLATION_RANDOM_A:
            {
                transmit_message = ("RA/\n");
                break;
            }
            case ModelParameter::INTERPOLATION_RANDOM_B:
            {
                transmit_message = ("RB/\n");
                break;
            }
            case ModelParameter::FREE_RECORDING:
            {
                transmit_message = ("FR/\n");
                break;
            }
            case ModelParameter::OVERDUB_RECORDING:
            {
                transmit_message = ("OR/\n");
                break;
            }
            case ModelParameter::RESET_GROOVE:
            {
                transmit_message = ("RG/\n");
                break;
            }
            default:
                return;
        }
        /** Add each char to tx buffer */
        int transmit_message_length = (int) transmit_message.length();
        for (int i = 0; i < transmit_message_length; i++) {
            tx_buff[i] = transmit_message[i];
        }
        this->uart_libre->BlockingTransmit(&tx_buff[0], transmit_message.length()); // trasmit tx buffer
    }
};



