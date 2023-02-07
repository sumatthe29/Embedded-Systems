#ifndef BRD_CONFIG_HG
#define BRD_CONFIG_HG

//***********************************************************************************
// Include files
//***********************************************************************************
/* System include statements */


/* Silicon Labs include statements */
#include "em_cmu.h"
#include "em_gpio.h"

/* The developer's include statements */


//***********************************************************************************
// defined files
//***********************************************************************************

// GPIO pin setup
#define STRONG_DRIVE

// LED 0 pin is
#define LED0_PORT         gpioPortF
#define LED0_PIN          04u
#define LED0_DEFAULT      false   // Default false (0) = off, true (1) = on
#define LED0_GPIOMODE     gpioModePushPull

// LED 1 pin is
#define LED1_PORT         gpioPortF
#define LED1_PIN          05u
#define LED1_DEFAULT      false // Default false (0) = off, true (1) = on
#define LED1_GPIOMODE     gpioModePushPull

#ifdef STRONG_DRIVE
  #define LED0_DRIVE_STRENGTH   gpioDriveStrengthStrongAlternateStrong
  #define LED1_DRIVE_STRENGTH   gpioDriveStrengthStrongAlternateStrong
#else
  #define LED0_DRIVE_STRENGTH   gpioDriveStrengthWeakAlternateWeak
  #define LED1_DRIVE_STRENGTH   gpioDriveStrengthWeakAlternateWeak
#endif


// System Clock setup
#define MCU_HFXO_FREQ     cmuHFRCOFreq_32M0Hz


// LETIMER PWM Configuration
//#define LETIMER_ROUTELOC0_OUT0LOC_LOC01 (28)
//#define LETIMER_ROUTELOC0_OUT1LOC_LOC01 (29)

#define   PWM_ROUTE_0    LETIMER_ROUTELOC0_OUT0LOC_LOC28
#define   PWM_ROUTE_1    LETIMER_ROUTELOC0_OUT1LOC_LOC28


// Button configuration
#define BUTTON_0_PORT   gpioPortF
#define BUTTON_0_PIN    06u
#define BUTTON_0_CONFIG gpioModeInput // gpio pin mode as input
#define BUTTON_1_PORT   gpioPortF
#define BUTTON_1_PIN    07u
#define BUTTON_1_CONFIG gpioModeInput // gpio pin mode as input
#define BUTTON_DEFAULT true // input filter enabled


// Button interrupt configuration
#define BUTTON_0_INT_NUM BUTTON_0_PIN // Pin # for Push Button 0 (use Pin # def)
#define BUTTON_0_INT_RISING false // Do not trigger interrupt on rising edge
#define BUTTON_0_INT_FALLING true // Trigger interrupt on falling edge
#define BUTTON_0_INT_ENABLE true // Enable Interrupt
#define BUTTON_1_INT_NUM BUTTON_1_PIN // Pin # for Push Button 1 (use Pin # def)
#define BUTTON_1_INT_RISING false // Do not trigger interrupt on rising edge
#define BUTTON_1_INT_FALLING true // Trigger interrupt on falling edge
#define BUTTON_1_INT_ENABLE true // Enable Interrupt

// SI7021
#define SI7021_SCL_PORT gpioPortC
#define SI7021_SCL_PIN 11
#define SI7021_SDA_PORT gpioPortC
#define SI7021_SDA_PIN 10
#define SI7021_SENSOR_EN_PORT gpioPortB
#define SI7021_SENSOR_EN_PIN 10

#define SI_DRIVESTRENGTH gpioDriveStrengthWeakAlternateWeak

#define SI7021_SCL_GPIOMODE gpioModeWiredAnd
#define SI7021_SDA_GPIOMODE gpioModeWiredAnd

#define SI7021_SENSOR_EN_DEFAULT true
#define SI7021_SENSOR_EN_GPIOMODE gpioModePushPull


// SHTC3
#define SHTC3_SCL_PORT gpioPortB
#define SHTC3_SCL_PIN 6
#define SHTC3_SDA_PORT gpioPortB
#define SHTC3_SDA_PIN 7
#define SHTC3_SENSOR_EN_PORT gpioPortB
#define SHTC3_SENSOR_EN_PIN 7

#define SH_DRIVESTRENGTH gpioDriveStrengthWeakAlternateWeak

#define SHTC3_SCL_GPIOMODE gpioModeWiredAnd
#define SHTC3_SDA_GPIOMODE gpioModeWiredAnd

#define SHTC3_SENSOR_EN_DEFAULT true
#define SHTC3_SENSOR_EN_GPIOMODE gpioModePushPull


#define I2

#ifdef I2
#define I2C I2C0
#define SCL_ROUTE             I2C_ROUTELOC0_SCLLOC_LOC15
#define SDA_ROUTE             I2C_ROUTELOC0_SDALOC_LOC15
#define SCL_ROUTE_1           I2C_ROUTELOC0_SCLLOC_LOC6
#define SDA_ROUTE_1           I2C_ROUTELOC0_SDALOC_LOC6
#else
#define I2C I2C1
#define SCL_ROUTE             I2C_ROUTELOC0_SCLLOC_LOC19
#define SDA_ROUTE             I2C_ROUTELOC0_SDALOC_LOC19
#endif



//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************

#endif
