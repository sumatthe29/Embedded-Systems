/**
 * @file sleep_routines.c
 * @author Matthew Su
 * @date 10/2/2022
 * @brief Contains all the sleep_routine driver functions
 *
 */

/**************************************************************************
 ***************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
**************************************************************************/

//***********************************************************************************
// include statements
//***********************************************************************************

#include "sleep_routines.h"

//***********************************************************************************
// static / private variables
//***********************************************************************************


static int lowest_energy_mode[MAX_ENERGY_MODES]; // @suppress("Symbol is not resolved")

/***************************************************************************//**
 * @brief
 * Initialize the sleep_routines static/private array, lowest_energy_mode[],
 * to all zeroes
 *
 * @note
 * setting all values in lowest_energy_mode to 0 clears any residue values for all
 * energy mode instances.
 *
 ******************************************************************************/



void sleep_open(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  // Initialize the sleep_routines static /private array, lowest_energy_mode[], to all zeroes
  for (int i = 0; i < MAX_ENERGY_MODES; i++)
    {
      lowest_energy_mode[i] = 0;
    }

  CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * @brief
 * Utilized by a peripheral to prevent the Pearl Gecko from going into that sleep mode
 * while the peripheral is active. Increments the associated array element in
 * lowest_energy_mode[] by one.

 * @note
 * Must be atomic, critical protection statements applied to prevent interrupts during
 * change in value
 * @param[in]
 * takes in a uint32_t value, static and found in sleep_routine.h
 ******************************************************************************/

void sleep_block_mode(uint32_t EM)
{
  // Utilized by a peripheral to prevent the Mighty Gecko from going into that sleep mode while
  // the peripheral is active.
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  // It will increment the associated array element in lowest_energy_mode[] by one.
  lowest_energy_mode[EM]++;

  // Assert Statement
  EFM_ASSERT(lowest_energy_mode[EM] < 5);

  CORE_EXIT_CRITICAL();


}

/***************************************************************************//**
 * @brief
 * Utilized to release the processor from going into any sleep mode while peripheral
 * is active. Decrement  the associated array element in lowest_energy_mode[] by one.
 *
 * @note
 * Must be atomic, critical protection statements applied to prevent interrupts during
 * change in value
 * @param[in]
 * takes in a uint32_t value, static and found in sleep_routine.h
 ******************************************************************************/

void sleep_unblock_mode(uint32_t EM)
{
  // Utilized to release the processor from going into a sleep mode with a peripheral that is
  // no longer active.
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  // It will decrement the associated array element in lowest_energy_mode[] by one.
  lowest_energy_mode[EM]--;

  EFM_ASSERT(lowest_energy_mode[EM]>=0);

  CORE_EXIT_CRITICAL();

}

/***************************************************************************//**
 * @brief
 * Function that will enter the appropriate sleep Energy Mode based on the first
 * non-zero array element in lowest_energy_mode[]. Appropriate operations occur for
 * each instance of a EM being designated
 *
 * @note
 * Must be atomic, critical protection statements applied to prevent interrupts during
 * change in value
 ******************************************************************************/

void enter_sleep(void)
{
  // Function that will enter the appropriate sleep Energy Mode based on the first non-zero array element in
  // lowest_energy_mode[].

  // From lecture slides ECEN2370F22_L6_Sep19_EM.pdf

  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  if (lowest_energy_mode[EM0] > 0){}
  else if (lowest_energy_mode[EM1] > 0){}
  else if (lowest_energy_mode[EM2] > 0) EMU_EnterEM1();
  else if (lowest_energy_mode[EM3] > 0) EMU_EnterEM2(true);
  else EMU_EnterEM3(true);

  CORE_EXIT_CRITICAL();
  return;
}

/***************************************************************************//**
 * @brief
 * Function that returns which energy mode that the current system cannot enter,
 * otherwise recon-ed as the first non-zero array element in lowest_energy_mode[].
 *
 * @note
 * Utilizes a for loop to travel through array until a non-zero value is encountered.
 * MAX_ENERGY_MODES is used to mark the size of the array it is traversing.
 *
 * @param[out]
 * returns lem, default (no cases are activated) is EM4, otherwise calculated instance
 ******************************************************************************/


uint32_t current_block_energy_mode(void)
{
  // Function that returns which energy mode that the current system cannot enter,
  // the first non-zero array element in lowest_energy_mode[].

  uint32_t lem = EM4;
  for (int i = 0; i < MAX_ENERGY_MODES; i++)
    {
      if (lowest_energy_mode[i] != 0)
        {
          lem = i;
        }
    }
  return lem;

}


