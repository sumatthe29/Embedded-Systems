/**
 * @file i2c.c
 * @author Matthew Su
 * @date 10/6/2022
 * @brief Contains all the I2C driver functions
 *
 */

#include "i2c.h"

// private function prototype as described in lab5 feedback
static void i2c_bus_reset(I2C_TypeDef *i2c);

/* Private-static functions for ack, nack, rxdatav: The state machine support
 * functions must be localized to the i2c.c module to prevent any other function
 * from call or interfering with them. They must have static function prototypes
 * within the i2c.c module.
 */

static void i2c_ack(I2C_STATE_MACHINE* ism);
static void i2c_nack(I2C_STATE_MACHINE* ism);
static void i2c_rxstate(I2C_STATE_MACHINE* ism);
static void i2c_mstop(I2C_STATE_MACHINE* ism);

static I2C_STATE_MACHINE i2c0_state_machine;
static I2C_STATE_MACHINE i2c1_state_machine;

/***************************************************************************//**
 * @brief
 *   Init i2c related items and enables interrupts
 *
 * @details
 *   Looks to enable values in the I2C_Init_TypeDef, maually defined in this
 *   function and enabled according to what we need, in this case clock freqs,
 *   pinouts, routepins, etc. Additionally, interrupts for ACK, NACK, RXDATAv
 *   and MSTOP are enabled.
 *
 * @note
 *   This function is normally called once to initialize the peripheral and the
 *   function i2c_start() is called to turn-on the Si7021/SHTC3 ops.
 *
 * @param[in] I2C_TypeDef *i2c
 *   Pointer to the base peripheral address of I2C0 or I2C1 which will be used
 *
 * @param[in] I2C_OPEN_STRUCT *i2c_setup
 *   Is the STRUCT that the calling routine will use to set the parameters for
 *   i2c ops.
 *
 ******************************************************************************/


void i2c_open(I2C_TypeDef *i2c, I2C_OPEN_STRUCT *i2c_setup)
{

  // declared locally
  I2C_Init_TypeDef i2c_in;

  if (i2c == I2C0 ){
        CMU_ClockEnable(cmuClock_I2C0 , true);
    }

  if (i2c == I2C1){
      CMU_ClockEnable(cmuClock_I2C1, true);
  }

  if ((i2c->IF & IFEB) == BYTEZERO)
    {
       i2c->IFS = IFEB;
       EFM_ASSERT(i2c->IF & IFEB);
       i2c->IFC = IFEB;
    }

  else
    {
      i2c->IFC = IFEB;
      EFM_ASSERT(!(i2c->IF & IFEB));
    }

  i2c_in.enable = i2c_setup->enable;
  i2c_in.master = i2c_setup->master;
  i2c_in.refFreq = i2c_setup->refFreq;
  i2c_in.freq = i2c_setup->freq;
  i2c_in.clhr = i2c_setup->clhr;
  // declared locally, reference address of local struct
  I2C_Init(i2c,&i2c_in);

//  i2c->ROUTELOC0 = i2c_setup->out_pin_SCL;
//  i2c->ROUTELOC0 |= i2c_setup->out_pin_SDA;
//  i2c->ROUTEPEN = (i2c_setup->out_pin_SCL_en * I2C_ROUTELOC0_SCLLOC_LOC10) | (I2C_ROUTELOC0_SCLLOC_LOC10 * i2c_setup->out_pin_SDA_en);
  i2c->ROUTEPEN = (I2C_ROUTEPEN_SCLPEN * i2c_setup->out_pin_SCL_en | I2C_ROUTEPEN_SDAPEN * i2c_setup->out_pin_SDA_en);
  i2c->ROUTELOC0 = (i2c_setup->out_pin_SCL | i2c_setup->out_pin_SDA);

  i2c->IFC = I2C_IFC_MSTOP | I2C_IFC_ACK | I2C_IFC_RXFULL | I2C_IFC_NACK;
  i2c->IEN = I2C_IEN_MSTOP | I2C_IEN_ACK | I2C_IEN_RXDATAV | I2C_IEN_NACK;
//  i2c_setting->enable = false;
//  i2c_setting->master = true;
//  i2c_setting->refFreq = 0;
//    i2c_setting->freq = I2C_FREQ_FAST_MAX;
//    i2c_setting->clhr; //ratio 6:3 ratio

//  interrupt enables

  if(i2c == I2C0)
    {
      NVIC_EnableIRQ(I2C0_IRQn);
    }

  if(i2c == I2C1)
    {
      NVIC_EnableIRQ(I2C1_IRQn);
    }

  i2c_bus_reset(i2c); //call bus reset, inputing I2C_TypeDef that i2c_start

}

/***************************************************************************//**
 * @brief
 *  Responsible for resetting the bus for specified i2c_typedef
 *
 * @details
 *
 *  Sends out any resets and clears flags, thus reseting the bus for i2c. Called
 *  at the end of i2c_start to clear previous op status and reset for new round
 *
 * @note
 *  Ensure that the i2c peripheral state machine is available to accept a
 *  command by sending the ABORT command to the I2C peripheral to reset
 *  it’s internal state machine.
 *
 * @param[in] I2C_TypeDef *i2c
 *   Pointer to the base peripheral address of I2C0 or I2C1 which will be reset
 *
 ******************************************************************************/

static void i2c_bus_reset(I2C_TypeDef *i2c)
{
  /*Ensure that the i2c peripheral state machine is available to accept a
  command by sending the ABORT command to the I2C peripheral to reset
  it’s internal state machine */

  i2c->CMD = I2C_CMD_ABORT;

  // save the status of the i2c->IEN registers
  uint32_t ien = i2c->IEN;

  // disable interrupts
  i2c->IEN &= ~ien;
  // Clear the Interrupt Flag (IF) register via writing to the Interrupt
  // Flag Clear (IFC) register

  i2c->IFC = i2c->IF;

  // clear i2c transmit buffer
  i2c->CMD = I2C_CMD_CLEARTX;

  // simaltanious setting of CMD start/stop
  i2c->CMD = I2C_CMD_START | I2C_CMD_STOP;


  // Stall until stop is completed by checking the MSTOP IF bit
  while(!(i2c->IF & I2C_IF_MSTOP));
  i2c->IFC = i2c->IF;
  // write abort bit into i2c->CMD to reset the microcontroller
  i2c->CMD = I2C_CMD_ABORT;
  // reset status of IEN register to original values
  i2c->IEN = ien;
}

/***************************************************************************//**
 * @brief
 *  Starts the entire state_machine flowchart operation suite
 *
 * @details
 *
 *  Responsible for starting the specified global machine by setting values based on
 *  specified i2c_typedef values and ACKs, allowing for state_machine_tree to start
 *  working through it's eventually command operations to read the Si7021 sensor and
 *  eventually complete its ops and return to main
 *
 * @note
 *  Input values can also be passed as the pointer to a state_machine struct called
 *  locally in the function for which this function is called. ATM, it was not
 *  implemented to ensure that values were being input and received. Updated from lab 5
 *  to include items for SHTC3 operation.
 *
 *  Additionally descriptions of parameters directly correspond to what can be found in
 *  the STATE_MACHINE
 *
 * @param[in] I2C_TypeDef *i2c
 *   Pointer to the base peripheral address of I2C0 or I2C1 which will be reset
 *
 * @param[in] uint32_t dadd
 *   Device Address, known value of 0x40 that has been defined in Si7021.h
 *
 * @param[in] uint32_t* rec_data
 *   Pointer to address where a static uint32_t value stores the data generated by
 *   the Si7021 sensor when it reads the humidity.
 *
 * @param[in] uint32_t sensor_cmd
 *   Specific sensor command for Si7021 or SHTC3 specific operations. Can take in a 1 or
 *   two byte command.
 *
 * @param[in] uint32_t bytes_num
 *   Number of bytes to be read. Controls number of iterations of read ops in RX_STATE for
 *   Si7021 and SHTC3 variations.
 *
 * @param[in] uint32_t bytes_w
 *   Number of bytes to be written. Controls number of iterations of write ops in WX_STATE
 *   and INITILAIZATION STATE. Exclusively useful for the MSB and LSB portions of the
 *   sensor_cmd related to the SHTC3 hum and temp commands, represented as 1 during
 *   i2c_start call. It also controls the number of 8 shifts required during the passing of
 *   the sensor command, passing MSB (8*1) then LSB (8*0).
 *
 * @param[in] bool busy
 *   Bool value that blocks operation unless it is specified that a state_machine is
 *   not busy
 *
 * @param[in] bool rw
 *   Bool value that determines either read (1) or write (0) operation
 *
 * @param[in] uint32_t callback
 *   CALLBACK value that when placed into the scheduler is recognized and executes
 *   Si7021/SHTC3 related ops to generate an output for humidity and temperature
 *
 *
 ******************************************************************************/

void i2c_start(I2C_TypeDef *i2c, uint32_t dadd, uint32_t* rec_data, uint32_t sensor_cmd, uint32_t bytes_num, uint32_t bytes_w, bool busy, bool rw, uint32_t callback)
{
    // create a pointer to hold the address of the instance of our struct
    // removed in favor of direct access to state machines
    I2C_STATE_MACHINE * state_machine_pointer;

    sleep_block_mode(EM2);   //block sleep mode

    if(i2c == I2C0)//i2c0              //i2c0 //// I2C0 is a constant in an efm file
    {
        state_machine_pointer = &i2c0_state_machine;
        // This delay checks the busy bit of the previous instance of the state_machine_pointer (refs global state machine)
        // check busy bit BEFORE we create a new instance and assign values to see if the previous i2c operation is still going
        // i2c0 and i2c1 run independently so you only need to check (in this case) if i2c0 is busy
        while(state_machine_pointer->busy_bit);

        EFM_ASSERT((state_machine_pointer->current_state & _I2C_STATE_STATE_MASK) == I2C_STATE_STATE_IDLE);

                    // Address of global state machine i2c0

        //struct assignments
        state_machine_pointer->i2c = i2c;
        state_machine_pointer->device_address = dadd;
        state_machine_pointer->rec_data = rec_data;
        state_machine_pointer->sensor_cmd = sensor_cmd;
        state_machine_pointer->callback_event_request = callback;
        state_machine_pointer->bytes = bytes_num;
        state_machine_pointer->bytesw = bytes_w;
        state_machine_pointer->busy_bit = busy;
        state_machine_pointer->r_or_w = rw;



    }
    if(i2c == I2C1)//i2c1               //i2c1 // I2C1 is a constant in an efm file
    {
        state_machine_pointer = &i2c1_state_machine; //Address of the global state machine i2c1

        while(state_machine_pointer->busy_bit);             //delay while the previous i2c1 operation finishes
        EFM_ASSERT((state_machine_pointer->current_state & _I2C_STATE_STATE_MASK) == I2C_STATE_STATE_IDLE);    // X = the I2C peripheral #

        //struct assignments
        state_machine_pointer->i2c = i2c;
        state_machine_pointer->device_address = dadd;
        state_machine_pointer->rec_data = rec_data;
        state_machine_pointer->sensor_cmd = sensor_cmd;
        state_machine_pointer->callback_event_request = callback;
        state_machine_pointer->bytes = bytes_num;
        state_machine_pointer->bytesw = bytes_w;
        state_machine_pointer->busy_bit = busy;
        state_machine_pointer->r_or_w = rw;
     }

    state_machine_pointer->busy_bit = true;

    i2c->CMD = I2C_CMD_START;
    state_machine_pointer->current_state = INITIALIZE_STATE;
    // sends current state straight to WX_STATE in case of sh
//    if (!sish)
//      {
//        state_machine_pointer->current_state = WX_STATE;
//      }
    i2c->TXDATA = dadd << ONESHIFT | W;

}

/***************************************************************************//**
 * @brief
 *  Handles interrupts and directs to specific interrupt ops ack, nack, rxstate
 *  or mstop
 *
 * @details
 *
 *  Designed to be automatically called when interrupt is specified in order to
 *  direct it to correct function within it's available options. This handler is
 *  specifically for I2C0
 *
 * @note
 *  Already referenced in source code, no need to declare it in i2c.h
 *
 ******************************************************************************/

void I2C0_IRQHandler(void)
{
  uint32_t flag = I2C0->IEN & I2C0->IF;
  I2C0->IFC = flag;

  if (flag & I2C_IF_ACK)
    {
      i2c_ack(&i2c0_state_machine);
    }
  if (flag & I2C_IF_NACK)
    {
      i2c_nack(&i2c0_state_machine);
    }
  if (flag & I2C_IF_RXDATAV)
    {
      i2c_rxstate(&i2c0_state_machine);
    }
  if (flag & I2C_IF_MSTOP)
    {
      i2c_mstop(&i2c0_state_machine);
    }
}

/***************************************************************************//**
 * @brief
 *  Handles interrupts and directs to specific interrupt ops ack, nack, rxstate
 *  or mstop
 *
 * @details
 *
 *  Designed to be automatically called when interrupt is specified in order to
 *  direct it to correct function within it's available options. This handler is
 *  specifically for I2C1
 *
 * @note
 *  Already referenced in source code, no need to declare it in i2c.h
 *
 ******************************************************************************/

void I2C1_IRQHandler(void)
{
  uint32_t flag = I2C1->IEN & I2C1->IF;
  I2C1->IFC = flag;

  if (flag & I2C_IF_ACK)
    {
      i2c_ack(&i2c1_state_machine);
    }
  if (flag & I2C_IF_NACK)
    {
      i2c_nack(&i2c1_state_machine);
    }
  if (flag & I2C_IF_RXDATAV)
    {
      i2c_rxstate(&i2c1_state_machine);
    }
  if (flag & I2C_IF_MSTOP)
      {
        i2c_mstop(&i2c1_state_machine);
      }
}

/***************************************************************************//**
 * @brief
 *  ACKNOWLEDGEMENT of process during flowchart progression
 *
 * @details
 *
 *  Called multiple times in order to make sure progression within the flowchart
 *  is correct. Once acked, different enum states contain different ops relating to
 *  the recording of data reported by the Si7021 and SHTC3 during a sensor_cmd.
 *  After each ack, it progresses to a new state.
 *
 *  States that occur in the ack are described as follows;
 *
 *  INITILAIZATION STATE: Where the ack starts and the first state set by i2c_start.
 *  A "read or write" boolean determines the course of action that all the other acks
 *  will take.
 *
 *  If read, will write to TXDATA the accompanying sensor command and shifted
 *  based on bytesw, a num value that will determine both the increment of 8 required
 *  to be shifted by as well as the number of writes that will occur. It's dual purpose
 *  does not affect SI7021 read ops, as when set to 0 it performs the normal TXDATA
 *  write without any shift and moves on to the SET_REG state. For the SHTC3, it will
 *  use the bytesw value to write the sensor_cmd in MSB and LSB segments to the register
 *  before it starts its actual read operation.
 *
 *  If write, then it will direct the ack to the WX_STATE and perform the first write
 *  op to follow to the WX_STATE.
 *
 *  WX_STATE: Instead of in it's own dedicated function, it was kept in the ack
 *  during the initial build and testing phase of the state. There seemed no reason
 *  to move it after so it remains within the ack itself. It replicates the same process
 *  as in the if statement for the read version of the INITILAZATION STATE, which also
 *  uses the bytesw to iterate a specific number of times and passing the MSB and LSB
 *  of the sensor command. Once it finishes it will direct immediatly to CLOSE_PROCESS
 *
 *  SET_REG: Sets the next state to HOLD, resends a START command, and sets the register
 *  with the address of the device SI7021 or SHTC3
 *
 *  HOLD: Holds for a round of ack. Sets the next state to the RX_STATE.
 *
 *  @note
 *
 *  For some reason in the INITILAIZATION STATE, for the write branch the ops could not
 *  all be completed in the WX_STATE itself, either doing one too many or one too few.
 *  This solution was to perform the first write op locally in the I_S before moving to the
 *  WX_STATE. It's jank but was validated to be a working solution so it was kept.
 *
 * @param[in] I2C_STATE_MACHINE* ism
 *   Pointer to address of global state_machine specified either to be I2C0 or I2C1 by
 *   the respective IRQ_handler
 *
 ******************************************************************************/

static void i2c_ack(I2C_STATE_MACHINE* ism)
{
  switch(ism->current_state)
  {
    case (INITIALIZE_STATE):
        if (ism->r_or_w)
          {
            // in shtc3 mode, before it reads, need to do a write in order for sensor to know whether or not to
            // read temp or hum

            ism->i2c->TXDATA = (ism->sensor_cmd) >> (EIGHTSHIFT)*(ism->bytesw);
            // ism->i2c->TXDATA = 1;

            if(ism->bytesw == 0)
            {
                ism->current_state = SET_REG;
                break;
            }

            ism->bytesw--;
          }
        else
          {
            ism->current_state = WX_STATE;
            // shift 8 once
            ism->i2c->TXDATA = (ism->sensor_cmd) >> (EIGHTSHIFT)*(ism->bytesw);
            // ism->i2c->TXDATA = ism->sensor_cmd;
          }

        break;
    case (SET_REG):
        ism->current_state = HOLD;
        ism->i2c->CMD = I2C_CMD_START;
        ism->i2c->TXDATA = (ism->device_address << ONESHIFT) | R;
        break;
    case (HOLD):
        ism->current_state = RX_STATE;
        break;
    case (WX_STATE):
        // uint32_t write data into state machine set = 1
        ism->bytesw--;
        ism->i2c->TXDATA = (ism->sensor_cmd) >> (EIGHTSHIFT)*(ism->bytesw);
        // ism->i2c->TXDATA = 1;

        // Less then for si7021 case
        if(ism->bytesw <= 0)
        {
          ism->current_state = CLOSE_PROCESS;
        }

        break;
    case (RX_STATE):
        // EFM_ASSERT(false);
        *ism->rec_data = 0;
        break;
    case (CLOSE_PROCESS):
        ism->i2c->CMD = I2C_CMD_STOP;
        break;
    default:
        EFM_ASSERT(false);
        break;

  }
}

/***************************************************************************//**
 * @brief
 *  NOT ACKNOWLEDGEMENT of data received in the sensor
 *
 * @details
 *
 *  Waits during HOLD state to make sure data is received and a restart is passed,
 *  ops for state HOLD only.
 *
 * @param[in] I2C_STATE_MACHINE* ism
 *   Pointer to address of global state_machine specified either to be I2C0 or I2C1 by
 *   the respective IRQ_handler
 *
 ******************************************************************************/

static void i2c_nack(I2C_STATE_MACHINE* ism)
{
  switch(ism->current_state)
  {
    case (HOLD):
        ism->i2c->CMD = I2C_CMD_START;
        ism->i2c->TXDATA = (ism->device_address << ONESHIFT) | R;
        break;
    default:
        EFM_ASSERT(false);
        break;

  }
}

/***************************************************************************//**
 * @brief
 *  Reads value from the Si7021 or SHTC3 based on # bytes specified
 *
 * @details
 * Reads value from the Si7021/SHTC3 based on # bytes specified, when operation is complete,
 * sends two commands NACK and STOP. Takes care of state RX_STATE.
 *
 * @param[in] I2C_STATE_MACHINE* ism
 *   Pointer to address of global state_machine specified either to be I2C0 or I2C1 by
 *   the respective IRQ_handler
 *
 ******************************************************************************/

static void i2c_rxstate(I2C_STATE_MACHINE *ism)
{
  switch(ism->current_state)
  {
    case (RX_STATE):
        *(ism->rec_data) |= ism->i2c->RXDATA;
        if(ism->bytes > BYTEZERO)
        {

            *(ism->rec_data) = *(ism->rec_data) << EIGHTSHIFT;

            ism->i2c->CMD = I2C_CMD_ACK;
            ism->bytes--;
        }
        else
          {
            ism->current_state = CLOSE_PROCESS;
            ism->i2c->CMD = I2C_CMD_NACK;
            ism->i2c->CMD = I2C_CMD_STOP;
          }
        break;
    default:
        EFM_ASSERT(false);
        break;

  }
}


/***************************************************************************//**
 * @brief
 *  End of flowchart operations, terminates and adds event to event scheduler
 *
 * @details
 *
 *  Hardstop that the state_machine flowchart will come to at end of ops, unblocks
 *  sleep mode, sets the state_machine to not busy, and adds scheduled event to the
 *  scheduler so it can start the process of translating and reporting the humidity
 *  value. Takes care of state CLOSED_PROCESS;
 *
 *
 * @param[in] I2C_STATE_MACHINE* ism
 *   Pointer to address of global state_machine specified either to be I2C0 or I2C1 by
 *   the respective IRQ_handler
 *
 ******************************************************************************/

static void i2c_mstop(I2C_STATE_MACHINE* ism)
{
  switch(ism->current_state)
  {
//    case (INITIALIZE_STATE):
//        // ism->current_state = SET_REG;
//        EFM_ASSERT(false);
//        break;
//    case (SET_REG):
//        EFM_ASSERT(false);
//        break;
//    case (HOLD):
//        EFM_ASSERT(false);
//        break;
//    case (RX_STATE):
//        ism->current_state = CLOSE_PROCESS;
//        ism->busy_bit = false;
//        sleep_unblock_mode(EM2);
//        break;
    case (CLOSE_PROCESS):
        ism->current_state = CLOSE_PROCESS;
        ism->busy_bit = false;
        sleep_unblock_mode(EM2);
        add_scheduled_event(ism->callback_event_request);
        break;
    default:
        EFM_ASSERT(false);
        break;
  }
}

