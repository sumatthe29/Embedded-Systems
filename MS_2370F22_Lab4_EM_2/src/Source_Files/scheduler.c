/**
 * @file scheduler.c
 * @author Matthew Su
 * @date 10/2/2022
 * @brief Contains all the scheduler related functions
 *
 */

//***********************************************************************************
// include statements
//***********************************************************************************

#include "scheduler.h"
#include "em_assert.h"
#include "em_core.h"
#include "em_emu.h"

//***********************************************************************************
// static / private variables
//***********************************************************************************

static uint32_t event_scheduled;

#define CLEAR 0

/***************************************************************************//**
 * @brief
 * Opens the scheduler functionality by resetting the static / private variable
 * event_scheduled to 0.
 *
 * @note
 * Must be atomic since values are changed
 *
 ******************************************************************************/

void scheduler_open(void)
{
  //  Opens the scheduler functionality by
  //  resetting the static / private variable event_scheduled to 0
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  event_scheduled = CLEAR;

  CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * @brief
 * ORs a new event, the input argument, into the existing state of the private
 * (static) variable event_scheduled.
 *
 * @note
 * Must be atomic since values are changed
 *
 * @param[in]
 * takes in uint32_t value event
 *
 ******************************************************************************/

void add_scheduled_event(uint32_t event)
{
  //  ORs a new event, the input
  //  argument, into the existing state of the private (static) variable
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  event_scheduled |= event;

  CORE_EXIT_CRITICAL();

}

/***************************************************************************//**
 * @brief
 * Removes the event, the input argument, from the existing state of the private
 * (static) variable event_scheduled. This is done by &- events_scheduled with the
 * inverse (~) of input parameter event
 *
 * @note
 * Must be atomic since values are changed
 *
 * @param[in]
 * takes in uint32_t value event
 *
 ******************************************************************************/

void remove_scheduled_event(uint32_t event)
{
  // Removes the event, the
  // input argument, from the existing state of the private (static) variable
  // event_scheduled.
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  event_scheduled &= ~event;

  CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * @brief
 * Returns the current state of the private (static) variable event_scheduled.
 *
 ******************************************************************************/

uint32_t get_scheduled_events(void)
{
  //  Returns the current state of the private (static)
  //  variable event_scheduled
  return event_scheduled;
}
