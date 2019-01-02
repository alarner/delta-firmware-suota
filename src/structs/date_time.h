#pragma once

#include <stdint.h>

namespace Climbalytics
{

// 6 bytes
struct DateTime {
  uint8_t year = 0;
  uint8_t month = 0;
  uint8_t day = 0;
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t second = 0;
};

}