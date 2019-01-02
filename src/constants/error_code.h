#pragma once

namespace Climbalytics
{

enum ErrorCode {
  ErrorCode_None = 0x00,
  ErrorCode_MallocFailed = 0x01,
  ErrorCode_StackOverflow = 0x02,
  ErrorCode_TaskSchedulerNotRunning = 0x03,
  ErrorCode_PeripheralError = 0x04,
  ErrorCode_RfidInitializationError1 = 0x05,
  ErrorCode_RfidInitializationError2 = 0x06,
  ErrorCode_RfidReadTimeoutStart = 0x07,
  ErrorCode_RfidReadInvalidFrameStart = 0x08,
  ErrorCode_RfidReadTimeoutData = 0x09,
  ErrorCode_RfidReadTimeoutEnd = 0x0A,
  ErrorCode_RfidReadBadChecksum = 0x0B,
  ErrorCode_RfidReadInvalidFrameEnd = 0x0C,
  ErrorCode_RfidReadTooLong = 0x0D,
  ErrorCode_Imu_CTRL1_XL = 0x0E,
  ErrorCode_Imu_WAKE_UP_DUR = 0x0F,
  ErrorCode_Imu_WAKE_UP_THS = 0x10,
  ErrorCode_Imu_MD1_CFG = 0x11,
  ErrorCode_NotEnoughMemory = 0x12,
  ErrorCode_Altimeter_CTRL_REG1 = 0x17,
  ErrorCode_ActionDataTooLong = 0x18,
  ErrorCode_NoData = 0x19,
  ErrorCode_MemoryRead = 0x1A,
  ErrorCode_MemoryWrite = 0x1B,
  ErrorCode_RtcSetTime = 0x1C,
  ErrorCode_RtcGetTime = 0x1D,
  ErrorCode_NameTooLong = 0x1E,
  ErrorCode_RtcInit = 0x1F,
  ErrorCode_RfidCommandFailed = 0x20,
  ErrorCode_RfidInventoryTimeout = 0x21,
  ErrorCode_RfidUnknown = 0x22,
  ErrorCode_RfidInvalidReadFormat = 0x23,
  ErrorCode_RfidTooManyUniqueTags = 0x24,
  ErrorCode_RfidStopCommand = 0x25
};

}
