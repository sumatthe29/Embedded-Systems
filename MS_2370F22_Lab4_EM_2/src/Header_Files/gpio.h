//***********************************************************************************
// Include files
//***********************************************************************************
#ifndef GPIO_HG
#define GPIO_HG

/* System include statements */


/* Silicon Labs include statements */
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_assert.h"

/* The developer's include statements */
#include "app.h"
#include "brd_config.h"
#include "scheduler.h"

//***********************************************************************************
// defined files
//***********************************************************************************

//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void gpio_open(void);
void GPIO_ODD_IRQHandler(void);
void GPIO_EVEN_IRQHandler(void);

#endif
