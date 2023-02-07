//***********************************************************************************
// Include files
//***********************************************************************************
#ifndef I2C_HG
#define I2C_HG

/* System include statements */


/* Silicon Labs include statements */


/* The developer's include statements */

#define SMB   2
#define I2C_EM_BLOCK EM1
#define W     0
#define R     1
#define BYTEZERO 0
#define EIGHTSHIFT 8
#define ONESHIFT 1
#define IFEB 0x01

#include "brd_config.h"
#include "scheduler.h"
#include "sleep_routines.h"
#include "em_i2c.h"
typedef struct {

  bool      enable;          //  enable the I2C upon completion of open
  bool      master;          //  Set to Controller (true) or Target (false) mode.
  uint32_t  refFreq;         //  I2C reference clock assumed when configuring bus frequency setup.
  uint32_t  freq;            // (Max) I2C bus frequency to use.
  I2C_ClockHLR_TypeDef clhr; // Clock low/high ration control

  uint32_t  out_pin_SCL;     // out 0 route to gpio port/pin
  uint32_t  out_pin_SDA;     // out 1 route to gpio port/pin
  bool      out_pin_SCL_en;  // enable out 0 route
  bool      out_pin_SDA_en;  // enable out 1 route

  // add interrupts later

} I2C_OPEN_STRUCT ;

typedef enum
{
  INITIALIZE_STATE,
  SET_REG,
  HOLD,
  WX_STATE,
  RX_STATE,
  CLOSE_PROCESS
}DEFINED_STATES;

typedef struct{
  //PG.23 #1
  DEFINED_STATES            current_state;
  // tells whether we are using I2C0 or I2C1
  I2C_TypeDef               *i2c;
  // what i2c device we are using
  uint32_t                  device_address;

  // hum-cmd is more general, basically stores whatever cmd was passed during start
  uint32_t                  sensor_cmd; //
  uint32_t                  *rec_data;
  // stores pointer to result
  // remember to left shift by 8 bits
  uint32_t                  bytes; //how many bytes to transfer
  uint32_t                  bytesw; // how many bytes to write
  //PG.24 #3
  // if bool = 1, resource is busy
  bool                      busy_bit;
  // read or write command bool, true = r, false = w
  bool                      r_or_w;
  // The callback event to request upon
  uint32_t                  callback_event_request;

}I2C_STATE_MACHINE;


void i2c_open(I2C_TypeDef *i2c, I2C_OPEN_STRUCT *i2c_setup);
void i2c_start(I2C_TypeDef *i2c, uint32_t dadd, uint32_t* rec_data, uint32_t sensor_cmd, uint32_t bytes_num, uint32_t bytes_w, bool busy, bool rw, uint32_t callback);



#endif
