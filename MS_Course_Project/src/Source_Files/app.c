/**
 * @file app.c
 * @author Matthew Su
 * @date 10-2-2022
 * @brief
 * app.c contains functions app_letimer_pwm_open and app_peripheral_setup
 *
 */

//***********************************************************************************
// Include files
//***********************************************************************************
#include "app.h"



//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// Static / Private Variables
//***********************************************************************************


//***********************************************************************************
// Private functions
//***********************************************************************************

static void app_letimer_pwm_open(float period, float act_period, uint32_t out0_route, uint32_t out1_route);

//***********************************************************************************
// Global functions
//***********************************************************************************

/***************************************************************************//**
 * @brief
 * first function to run through when program is run
 *
 *
 * @details
 * acts as the central hub for which different peripheral setup related operations
 * are opened and occur. Once setup is complete, le_timer is started and the
 * peripheral is allowed to start counting
 *
 * @note
 * where the application code resides
 *
 ******************************************************************************/

void app_peripheral_setup(void){
  // open up and initialize state of scheduler
  sleep_open();
  scheduler_open();

  cmu_open();
  gpio_open();

  si7021_i2c_open();
  shtc3_open();

  // for si7021 write test
  // si7021_write(0);
  // prevents unintended letimer interference
  // timer_delay(1000);
  // si7021_read_user(CALLBACK);

  app_letimer_pwm_open(PWM_PER, PWM_ACT_PER, PWM_ROUTE_0, PWM_ROUTE_1);
  letimer_start(LETIMER0, true);   // letimer_start will inform the LETIMER0 peripheral to begin counting.
}

/***************************************************************************//**
 * @brief
 * sets values in a struct and passes that struct to letimer_pwm_open
 *
 *
 * @details
 * one of three ops that reside within app_peripheral_setup, creates a struct
 * APP_LETIMER_PWM_TYPEDEF pwmops and fills with proper values.
 *
 * @note
 *  The struct is then passed onto another op letimer_pwm_open
 *
 * @param[in]
 * period and act_period are float values representing seconds
 *
 * @param[in]
 * out0_route, out1_route carry uint32_t values for out route 0,1 to gpio port/pins
 ******************************************************************************/
void app_letimer_pwm_open(float period, float act_period, uint32_t out0_route, uint32_t out1_route){
  // Initializing LETIMER0 for PWM operation by creating the
  // letimer_pwm_struct and initializing all of its elements
  APP_LETIMER_PWM_TypeDef pwmops;
  pwmops.out_pin_route0 = out0_route;
  pwmops.out_pin_route1 = out1_route;
  pwmops.period = period;
  pwmops.active_period = act_period;

  pwmops.enable = false;
  pwmops.out_pin_0_en = false;
  pwmops.out_pin_1_en = false;

  pwmops.debugRun = false;

  pwmops.comp0_irq_enable = false;
  pwmops.comp0_cb = LETIMER0_COMP0_CB;



  pwmops.comp1_irq_enable = true;
  pwmops.comp1_cb = LETIMER0_COMP1_CB;

  pwmops.uf_irq_enable = true;
  pwmops.uf_cb = LETIMER0_UF_CB;

  // populate before declaring this
  letimer_pwm_open(LETIMER0,&pwmops);


//  letimer_pwm_open(LETIMER0, &letimer_pwm_struct);

}

void scheduled_letimer0_uf_cb(void)
{
  // EFM_ASSERT(!(get_scheduled_events() & LETIMER0_UF_CB));
  si7021_read(CALLBACK);
  shtc3_read();
//  si7021_write(0x170);
//  si7021_read_user(CALLBACK);
}

void scheduled_letimer0_comp1_cb(void)
{
  EFM_ASSERT(!(get_scheduled_events() & LETIMER0_COMP1_CB));
  // si7021_read(CALLBACK);
}

// set to false since we are not using comp0
void scheduled_letimer0_comp0_cb(void)
{
  EFM_ASSERT(!(get_scheduled_events() & LETIMER0_COMP0_CB));
  // EFM_ASSERT(false);
}

/***************************************************************************//**
 * @brief
 * gpio Call Backs for BUTTON1
 *
 * @details
 * BTN1 will increment the energy mode based on when the button is pressed.
 * Initialize a local variable for the current_block_em to store the return value
 * of current_block_energy_mode(). Default value is to return EM0 (static variable
 * found in sleep_routines.h) when the if statement fails. This means that it is at
 * its highest state and will wrap around to the lowest.
 *
 ******************************************************************************/

void scheduled_gpio_odd_irq_cb(void)
{
  uint32_t current_b;
  current_b = current_block_energy_mode();

  sleep_unblock_mode(current_b);
  if(current_b < 4)
    {
      sleep_block_mode(current_b+1);
    }
  else
    {
      sleep_block_mode(EM0);
    }
}

/***************************************************************************//**
 * @brief
 * gpio Call Backs for BUTTON0
 *
 * @details
 * BTN1 will decrement the energy mode based on when the button is pressed.
 * Initialize a local variable for the current_block_em to store the return value
 * of current_block_energy_mode(). Default value is to return EM4 (static variable
 * found in sleep_routines.h) if the if statement fails (at lowest position). This
 * will have it wrap around.
 *
 ******************************************************************************/

void scheduled_gpio_even_irq_cb(void)
{
  uint32_t current_b;
  current_b = current_block_energy_mode();

  sleep_unblock_mode(current_b);
  if(current_b > 0)
    {
      sleep_block_mode(current_b-1);
    }
  else
    {
      sleep_block_mode(EM4);
    }
}

/***************************************************************************//**
 * @brief
 * enables the led if humidity > 30, Si7021 specific
 *
 * @details
 * calculates a usable value for humidity and temperature using the results of data
 * recorded by the Si7021 specifically. Lights up LED1 if humidity > 30.
 *
 ******************************************************************************/

// this remains the same as uf_cb

void scheduled_si7021_read_cb(void)
{
  EFM_ASSERT(!(get_scheduled_events() & CALLBACK));
  uint32_t hum;
  hum = si7021_read_convert();

  //  never used, can comment back in to see value of temp received from the related
  //  conversion function
    uint32_t temp;
    temp = si7021_read_convert_temp();

  // led 1

  if (hum > HUM_EXCEED)
    {
      GPIO_PinOutSet(gpioPortF, 05u);
    }
  else
    {
      GPIO_PinOutClear(gpioPortF, 05u);
    }

  // add_scheduled_event(TEMP_START_CB);

}

/***************************************************************************//**
 * @brief
 * enables the led if humidity > 30, SHTC3 specific
 *
 * @details
 * calculates a usable value for humidity and temperature using the results of data
 * recorded by the SHTC3 specifically. Lights up LED0 if humidity > 30.
 *
 ******************************************************************************/

void scheduled_shtc3_read_cb(void)
{
  EFM_ASSERT(!(get_scheduled_events() & SH_CALLBACK));
  uint32_t hum = sh_hum_convert();

  // never used, can comment back in to see value of temp received from the related
   uint32_t temp;
   temp = sh_temp_convert();

  // led 0

  if (hum > HUM_EXCEED)
    {
      GPIO_PinOutSet(gpioPortF, 04u);
    }
  else
    {
      GPIO_PinOutClear(gpioPortF, 04u);
    }

  // add_scheduled_event(TEMP_START_CB);

}



