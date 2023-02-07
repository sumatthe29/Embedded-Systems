//***********************************************************************************
// Include files
//***********************************************************************************
#ifndef APP_HG
#define APP_HG

/* System include statements */


/* Silicon Labs include statements */
#include "em_cmu.h"
#include "em_assert.h"
#include "scheduler.h"

/* The developer's include statements */
#include "cmu.h"
#include "gpio.h"
#include "letimer.h"
#include "brd_config.h"
#include "Si7021.h"
#include "shtc3.h"


//***********************************************************************************
// defined files
//***********************************************************************************
#define   PWM_PER           3   // PWM period in seconds
#define   PWM_ACT_PER       0.002  // PWM active period in seconds

#define   GPIO_ODD_IRQ_CB             0x00000008
#define   GPIO_EVEN_IRQ_CB            0x00000016


//***********************************************************************************
// Application scheduled events
//***********************************************************************************

#define   LETIMER0_COMP0_CB         0x00000001 //0b0001
#define   LETIMER0_COMP1_CB         0x00000002 //0b0002
#define   LETIMER0_UF_CB            0x00000004 //0b0004

//***********************************************************************************
// global variables
//***********************************************************************************
// CALLBACK = HUM_MEASURE_CB
#define   CALLBACK                  0x160
#define   SH_CALLBACK               0x180
#define   TEMP_START_CB             0x175
#define   TEMP_MEASURE_CB           0x180
#define   HUM_EXCEED                30

//***********************************************************************************
// function prototypes
//***********************************************************************************
void app_peripheral_setup(void);
void scheduled_letimer0_uf_cb(void);
void scheduled_letimer0_comp0_cb(void);
void scheduled_letimer0_comp1_cb(void);

void scheduled_gpio_odd_irq_cb(void);
void scheduled_gpio_even_irq_cb(void);

void scheduled_si7021_read_cb(void);
void scheduled_shtc3_read_cb(void);


#endif
