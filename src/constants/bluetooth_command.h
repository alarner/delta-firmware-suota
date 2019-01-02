#pragma once

namespace Climbalytics
{

enum BluetoothCommand {
  BluetoothCommand_SetFlowControl = 0x01,
  BluetoothCommand_GetDataLength = 0x02,
  BluetoothCommand_ReadCurrentBlock = 0x03,
  BluetoothCommand_AdvanceBlock = 0x04,
  BluetoothCommand_SetTime = 0x05,
  BluetoothCommand_ReadSingleTag = 0x06,
  BluetoothCommand_SetName = 0x07,
  BluetoothCommand_GetBatteryLevel = 0x08,
  BluetoothCommand_FactoryReset = 0x09,
  BluetoothCommand_SetLogStatus = 0x0A
};

}
