/*
 * error.h
 *
 *  Created on: Oct 4, 2023
 *      Author: kryptokommunist
 */

#ifndef INC_ERROR_H_
#define INC_ERROR_H_

/**
 * Qubik COMMS error codes
 */
enum qubik_error_t {
	NO_ERROR,   //!< All ok
	INVAL_PARAM,//!< Invalid parameter
	OSDLP_INIT_ERROR, //!< OSDLP initialization error
	OSDLP_MTX_LOCK, //!< OSDLP mutex not locked
	OSDLP_QUEUE_EMPTY, //!< Access to empty queue
	OSDLP_QUEUE_FULL, //!< Access to full queue
	OSDLP_QUEUE_MEM_ERROR, //!< Queue memory error
	OSDLP_NULL, //!< Access to NULL structure
	FT_STORAGE_INIT_ERROR,  //!< Fault tolerant storage have not properly initialized
	FT_STORAGE_COR_MEM, //!< Flash storage is corrupted and contains invalid data
	FLASH_ERASE_ERROR, //!< Flash erase error
	FLASH_PROGRAM_ERROR, //!< Flash program error
	MTX_TIMEOUT_ERROR, //!< Mutex timeout error
	QUBIK_NOT_IMPL, //!< Not enough time baby :(
	QUBIK_PWR_ERROR, //!< Power reading error
	QUBIK_ERROR_NUM //!< Total number of errors. Should be always last!
};

static inline int
HAL_to_qubik_error(int err)
{
	return err + QUBIK_ERROR_NUM;
}


#endif /* INC_ERROR_H_ */
