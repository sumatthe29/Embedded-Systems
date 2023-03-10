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

//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************

#endif
