/*
 * letimer.c
 *
 *  Created on: May 13, 2019
 *      Author: kgraham
 */


//***********************************************************************************
// Include files
//***********************************************************************************
#include "app.h"


//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function
//***********************************************************************************

// Question 3
void app_check_math(void){

  uint16_t int_variable = 7;
  float float_variable, float_result;
  float_variable = 2.5;

  float_variable = int_variable * float_variable;
  int_variable = float_variable/int_variable;
  float_result = float_variable/int_variable;
  int_variable = (int)(float_variable/int_variable);
}

void app_condition(void){
  uint16_t gecko,iguana,skink;
  gecko = 3;
  iguana= 0;
  skink = 1;
  if(skink == 0|| (gecko && !iguana))
    {
    if(!skink){
        iguana = 5;
    }

    }
  else
    {
      gecko = 5;
    }


}

// Question 8c

struct habitat{
    uint16_t gecko, iguana, skink;
    float heat_time;
  };

void app_check_structs_and_ptrs(void){
  struct habitat Ponderosa = {0,0,2,4.4};
  struct habitat Douglas = {4,0,0,2.0};

  struct habitat *current_habitat;
  current_habitat = &Ponderosa;
  --current_habitat->heat_time;

  current_habitat = &Douglas;
  ++current_habitat->heat_time;

}

void app_peripheral_setup(void){
	cmu_open();
	gpio_open();
}

void app_letimer0_open(void){

}
