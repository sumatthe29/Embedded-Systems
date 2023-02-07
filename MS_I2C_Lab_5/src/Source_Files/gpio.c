/**
 * @file gpio.c
 * @author Matthew Su
 * @date 10-2-2022
 * @brief
 * second op (gpio_open), enables/sets different members of GPIO
 *
 */

//***********************************************************************************
// Include files
//***********************************************************************************
#include "gpio.h"


//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// Private variables
//***********************************************************************************
static uint32_t gpio_even_irq_cb = GPIO_EVEN_IRQ_CB;
static uint32_t gpio_odd_irq_cb = GPIO_ODD_IRQ_CB;

//***********************************************************************************
// Private functions
//***********************************************************************************


//***********************************************************************************
// Global functions
//***********************************************************************************

/***************************************************************************//**
 * @brief
 * second op (gpio_open), enables/sets different members of GPIO
 *
 *
 * @details
 * enables DriveStrengthSet and PinModeSet for LED0 and LED1, as well as buttons
 * BUTTON0 and BUTTON1. Pins are updated for I2C#
 *
 * @note
 * New ops added from Lab4 include activating BUTTON0 and BUTTON1 on the pearl gecko
 *
 ******************************************************************************/

void gpio_open(void){

  CMU_ClockEnable(cmuClock_GPIO, true);

  // Configure LED pins
  GPIO_DriveStrengthSet(LED0_PORT, LED0_DRIVE_STRENGTH);
  GPIO_PinModeSet(LED0_PORT, LED0_PIN, LED0_GPIOMODE, LED0_DEFAULT);

  GPIO_DriveStrengthSet(LED1_PORT, LED1_DRIVE_STRENGTH);
  GPIO_PinModeSet(LED1_PORT, LED1_PIN, LED1_GPIOMODE, LED1_DEFAULT);

  GPIO_PinModeSet(BUTTON_0_PORT, BUTTON_0_PIN, BUTTON_0_CONFIG, BUTTON_DEFAULT);

  GPIO_PinModeSet(BUTTON_1_PORT, BUTTON_1_PIN, BUTTON_1_CONFIG, BUTTON_DEFAULT);

  GPIO_ExtIntConfig(BUTTON_0_PORT, BUTTON_0_PIN,
  BUTTON_0_INT_NUM, BUTTON_0_INT_RISING, BUTTON_0_INT_FALLING,
  BUTTON_0_INT_ENABLE);

  GPIO_ExtIntConfig(BUTTON_1_PORT, BUTTON_1_PIN,
  BUTTON_1_INT_NUM, BUTTON_1_INT_RISING, BUTTON_1_INT_FALLING,
  BUTTON_1_INT_ENABLE);

  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);

  GPIO_PinModeSet(SI7021_SENSOR_EN_PORT, SI7021_SENSOR_EN_PIN, gpioModePushPull, SI7021_SENSOR_EN_DEFAULT);
  GPIO_DriveStrengthSet(SI7021_SENSOR_EN_PORT, SI_DRIVESTRENGTH);
  GPIO_PinModeSet(SI7021_SCL_PORT, SI7021_SCL_PIN, gpioModeWiredAnd, SI7021_SENSOR_EN_DEFAULT);
  GPIO_PinModeSet(SI7021_SDA_PORT, SI7021_SDA_PIN, gpioModeWiredAnd, SI7021_SENSOR_EN_DEFAULT);





}

/***************************************************************************//**
 * @brief
 * IRQ handler for GPIO ODD (BUTTON1)
 *
 * @note
 * Based almost 1:1 on the LETIMER0 version of the IRQHandler
 *
 ******************************************************************************/

void GPIO_ODD_IRQHandler(void)
{
  uint32_t int_flag = (GPIO->IF) & (GPIO->IEN);
  GPIO->IFC = int_flag;
  add_scheduled_event(gpio_odd_irq_cb);

}

/***************************************************************************//**
 * @brief
 * IRQ handler for GPIO EVEN (BUTTON0)
 *
 * @note
 * Based almost 1:1 on the LETIMER0 version of the IRQHandler
 *
 ******************************************************************************/


void GPIO_EVEN_IRQHandler(void)
{
  uint32_t int_flag = (GPIO->IF) & (GPIO->IEN);
  GPIO->IFC = int_flag;
  add_scheduled_event(gpio_even_irq_cb);
}

