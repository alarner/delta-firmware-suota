/**
 *  @file    rgb_led.cpp
 *  @author  Matthew Jacobs (codingmatty)
 *  @date    9/13/2017
 *  @version 1.0
 *
 *  @brief Implementation of Controller for on board RGB LED
 */

#include "led_adapter.h"

using namespace Climbalytics;

LedAdapter::LedAdapter() {

}

void LedAdapter::init()
{
  // Configure Timer 2 PWM
  timer2_config pwmConfig;

  pwmConfig.frequency = 100; // Hz
  pwmConfig.pwm2_start = 0;  // red
  pwmConfig.pwm2_end = 0;    // red
  pwmConfig.pwm3_start = 0;  // green
  pwmConfig.pwm3_end = 0;    // green
  pwmConfig.pwm4_start = 0;  // blue
  pwmConfig.pwm4_end = 0;    // blue

  INIT_TIMER_2(&pwmConfig);
  ENABLE_TIMER_2();

  // LED Settings - Use Timer 2 PWM
  SET_LED_SRC(1, HW_LED_SRC1_PWM2); // red
  SET_LED_SRC(2, HW_LED_SRC2_PWM3); // green
  SET_LED_SRC(3, HW_LED_SRC3_PWM4); // blue
}

void LedAdapter::setColorState(LedColor color, bool newState)
{
  if (newState)
  {
    this->colorStates = this->colorStates | color;
  }
  else
  {
    this->colorStates = this->colorStates & ~color;
  }
}

bool LedAdapter::getColorState(LedColor color)
{
  return (this->colorStates & color) == color;
}

void LedAdapter::disableColor(LedColor color)
{
  if ((LedColor_Red & color) == LedColor_Red)
  {
    ENABLE_LED(1, false);
  }

  if ((LedColor_Green & color) == LedColor_Green)
  {
    ENABLE_LED(2, false);
  }

  if ((LedColor_Blue & color) == LedColor_Blue)
  {
    ENABLE_LED(3, false);
  }

  this->setColorState(color, false);
}

void LedAdapter::enableColor(LedColor color)
{
  if ((LedColor_Red & color) == LedColor_Red)
  {
    ENABLE_LED(1, true);
  }

  if ((LedColor_Green & color) == LedColor_Green)
  {
    ENABLE_LED(2, true);
  }

  if ((LedColor_Blue & color) == LedColor_Blue)
  {
    ENABLE_LED(3, true);
  }

  this->setColorState(color, true);
}

void LedAdapter::enableColor(LedColor color, uint16_t brightnessLevel)
{
  this->enableColor(color);
  this->setBrightnessLevel(color, brightnessLevel);
}

void LedAdapter::setColor(LedColor color)
{
  this->disableColor(LedColor_All);
  this->enableColor(color);
}

void LedAdapter::setColor(LedColor color, uint16_t brightnessLevel)
{
  this->disableColor(LedColor_All);
  this->enableColor(color);
  this->setBrightnessLevel(color, brightnessLevel);
}

void LedAdapter::toggleColor(LedColor color)
{
  if (getColorState(color))
  {
    this->disableColor(color);
  }
  else
  {
    this->enableColor(color);
  }
}

void LedAdapter::toggleColor(LedColor color, uint16_t brightnessLevel)
{
  if (getColorState(color))
  {
    this->disableColor(color);
  }
  else
  {
    this->enableColor(color, brightnessLevel);
  }
}

void LedAdapter::setBrightnessLevel(LedColor color, uint16_t brightnessLevel)
{
  if ((LedColor_Red & color) == LedColor_Red)
  {
    SET_DUTY_CYCLE(HW_TIMER2_PWM_2, brightnessLevel);
  }

  if ((LedColor_Green & color) == LedColor_Green)
  {
    SET_DUTY_CYCLE(HW_TIMER2_PWM_3, brightnessLevel);
  }

  if ((LedColor_Blue & color) == LedColor_Blue)
  {
    SET_DUTY_CYCLE(HW_TIMER2_PWM_4, brightnessLevel);
  }
}

uint16_t LedAdapter::getBrightnessLevel(LedColor color)
{
  switch (color)
  {
  case LedColor_Red:
    return GET_DUTY_CYCLE(HW_TIMER2_PWM_2);
  case LedColor_Green:
    return GET_DUTY_CYCLE(HW_TIMER2_PWM_3);
  case LedColor_Blue:
    return GET_DUTY_CYCLE(HW_TIMER2_PWM_4);
  case LedColor_All:
    return 0; // Can not get the brightness level of ALL colors at once
  }
  return 0;
}

std::map<LedColor, uint16_t> LedAdapter::getBrightnessLevels()
{
  std::map<LedColor, uint16_t> brightnessLevels;

  brightnessLevels[LedColor_Red] = GET_DUTY_CYCLE(HW_TIMER2_PWM_2);
  brightnessLevels[LedColor_Green] = GET_DUTY_CYCLE(HW_TIMER2_PWM_3);
  brightnessLevels[LedColor_Blue] = GET_DUTY_CYCLE(HW_TIMER2_PWM_4);

  return brightnessLevels;
}

OS_TICK_TIME LedAdapter::blink(
  LedColor color,
  uint8_t numBlinks,
  OS_TICK_TIME onTicks,
  OS_TICK_TIME offTicks,
  OS_TICK_TIME pauseTicks,
  bool startOn,
  OS_TICK_TIME startTick
) {
  OS_TICK_TIME nextUpdate;

  int beforePause = (numBlinks * onTicks) + (numBlinks - (startOn ? 1 : 0)) * offTicks;
  int max =  beforePause + pauseTicks;
  OS_TICK_TIME elapsedTicks = (OS_GET_TICK_COUNT() - startTick) % max;
  bool on = false;
  if(elapsedTicks < beforePause) {
    OS_TICK_TIME phase = elapsedTicks % (onTicks + offTicks);
    if(startOn) {
      if(phase < onTicks) {
        on = true;
        nextUpdate = onTicks - phase;
      }
      else {
        on = false;
        nextUpdate = onTicks + offTicks - phase;
      }

    }
    else {
      if(phase > offTicks) {
        on = true;
        nextUpdate = onTicks + offTicks - phase;
      }
      else {
        on = false;
        nextUpdate = offTicks - phase;
      }
    }
  }
  else {
    nextUpdate = max - elapsedTicks;
  }

  this->setColor(on ? color : LedColor_None);

  return nextUpdate;
}

OS_TICK_TIME LedAdapter::updateFromState(const ApplicationState *appState, const OS_TICK_TIME actionOccurredAt) {
  if(appState->on) {
    // We recently discovered a new tag
    if(appState->lastUniqueTagReadAt != 0 && actionOccurredAt < appState->lastUniqueTagReadAt + RFID_READ_LED_TIMEFRAME) {
      this->setColor(LedColor_Red);
      return appState->lastUniqueTagReadAt + RFID_READ_LED_TIMEFRAME - actionOccurredAt;
    }
    // The RFID reader is on
    else if(appState->rfidReading) {
      this->setColor(LedColor_Blue);
    }
    // Standby mode
    else {
      this->setColor(LedColor_Green);
    }
  }
  // Off
  else {
    this->setColor(LedColor_None);
  }
  return portMAX_DELAY;
}
