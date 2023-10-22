//
// Created by Marcus Ding on 15.10.23.
//

#ifndef CURIUM_OBC_SOFTWARE_RADIO_H
#define CURIUM_OBC_SOFTWARE_RADIO_H

/**
 * The maximum RX frame size
 */
#define MAX_RX_FRAME_LEN 512

/**
 * The maximum number of RX frames that can wait at the RX queue to be processed
 * by the OSDLP logic
 */
#define MAX_RX_FRAMES    4

struct rx_frame {
    uint32_t len;
    uint8_t pdu[MAX_RX_FRAME_LEN];
};



#endif //CURIUM_OBC_SOFTWARE_RADIO_H
