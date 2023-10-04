/*
 * watchdog.h
 *
 *  Created on: Oct 4, 2023
 *      Author: kryptokommunist
 */

#ifndef INC_WATCHDOG_H_
#define INC_WATCHDOG_H_

#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "cmsis_os.h"

#define MAX_TASKS_NUM                  32
#define CHARS_PER_TASK                 10

struct subsystem {
	uint8_t id;
	char name[CHARS_PER_TASK];
};
struct wdg_rec {
	struct subsystem  store_buffer[MAX_TASKS_NUM];
	uint32_t mask;
	uint8_t num_subsystems;
};

struct watchdog {
	uint8_t               subsystems_num;
	uint32_t              subsystems;
	uint32_t              subsystems_mask;
	uint8_t               registered;
	uint32_t              last_contact_secs;
	osMutexId             mtx;
	struct wdg_rec        *wdg_recorder;
	IWDG_HandleTypeDef    *hiwdg;
};

#define TOTAL_RAM_USED MAX_TASKS_NUM * sizeof(struct subsystem) + sizeof(uint32_t) + sizeof(uint8_t)

int
watchdog_init(struct watchdog *w, IWDG_HandleTypeDef *hiwdg,
              osMutexId mtx, uint8_t n, struct wdg_rec *wdg_recorder,
              uint8_t *mem_addr);

int
watchdog_register(struct watchdog *w, uint8_t *id, const char *name);

int
watchdog_reset(struct watchdog *w);

int
watchdog_reset_subsystem(struct watchdog *w, uint8_t id);


#endif /* INC_WATCHDOG_H_ */
