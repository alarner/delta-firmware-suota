#pragma once

#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern "C" {
  #include "osal.h"
}

#include "app_config.h"
#include "bluetooth_command.h"
#include "error_code.h"
#include "operating_mode.h"
#include "rfid_read.h"

namespace Climbalytics
{

class ApplicationState
{
  public:
    bool on = true;
    uint32_t batteryVoltage = 0;
    
    RfidRead lastRfidRead;
    uint16_t rfidPower = 0;
    bool rfidReading = false;
    RfidRead accumulatedTags[MAX_UNIQUE_TAGS_PER_BATCH];
    uint8_t accumulatedCounts[MAX_UNIQUE_TAGS_PER_BATCH];
    uint8_t accumulatedLength = 0;

    OS_TICK_TIME poweredStateChangedAt = 0;
    OS_TICK_TIME imuChangedAt = 0;
    OS_TICK_TIME buttonPressedAt = 0;
    OS_TICK_TIME buttonReleasedAt = 0;
    OS_TICK_TIME lastUniqueTagReadAt = 0;
    OS_TICK_TIME lastDateBlockWrittenAt = 0;

    ErrorCode errorCode = ErrorCode_None;
    OperatingMode mode = OperatingMode_Off;
    bool charging = false;
    bool usbAttached = false;
    BluetoothCommand bluetoothCommand = NULL;

    OS_TASK interruptTaskHandle = NULL;
    OS_TASK actionTaskHandle = NULL;
    OS_TASK rfidTaskHandle = NULL;
    OS_TASK bluetoothTaskHandle = NULL;
    OS_TASK periodicTaskHandle = NULL;

    bool canRead(OS_TICK_TIME now) {
      printf(
        "%d %d %d %d %d %d %d\n",
        this->on,
        !this->usbAttached,
        this->bluetoothCommand == BluetoothCommand_ReadSingleTag,
        (this->imuChangedAt != 0),
        (this->lastRfidRead.readAt < this->imuChangedAt),
        (now > this->imuChangedAt + IMU_DELAY_BEFORE_READ),
        (now < this->imuChangedAt + IMU_DELAY_BEFORE_READ + RFID_READABLE_TIMEFRAME)
      );
      return (
        this->on &&
        !this->usbAttached &&
        (
          this->bluetoothCommand == BluetoothCommand_ReadSingleTag ||
          (
            (this->imuChangedAt != 0) &&
            (this->lastRfidRead.readAt < this->imuChangedAt) &&
            (now > this->imuChangedAt + IMU_DELAY_BEFORE_READ) &&
            (now < this->imuChangedAt + IMU_DELAY_BEFORE_READ + RFID_READABLE_TIMEFRAME)
          )
        )
      );
    }
};

}