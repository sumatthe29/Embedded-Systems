/**
 * @file Si7021.c
 * @author Matthew Su
 * @date 10/6/2022
 * @brief Contains all the Si7021 driver functions
 *
 */

#include "Si7021.h"

static uint32_t read_result;
static uint32_t read_result_temp;


/***************************************************************************//**
 * @brief
 * init all I2C_OPEN_STRUCT operation values and transitions into i2c_open()
 *
 * @details
 * Uses timer_delay to delay while Si7021 is given max time (80ms) to fully power up.
 * Then calls a local struct to set values that will be passed on to i2c_open and used
 * during its setup.
 *
 ******************************************************************************/

void si7021_i2c_open()
{
  timer_delay(POR_DELAY);
  I2C_OPEN_STRUCT locali2c;

  locali2c.enable = ENABLE;
  locali2c.master = MASTER;
  locali2c.refFreq = 0; //Need to input some kind of value
  locali2c.freq = FREQ; //Need to input some kind of value
  locali2c.clhr = CLHR; // no idea what this value is
   // route location and enable
  locali2c.out_pin_SCL = SCL_ROUTE;
  locali2c.out_pin_SCL_en = true;
  locali2c.out_pin_SDA = SDA_ROUTE;
  locali2c.out_pin_SDA_en = true;

//  if(SDA_ROUTE == I2C_ROUTELOC0_SDALOC_LOC15){
//      i2c_open(I2C0,&locali2c);
//      }
//  if(SDA_ROUTE == I2C_ROUTELOC0_SDALOC_LOC19){
//      i2c_open(I2C0,&locali2c);
//    }

  i2c_open(I2C,&locali2c);

//  i2c_start(I2C_TypeDef *i2c, uint32_t dadd, uint32_t* rec_data, uint32_t hum_cmd, uint32_t bytes_num, bool busy, uint32_t callback)
//
//   i2c_start(I2C0, DAD, &read_result, HUM, 1, 0,CALLBACK);

}

/***************************************************************************//**
 * @brief
 * called during event scheduler in main, contains i2c_start function for SI7021 operations
 *
 * @details
 * called in main under if statement checking for event LETIMER0_UF_CB, input value is the
 * defined CALLBACK value of 0x160 which will remove the specific scheduled event and call
 * the described function. This is done in conjunction with the alternate shtc3_read() function
 * that performs a similar sweet of i2c_start function calls.
 *
 ******************************************************************************/

// void i2c_start(I2C_TypeDef *i2c, uint32_t dadd, uint32_t* rec_data, uint32_t sensor_cmd, uint32_t bytes_num, writebytes, bool busy, bool rw, uint32_t callback)

void si7021_read(uint32_t CB)
{
  // 0 for bytesw has dual purpose, when in SI7021 mode it will replicate the same << shift as before
  // it's only when in SHTC3 mode that it is used
  // read hum
  i2c_start(I2C, DAD, &read_result, HUM, NUMBYTES, NOBYTES, BUSYBIT, R, CB);
  // read temp
  i2c_start(I2C, DAD, &read_result_temp, TEMP, NUMBYTES, NOBYTES, BUSYBIT, R, CB);

}

void si7021_write(uint32_t CB)
{
  i2c_start(I2C, DAD, &read_result, 0xE6, 0, BUSYBIT, 1, 0, CB);
  // sish set to 0 to trigger shtc3 recognition
}

void si7021_read_user(uint32_t CB)
{
  i2c_start(I2C, DAD, &read_result, 0xE7, NUMBYTES, BUSYBIT, 1, RW, CB);

}

/***************************************************************************//**
 * @brief
 * converts read_result value from Si7021 into a usable value

 * @details
 * checks static global uint_32 value read_result and converts whatever value exists
 * within it into a usable humidity value that is relayed back into
 * scheduled_si7021_read_cb(void).
 *
 * @note
 * checking this value is possible by setting a breakpoint after the result variable
 * is calculated to compare that value to assumed room humidity.
 *
 ******************************************************************************/

uint32_t si7021_read_convert()
{
  uint32_t result = ((125*read_result)/(65536))-6;
  // return read_result to blank state
  read_result = 0;
  return result;
}

/***************************************************************************//**
 * @brief
 * converts read_result_temp value from Si7021 into a usable temperature value (F)

 * @details
 * checks static global uint_32 value read_result_temp and converts whatever value exists
 * within it into a usable temperature value that is relayed back into
 * scheduled_si7021_read_cb(void).
 *
 * @note
 * checking this value is possible by setting a breakpoint after the result variable
 * is calculated to compare that value (F) to surrounding temp.
 *
 ******************************************************************************/

uint32_t si7021_read_convert_temp()
{
  uint32_t result = ((176*read_result_temp)/(65536))-47;
  // convert temp from C to F
  result = (result * (9/5)) + 32;
  // return read_result to blank state
  read_result_temp = 0;
  return result;
}


