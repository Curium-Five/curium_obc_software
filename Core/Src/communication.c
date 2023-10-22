//
// Created by Marcus Ding on 17.10.23.
//

#include "stdint.h"
#include "stdio.h"
#include "communication.h"

/**
 * @brief Destuffs HLDC frames
 *
 * Takes an stuffed (encoded) HLDC frame and destuffs (decodes) it by removing
 * escape flag sequences. Then writes the result to destuffed_data and the length
 * of the destuffed frame to destuffed_length.
 *
 *
 * @param frame the frame of the encoded HDLC frame without start/end flags
 * @param destuffed_data the buffer into which the destuffed frame will be written
 * @param destuffed_length variable into which the length of the destuffed frame will be saved
 * @return void
 */
void destuff_hdlc_frame(HDLC_Frame_Struct *frame, uint8_t *destuffed_data, uint16_t *destuffed_length){

    *destuffed_length = 0;

    // Flag to keep track of escape characters
    int escape_flag = 0;

    for(int i = 0; i < frame->length; i++) {
        if(frame->data[i] == HDLC_CONTROL_ESCAPE) {
            escape_flag = 1;
        } else {
            if(escape_flag) {
                // If the previous character was an escape character,
                // flip the 5th bit of the current character
                destuffed_data[*destuffed_length] = frame->data[i] ^ HDLC_ESCAPE_BIT_FLIP;
                escape_flag = 0;
            } else {
                // No escape character, so just copy the data
                destuffed_data[*destuffed_length] = frame->data[i];
            }
            (*destuffed_length)++;
        }
    }

    // Now destuffed_data contains the destuffed data, and destuffed_length is its length.
    // You can process this data as needed.

    // Debug print the destuffed data:
    char msg[128]; // 3 characters per byte (2 hex digits and a space)
    for(int i = 0; i < *destuffed_length; i++) {
        sprintf(msg + (i * 3), "%02X ", destuffed_data[i]);
    }
    /*
    print_debug_msg("Destuffed data: ");
    print_debug_msg(msg);
    print_debug_msg("\n");
    */
}

/**
 * @brief Stuffs HLDC frames
 *
 * Takes an byte frame and stuffs (encodes) it by adding
 * escape flag sequences. Then writes the result to stuffed_data and the length
 * of the stuffed frame to stuffed_length.
 *
 *
 * @param data the bytestream to be encoded
 * @param stuffed_data the buffer into which the stuffed frame will be written
 * @param stuffed_length variable into which the length of the stuffed frame will be saved
 * @return void
 */
void bit_stuff(uint8_t *data, uint16_t length, uint8_t *stuffed_data, uint16_t *stuffed_length) {
    *stuffed_length = 0;

    for(int i = 0; i < length; i++) {
        uint8_t byte = data[i];

        // If the byte is equal to the control escape or flag sequence,
        // add an escape byte and then the byte XOR'd with the escape bit flip
        if (byte == HDLC_CONTROL_ESCAPE || byte == HDLC_FLAG_SEQUENCE) {
            stuffed_data[(*stuffed_length)++] = HDLC_CONTROL_ESCAPE;
            stuffed_data[(*stuffed_length)++] = byte ^ HDLC_ESCAPE_BIT_FLIP;
        } else {
            // Otherwise, just copy the byte
            stuffed_data[(*stuffed_length)++] = byte;
        }
    }
}

/**
 * @brief Takes byte stream and transforms them into HLDC frames
 *
 * Takes an byte stream and stuffs (encodes) it by adding
 * escape flag sequences, also adds end and start flags.
 *
 *
 * @param data the bytestream to be encoded
 * @param length the length of the data
 * @param frame variable into which the encoded HDLC frame will be saved
 * @return void
 */
void encode_hldc_frame(uint8_t *data, uint16_t length, HDLC_Frame_Struct *frame) {
    uint16_t encoded_length = 0;
    uint16_t stuffed_length;
    uint8_t stuffed_data[HLDC_BUFFER_SIZE];
    bit_stuff(data, length, stuffed_data, &stuffed_length);

    frame->data[encoded_length++] = HDLC_FLAG_SEQUENCE;

    for(int i = 0; i < stuffed_length; i++) {
        uint8_t byte = stuffed_data[i];
        frame->data[encoded_length++] = byte;
    }

    frame->data[encoded_length++] = HDLC_FLAG_SEQUENCE;

    frame->length = encoded_length;
}
