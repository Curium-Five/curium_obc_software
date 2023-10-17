//
// Created by Marcus Ding on 15.10.23.
//

#ifndef CURIUM_OBC_SOFTWARE_CURIUM_H
#define CURIUM_OBC_SOFTWARE_CURIUM_H

#include "radio.h"
#include "conf.h"

/**
 * @brief struct to store previous reasons of reset
 * Includes counters for each individual candidate reason
 */
struct ror_counters {
    // Counter of resets caused by low power
    uint16_t low_power_counter;
    // Counter of resets caused by independent watchdog
    uint16_t independent_watchdog_counter;
    // Counter of resets caused by software
    uint16_t software_counter;
    // Counter of resets caused by brownout
    uint16_t brownout_counter;
};

/**
 * @brief settings stored in flash
 * Align 32- and 16-bit values on respective borders to enable packing without
 * unaligned access penalties.
 * Size of this struct should not exceed 223 bytes
 *
 * ATTENTION: remember to change the value of QUBIK_FLASH_MAGIC_VAL in conf.h to
 * a different unique random value when changing this structure!
 */
// TODO: make __attribute__((packed)), for now causes a compiler warning
struct qubik_settings {
    uint32_t init;
    uint16_t scid;				//!< CCSDS Spacecraft identifier
    uint32_t sync_word;
    struct ror_counters reset_counters;
    uint8_t first_deploy;			//!< First deployment True/False
    uint8_t antenna_first_deploy;		//!< First antenna deployment True/False
    uint16_t holdoff_timer;			//!< Post deploy hold of in seconds
    uint32_t tx_freq;
    uint32_t rx_freq;
    uint16_t telemetry_map[QUBIK_TELEMETRY_MAP_LEN];
    //struct ant_status antenna_status;
    //struct ant_power_stats antenna_pwr_stats;
    radio_encoding_t tm_resp_enc;
    radio_modulation_t tm_resp_mod;
    radio_baud_t tm_resp_baud;
    uint32_t rildos_gold_seed;
    uint8_t sha256[24];
    uint16_t trx_delay_ms;			//!< Delay in ms for switching from RX to TX
    uint8_t mute_flag;
    int8_t tm_resp_tx_power;
    uint8_t tm_resp_pa_en;
};

struct curium {
    struct radio hradio;
    struct max17261_conf hmax17261;
    struct power_status power;
    struct ft_storage ft_persist_mem;
    struct qubik_settings settings;
    uint32_t uptime_secs;
    fsm_state_t fsm_state;
    struct qubik_hw_status status;
    uint8_t telemetry_idx;
    uint8_t reason_of_reset;
    uint8_t experiment_req;
    uint8_t experiment_stop;
    uint8_t experiment_completed;
};

#endif //CURIUM_OBC_SOFTWARE_CURIUM_H
