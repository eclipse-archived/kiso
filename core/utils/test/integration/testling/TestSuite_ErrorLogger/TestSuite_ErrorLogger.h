#ifndef ERRORLOGGER_TESTSUITE_H_
#define ERRORLOGGER_TESTSUITE_H_

#include "BCDS_Retcode.h"

/* Constant definition */

#define MAX_ENTRY (ERRORLOGGER_SIZE/SIZEOF_LOGENTRY)

Retcode_T TestSuite_ErrorLogger_Initialize( uint8_t );

#endif /* ERRORLOGGER_TESTSUITE_H_ */
