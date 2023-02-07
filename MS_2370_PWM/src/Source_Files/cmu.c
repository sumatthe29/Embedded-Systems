/**
 * @file cmu.c
 * @author Matthew Su
 * @date 9-15-2022
 * @brief
 * first op (cmu_open), enables/sets different members of CMU
 *
 */


//***********************************************************************************
// Include files
//***********************************************************************************
#include "cmu.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// Private variables
//***********************************************************************************


//***********************************************************************************
// Private functions
//***********************************************************************************


//***********************************************************************************
// Global functions
//***********************************************************************************

/***************************************************************************//**
 * @brief
 * first op called during app_peripheral_setup
 *
 *
 * @details
 * enables different parts of the CMU clock tree
 * we want use the ULFRCO oscillator to enable operation down into PG energy
 * mode 3 level
 *
 * // Disable the LFRCO oscillator
    CMU_OscillatorEnable(cmuOsc_LFRCO , false, false);

   // Disable the Low Frequency Crystal Oscillator, LFXO
    CMU_OscillatorEnable(cmuOsc_LFXO, false, false);

   // Route LF clock to LETIMER0 clock tree
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO); // What clock tree does the LETIMER0 reside on?
 *
 * @note
 * CMU clock tree is established in this file
 *
 ******************************************************************************/


void cmu_open(void){

    CMU_ClockEnable(cmuClock_HFPER, true);

    // By default, Low Frequency Resistor Capacitor Oscillator, LFRCO, is enabled,
    // Disable the LFRCO oscillator
    CMU_OscillatorEnable(cmuOsc_LFRCO , false, false);  // What is the enumeration required for LFRCO?

    // Disable the Low Frequency Crystal Oscillator, LFXO
    CMU_OscillatorEnable(cmuOsc_LFXO, false, false);  // What is the enumeration required for LFXO?

    // No requirement to enable the ULFRCO oscillator.  It is always enabled in EM0-4H

    // Route LF clock to LETIMER0 clock tree
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO); // What clock tree does the LETIMER0 reside on?

    CMU_ClockEnable(cmuClock_CORELE, true); //This enumeration is found in the Lab 2 assignment

}

