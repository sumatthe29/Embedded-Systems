/***************************************************************************//**
 * @file
 * @brief Simple LED Blink Demo for SLSTK3402A
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/


#include "main.h"


int main(void)
{
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;

  /* Chip errata */
  CHIP_Init();

  /* Init DCDC regulator and HFXO with kit specific parameters */
  /* Init DCDC regulator and HFXO with kit specific parameters */
  /* Initialize DCDC. Always start in low-noise mode. */
  EMU_EM23Init_TypeDef em23Init = EMU_EM23INIT_DEFAULT;
  EMU_DCDCInit(&dcdcInit);
  em23Init.vScaleEM23Voltage = emuVScaleEM23_LowPower;
  EMU_EM23Init(&em23Init);
  CMU_HFXOInit(&hfxoInit);

  /* Switch HFCLK to HFRCO and disable HFRCO */
  CMU_HFRCOBandSet(MCU_HFXO_FREQ);          // Set main CPU oscillator frequency
  CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
  CMU_OscillatorEnable(cmuOsc_HFXO, false, false);

  /* Call application program to open / initialize all required peripheral */
  app_peripheral_setup();

  /* Infinite blink loop */
  while (1) {
    // EMU_EnterEM1();
    // EMU_EnterEM2(true);

    // EMU_EnterEM3(true);
    // get_scheduled_events();
    CORE_DECLARE_IRQ_STATE;

    CORE_ENTER_CRITICAL();
    if (!get_scheduled_events())
      {
          enter_sleep();

      }
    CORE_EXIT_CRITICAL();


    if (LETIMER0_COMP0_CB & get_scheduled_events())
      {
        remove_scheduled_event(LETIMER0_COMP0_CB);
        scheduled_letimer0_comp0_cb();

      }

    if (LETIMER0_COMP1_CB & get_scheduled_events())
      {
        remove_scheduled_event(LETIMER0_COMP1_CB);
        scheduled_letimer0_comp1_cb();

      }
    if (LETIMER0_UF_CB & get_scheduled_events())
      {
        remove_scheduled_event(LETIMER0_UF_CB);
        scheduled_letimer0_uf_cb();

      }
    if (CALLBACK & get_scheduled_events())
      {
        remove_scheduled_event(CALLBACK);
        scheduled_si7021_read_cb();

      }
//    if (GPIO_ODD_IRQ_CB & get_scheduled_events())
//      {
//        remove_scheduled_event(GPIO_ODD_IRQ_CB);
//        scheduled_gpio_odd_irq_cb();
//      }
//    if (GPIO_EVEN_IRQ_CB & get_scheduled_events())
//        {
//          remove_scheduled_event(GPIO_EVEN_IRQ_CB);
//          scheduled_gpio_even_irq_cb();
//        }
  }
}

