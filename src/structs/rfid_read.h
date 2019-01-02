#pragma once

#include <stdint.h>

#include "date_time.h"

namespace Climbalytics
{

// 19 bytes
struct RfidRead {
  OS_TICK_TIME readAt = 0;
  uint8_t uid[RFID_UID_LENGTH];
  uint8_t rssi;
};

}
