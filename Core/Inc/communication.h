//
// Created by Marcus Ding on 17.10.23.
//

#ifndef CURIUM_OBC_SOFTWARE_COMMUNICATION_H
#define CURIUM_OBC_SOFTWARE_COMMUNICATION_H

/**
 * HLDC protocol variables
 */
#define HLDC_BUFFER_SIZE 4096
#define HDLC_FLAG_SEQUENCE 0x7E
#define HLDC_ESCAPE_CHARACTER 0x7D
#define HLDC_BIT_STUFFING_XOR 0x20
#define HDLC_MIN_PAKET_LENGTH 2

#define HDLC_FLAG_SEQUENCE 0x7E
#define HDLC_CONTROL_ESCAPE 0x7D
#define HDLC_ESCAPE_BIT_FLIP 0x20

typedef struct HDLC_Frame_Struct {
    uint8_t data[HLDC_BUFFER_SIZE];
    uint16_t length;
} HDLC_Frame_Struct;

void destuff_hdlc_frame(HDLC_Frame_Struct *frame, uint8_t *destuffed_data, uint16_t *destuffed_length);
void bit_stuff(uint8_t *data, uint16_t length, uint8_t *stuffed_data, uint16_t *stuffed_length);
void encode_hldc_frame(uint8_t *data, uint16_t length, HDLC_Frame_Struct *frame);

#endif //CURIUM_OBC_SOFTWARE_COMMUNICATION_H
