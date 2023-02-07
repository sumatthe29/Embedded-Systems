/**
 * @file app.c
 * @author Matthew Su
 * @date 9-15-2022
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
  cmu_open();
  gpio_open();
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
  pwmops.out_pin_0_en = true;
  pwmops.out_pin_1_en = true;

  pwmops.debugRun = false;

  // populate before declaring this
  letimer_pwm_open(LETIMER0,&pwmops);


//  letimer_pwm_open(LETIMER0, &letimer_pwm_struct);

}


