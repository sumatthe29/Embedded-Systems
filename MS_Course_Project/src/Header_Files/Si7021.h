//***********************************************************************************
// Include files
//***********************************************************************************
#ifndef Si7021_HG
#define Si7021_HG

/* System include statements */
#include "brd_config.h"
#include "scheduler.h"
#include "i2c.h"
#include "HW_delay.h"
// #include "em_i2c.h"

/* Silicon Labs include statements */


/* The developer's include statements */
#define ENABLE true
#define MASTER true
#define POR_DELAY 80

#define FREQ I2C_FREQ_FAST_MAX
#define CLHR i2cClockHLRAsymetric

// #define SCL_ROUTE         I2C_ROUTELOC0_SCLLOC_LOC15
// #define SDA_ROUTE         I2C_ROUTELOC0_SDALOC_LOC15

#define DAD               0x40
#define HUM               0xF5
#define TEMP              0xF3
#define NUMBYTES          1
#define NOBYTES           0
#define BUSYBIT           0
#define RW                1

void si7021_i2c_open();
void si7021_read(uint32_t CB);
void si7021_write(uint32_t CB);
void si7021_read_user(uint32_t CB);
uint32_t si7021_read_convert();
uint32_t si7021_read_convert_temp();


#endif
