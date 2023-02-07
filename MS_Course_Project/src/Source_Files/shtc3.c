/**
 * @file shtc3.c
 * @author Matthew Su
 * @date 10/6/2022
 * @brief Contains all the shtc3 driver functions
 *
 */

#include "shtc3.h"


// static variables where values read for hum and temp are dumped and referenced
static uint32_t read_hum;
static uint32_t read_temp;

/***************************************************************************//**
 * @brief
 * init all I2C_OPEN_STRUCT operation values and transitions into i2c_open()
 *
 * @details
 * Function uses a local struct to initialize the basic values that will be
 * passed on to i2c_open and used during setup of the i2c. This function is called
 * during app_peripheral_setup along with other similar functions.
 *
 ******************************************************************************/

void shtc3_open()
{
  // timer_delay(POR_DELAY);
  I2C_OPEN_STRUCT locali2c;

  locali2c.enable = ENABLE;
  locali2c.master = MASTER;
  locali2c.refFreq = 0; //Need to input some kind of value
  locali2c.freq = FREQ; //Need to input some kind of value
  locali2c.clhr = CLHR; // no idea what this value is
   // route location and enable
  locali2c.out_pin_SCL = SCL_ROUTE_1;
  locali2c.out_pin_SCL_en = true;
  locali2c.out_pin_SDA = SDA_ROUTE_1;
  locali2c.out_pin_SDA_en = true;


  i2c_open(I2C1,&locali2c);

}
/***************************************************************************//**
 * @brief
 * called during event scheduler in main, contains i2c_start function for SHTC3 operation
 *
 * @details
 * called in main under if statement checking for event LETIMER0_UF_CB, input value is the
 * defined SH_CALLBACK value of 0x180 which will remove the specific scheduled event and call
 * the described function. This is done in conjunction with the alternate si7021_read() function
 * that performs a similar sweet of i2c_start function calls.
 *
 * @note
 * Of note is that there are delays of 2 ms added between each i2c_start call to give the previous
 * instance enough time to finish. When these delays were not in effect, the busy bit would not be
 * set back to false in time for the next instance, causing the program to hang as a result.
 *
 ******************************************************************************/

void shtc3_read()
{
    // doesn't take in any callback since there is now a null callback and an actual callback, references
    // what was already defined in h file

    // clear static values
    read_hum = 0;
    read_temp = 0;

    // void i2c_start(I2C_TypeDef *i2c, uint32_t dadd, uint32_t* rec_data, uint32_t sensor_cmd, uint32_t bytes_num, uint32_t bytes_w, bool busy, bool rw, uint32_t callback)
    timer_delay(SH_DELAY);
    // wakeup command
    i2c_start(I2C1, SHDAD, &read_hum, WAKEUP, ZEROBYTE, ONESHIFT, BUSYBIT, W, SKIPCB);
    timer_delay(SH_DELAY);
    // read hum command
    i2c_start(I2C1, SHDAD, &read_hum, MEASURE_HUM, ONEBYTE, ONESHIFT, BUSYBIT, R, SKIPCB);
    timer_delay(SH_DELAY);
    // read temp command
    i2c_start(I2C1, SHDAD, &read_temp, MEASURE_TEMP, ONEBYTE, ONESHIFT, BUSYBIT, R, SKIPCB);
    timer_delay(SH_DELAY);
    // sleep command
    i2c_start(I2C1, SHDAD, &read_hum, SLEEP, ZEROBYTE, ONESHIFT, BUSYBIT, W, SH_CALLBACK);
    timer_delay(SH_DELAY);
}

/***************************************************************************//**
 * @brief
 * converts read_hum value from shtc3 into a usable humidity value
 *
 * @details
 * checks static global uint_32 value read_hum and converts whatever value exists
 * within it into a usable humidity value that is relayed back into
 * scheduled_shtc3_read_cb(void). This value will be used to compare against our
 * baseline of humidity (30) and will trigger the led if greater then.
 *
 * @note
 * checking this value is possible by setting a breakpoint after the result variable
 * is calculated to compare that value to assumed room humidity.
 *
 ******************************************************************************/


uint32_t sh_hum_convert()
{
  uint32_t reshum = (100*read_hum)/65536;
  // extra insurance that the static variables were cleared correctly
  read_hum = 0;
  return reshum;
}

/***************************************************************************//**
 * @brief
 * converts read_temp value from shtc3 into a usable temperature value (F)
 *
 * @details
 * checks static global uint_32 value read_hum and converts whatever value exists
 * within it into a usable humidity value that is relayed back into
 * scheduled_shtc3_read_cb(void). This value is not actively used like with the hum
 * values of the si7021 and the shtc3 which are both used to trigger different leds
 * on the EMF32 board.
 *
 * @note
 * checking this value is possible by setting a breakpoint after the result variable
 * is calculated to compare that value (c) to surrounding temp.
 *
 ******************************************************************************/


uint32_t sh_temp_convert()
{
  uint32_t result = ((175*read_temp)/65536)-45;
  // extra insurance that the static variables were cleared correctly
  read_temp = 0;
  // convert from C to F
  result = (result * (9/5)) + 32;
  return result;
}
