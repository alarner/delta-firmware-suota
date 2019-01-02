/**
 *  @file    led_adapter.h
 *  @author  Matthew Jacobs (codingmatty)
 *  @date    9/13/2017
 *  @version 1.0
 *
 *  @brief Controller for on board RGB LED
 *
 *  @section DESCRIPTION
 *
 *  RGB LED Controller
 *
 *  This module controls the on board RGB LED using the built-in Dialog LED
 *   Driver. Ths built-in Driver requires the usage of either the Timer 2 PWM
 *   or the Breath timer. For the usage in our application, I have chosen the
 *   Timer 2 PWM. It is the easiest way to control the brightness of the LED by
 *   using 100 Hz frequency, and using the duty cycle as a simple percentage of
 *   0-100. The module allows controlling each light individually, or provides
 *   an ALL Enum to control all lights at once.
 *
 *  Hardware Note: All lights can be on at any time with different brightness
 *   levels, but the Red LED seems a little further from the other 2, and
 *   doesn't mix well with the other 2 colors. On the other hand, Blue and
 *   Green are able to mix fairly well to create a pretty Teal color.
 *
 */

#pragma once

#include <map>
#include <stdio.h>

extern "C" {
  #include "hw_led.h"
  #include "hw_timer2.h"
}

#include "app_config.h"
#include "application_state.h"
#include "led_color.h"


#ifdef TESTING
#include "stubs.h"
#define SET_LED_SRC(x, pwm) stub_hw_led_set_led##x##_src((pwm))
#define ENABLE_LED(x, state) stub_hw_led_enable_led##x((state))
#define ENABLE_TIMER_2() stub_hw_timer2_enable()
#else
#define SET_LED_SRC(x, pwm) hw_led_set_led##x##_src((pwm))
#define ENABLE_LED(x, state) hw_led_enable_led##x((state))
#define ENABLE_TIMER_2() hw_timer2_enable()
#endif

#define INIT_TIMER_2 hw_timer2_init
#define SET_DUTY_CYCLE hw_timer2_set_pwm_duty_cycle
#define GET_DUTY_CYCLE hw_timer2_get_pwm_duty_cycle

#define LED_TIMEOUT_DELAY OS_MS_2_TICKS(1000)
#define LED_HOLD_MIN_THRESHOLD OS_MS_2_TICKS(1000)

namespace Climbalytics {

  class LedAdapter
  {
    uint8_t colorStates;
    void setColorState(LedColor color, bool);

  public:
    LedAdapter();
    void init();
    bool getColorState(LedColor color);
    void disableColor(LedColor color);
    void enableColor(LedColor color);
    void enableColor(LedColor color, uint16_t brightnessLevel);
    void setColor(LedColor color);
    void setColor(LedColor color, uint16_t brightnessLevel);
    void toggleColor(LedColor color);
    void toggleColor(LedColor color, uint16_t brightnessLevel);
    void setBrightnessLevel(LedColor color, uint16_t brightnessLevel);
    uint16_t getBrightnessLevel(LedColor color);
    // Blinks a single color in a pattern:
    //
    //      on | off | on | off | on | pause
    //
    // and then repeats the pattern... This method is built to be called each time the LED should
    // change as to not block the calling task. It conveniently returns when it should be next
    // called.
    //
    // Param Description:
    //    color:      The color to blink
    //    numBlinks:  The number of times the light should turn on (per cycle)
    //    onTick:     The number of ticks that the light should stay on
    //    offTicks:   The number of ticks that the light should stay off
    //    pauseTicks: The number of ticks that the light should stay off before repeating the
    //                blinking pattern
    //    startOn:    True if the led should start lit, otherwise false
    //    startTick:  The elapsed ticks when the first call was made
    //
    // Return value:  The elapsed tick value when the next change is set to occur (ie. when this
    //                method should be called again)
    OS_TICK_TIME blink(
      LedColor color,
      uint8_t numBlinks,
      OS_TICK_TIME onTicks,
      OS_TICK_TIME offTicks,
      OS_TICK_TIME pauseTicks,
      bool startOn,
      OS_TICK_TIME startTick
    );
    std::map<LedColor, uint16_t> getBrightnessLevels();
    OS_TICK_TIME updateFromState(const ApplicationState *appState, const OS_TICK_TIME actionOccurredAt);
  };

}
