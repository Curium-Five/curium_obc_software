/*
 * conf.h
 *
 *  Created on: Oct 4, 2023
 *      Author: kryptokommunist
 */

#ifndef INC_CONF_H_
#define INC_CONF_H_


/**
 * The amount of seconds we allow the spacecraft not to receive
 * command from the ground without resetting
 */
#define CURIUM_GS_WDG_PERIOD_SECS        (30 * 60 * 60)


/**
 * The wdg_task interval in milliseconds between checking if
 * all subsystems have been reset.
 */
#define WDG_TASK_DELAY_MS               200



#endif /* INC_CONF_H_ */
