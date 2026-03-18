#ifndef SPELL_IOT_H
#define SPELL_IOT_H

#if !defined(SKYLINK_FIRMWARE_VERSION)
    #define SKYLINK_FIRMWARE_VERSION  "0.0.0"
#endif

#if defined(SPELLIOT_UPDATE)
  #define UPDATE_SKYLINK
#endif

#ifdef SPELLIOT_LED_DISPLAY
#define PETALLED_MATRIX
#include "Spelliot_Ledmatrix.hpp"
#endif

#ifdef SPELLIOT_STEPPER_MOTOR
#include "StepperMotor.h"
#endif

#include "iotCloud.h"
#include "iotCloud.hpp"

#endif

