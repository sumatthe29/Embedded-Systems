//***********************************************************************************
// Include files
//***********************************************************************************
#ifndef shtc3_HG
#define shtc3_HG

/* System include statements */

#include "brd_config.h"
#include "scheduler.h"
#include "i2c.h"
#include "HW_delay.h"
#include "Si7021.h"

/* defined command values */

#define SHDAD       0x70
#define WAKEUP      0x3517
#define TWO         2
#define SKIPCB      0
#define SH_DELAY    2
// temp first then hum read
#define MEASURE_HUM  0x58E0
#define MEASURE_TEMP 0x7866
#define SLEEP        0xB098

#define ONEBYTE  1
#define ZEROBYTE 0

#define   SH_CALLBACK               0x180

void shtc3_open();
void shtc3_read();
uint32_t sh_hum_convert();
uint32_t sh_temp_convert();

#endif
