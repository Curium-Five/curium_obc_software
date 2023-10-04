/*
 * watchdog.c
 *
 *  Created on: Oct 4, 2023
 *      Author: kryptokommunist
 */


#include "watchdog.h"
#include "error.h"
#include "conf.h"
#include <string.h>

/**
 * Initializes the watchdog. The watchdog needs to be reset
 * every 4095/128Hz = 31,99 seconds. Its tick frequency is
 * 32768kHz/256=128Hz.
 * @param w the watchdog structure
 * @param hiwdg the IWDG instance
 * @param mtx a valid mutex identifier
 * @param n the total number of sybsystems
 * @param tb_buffer memory space where watchdog
 * reset information is stored at init
 * @param mem_addr memory address where watchdog
 * information is regularly stored
 * @return 0 on success or negative error code
 */
int
watchdog_init(struct watchdog *w, IWDG_HandleTypeDef *hiwdg,
              osMutexId mtx, uint8_t n, struct wdg_rec *wdg_recorder,
              uint8_t *mem_addr)
{
	if (!w || !hiwdg || !mtx || n > 32) {
		return -INVAL_PARAM;
	}

	w->registered = 0;
	w->subsystems = 0;
	w->subsystems_mask = 0;
	w->subsystems_num = n;
	w->subsystems_mask = (1 << n) - 1;
	w->mtx = mtx;
	w->wdg_recorder = (struct wdg_rec *)mem_addr;
	w->hiwdg = hiwdg;
	w->last_contact_secs = 0;

	/* Move pre-reset software watchdog info from RAM to buffer*/

	memcpy(wdg_recorder, w->wdg_recorder,
	       sizeof(struct wdg_rec));
	return NO_ERROR;
}

/**
 * Register a new subsystem. The registration is thread safe and internally
 * locks/unlocks the mutex provided at the \ref watchdog_init()
 * @param w the wathcdog structure
 * @param id pointer to store the ID of the subsystem
 * @param task_id ID of the caller task defined in conf.h
 * @return 0 on success or negative error code
 */
int
watchdog_register(struct watchdog *w, uint8_t *id, const char *name)
{
	if (!w) {
		return -INVAL_PARAM;
	}
	osMutexWait(w->mtx, osWaitForever);
	if (w->registered >= w->subsystems_num) {
		osMutexRelease(w->mtx);
		return -INVAL_PARAM;
	}
	*id = w->registered;
	struct subsystem *t;
	t = &w->wdg_recorder->store_buffer[w->registered];
	t->id = w->registered;
	strncpy(t->name, name, CHARS_PER_TASK);
	w->registered++;
	w->wdg_recorder->num_subsystems = w->registered;
	osMutexRelease(w->mtx);
	return NO_ERROR;
}

/**
 * Resets a specific subsystem
 * @param w the watchdog structure
 * @param id the ID of the subsystem acquired with \ref watchdog_register()
 * @return 0 on success or negative error code
 */
int
watchdog_reset_subsystem(struct watchdog *w, uint8_t id)
{
	if (!w || id >= w->registered) {
		return -INVAL_PARAM;
	}

	osMutexWait(w->mtx, osWaitForever);
	w->subsystems |= 1 << id;
	w->wdg_recorder->mask = w->subsystems;
	osMutexRelease(w->mtx);

	return NO_ERROR;
}

/**
 * This function is responsible to reset the MCUs IWDG iff all of the registered
 * subsystems have already called at least once the
 * \ref watchdog_reset_subsystem(). If not, eventually the IWDG will be
 * triggered and the MCU will reset
 *
 * @param w the watchdog structure
 * @return 0 on success or negative error code.
 * @note: A 0 return value means that the call was successful not that the
 * IWDG was reset.
 */
int
watchdog_reset(struct watchdog *w)
{
	if (!w) {
		return -INVAL_PARAM;
	}

	/*
	 * If the GS elapsed time has passed the limit, do not reset the
	 * hardware watchdog. This will eventually reset the entire system
	 */
	if (w->last_contact_secs >= CURIUM_GS_WDG_PERIOD_SECS) {
		return NO_ERROR;
	}
	if (w->subsystems == w->subsystems_mask) {
		w->subsystems = 0;
		HAL_IWDG_Refresh(w->hiwdg);
	}
	return NO_ERROR;
}
