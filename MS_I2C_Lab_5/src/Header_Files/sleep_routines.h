/* Silicon Labs include statements */
#include "em_assert.h"
#include "em_core.h"
#include "em_emu.h"


/* The developer's include statements */
// #include "sleep_routines.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************
#define     EM0                     0
#define     EM1                     1
#define     EM2                     2
#define     EM3                     3
#define     EM4                     4
#define     MAX_ENERGY_MODES        5

//***********************************************************************************
// function prototypes
//***********************************************************************************

void sleep_open(void);
// Initialize the sleep_routines static /private array, lowest_energy_mode[], to all zeroes
void sleep_block_mode(uint32_t EM);
// Utilized by a peripheral to prevent the Mighty Gecko from going into that sleep mode while
// the peripheral is active. It will increment the associated array element in lowest_energy_mode[] by one.
void sleep_unblock_mode(uint32_t EM);
// Utilized to release the processor from going into a sleep mode with a peripheral that is
// no longer active. It will decrement the associated array element in lowest_energy_mode[] by one.
void enter_sleep(void);
// Function that will enter the appropriate sleep Energy Mode based on the first non-zero array element in
// lowest_energy_mode[].
uint32_t current_block_energy_mode(void);
// Function that returns which energy mode that the current system cannot enter,
// the first non-zero array element in lowest_energy_mode[].

