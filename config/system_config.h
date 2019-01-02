#ifndef _CUSTOM_CONFIG_COMMON_H_
#define _CUSTOM_CONFIG_COMMON_H_

#include "bsp_definitions.h"


#define BLE_MAX_DELAYS_ALLOWED                  (3)
#define BLE_MAX_MISSES_ALLOWED                  (3)
#define BLE_MGR_USE_EVT_LIST                    (1)
#define CONFIG_LSM6DS3               (1)
#define CONFIG_MPL3115A2             (1)
#define CONFIG_PCF85263A             (1)
#define CONFIG_RETARGET
#define CONFIG_RETARGET_UART HW_UART1
#define CONFIG_USE_BLE
#define configTOTAL_HEAP_SIZE   (11100 + SUOTA_HEAP_OVERHEAD)
#define defaultBLE_ATT_DB_CONFIGURATION         (0x10)  // with "Peripheral Preferred Connection Parameters"
#define defaultBLE_PPCP_INTERVAL_MAX            (BLE_CONN_INTERVAL_FROM_MS(750))    // 750 ms
#define defaultBLE_PPCP_INTERVAL_MIN            (BLE_CONN_INTERVAL_FROM_MS(500))    // 500 ms
#define defaultBLE_PPCP_SLAVE_LATENCY           (0)                                 // 0 events
#define defaultBLE_PPCP_SUP_TIMEOUT             (BLE_SUPERVISION_TMO_FROM_MS(6000)) // 6000 ms
#define dg_configALLOW_CHARGING_NOT_ENUM        (1)
#define dg_configBATTERY_ADAPTER        (1)
#define dg_configBATTERY_CHARGE_CURRENT           (8)  // 180mA (see lib/sdk/bsp/system/sys_man/sys_charger.c:115)
#define dg_configBATTERY_CHARGE_NTC               (1)    // NTC Protection is Disabled
#define dg_configBATTERY_CHARGE_VOLTAGE           (0xA)  // 4.2V - voltage at which to apply toward batter when charging
#define dg_configBATTERY_LOW_LEVEL                (2496) // 3.05V - used to know when battery is low
#define dg_configBATTERY_PRECHARGE_CURRENT        (3)  // 45mA
#define dg_configBATTERY_REPLENISH_GAP              (8) // 0.01V
#define dg_configBATTERY_TYPE                     (BATTERY_TYPE_CUSTOM) // Lithium Ion Polymer
#define dg_configBATTERY_TYPE_CUSTOM_ADC_VOLTAGE  (3440) // 4.2V - used to compare current Vbat level when charging
#define dg_configBLE_BROADCASTER                (0)
#define dg_configBLE_CENTRAL                    (0)
#define dg_configBLE_GATT_CLIENT                (0)
#define dg_configBLE_OBSERVER                   (0)
#define dg_configCACHEABLE_QSPI_AREA_LEN        (NVMS_PARAM_PART_start - MEMORY_QSPIF_BASE)
#define dg_configCHARGING_CC_TIMEOUT   (360 * 60 * 100)
#define dg_configCHARGING_CV_TIMEOUT   (720 * 60 * 100)
#define dg_configCHARGING_THRESHOLD                 (2498) // 3.05V - transitions from precharge to charge once above this value
#define dg_configCODE_LOCATION                  NON_VOLATILE_IS_FLASH
#define dg_configEMULATE_OTP_COPY               (0)
#define dg_configEXEC_MODE                      MODE_IS_CACHED
#define dg_configEXT_CRYSTAL_FREQ               EXT_CRYSTAL_IS_16M
#define dg_configFLASH_ADAPTER                  1
#define dg_configFLASH_CONNECTED_TO             (FLASH_CONNECTED_TO_1V8)
#define dg_configFLASH_POWER_DOWN               (0)
#define dg_configGPADC_ADAPTER        (1)
#define dg_configI2C_ADAPTER                                    (1)
// #define dg_configIMAGE_FLASH_OFFSET (0x20000)
#define dg_configIMAGE_SETUP                    DEVELOPMENT_MODE
#define dg_configMEM_RETENTION_MODE             (0x1B)
#define dg_configNVMS_ADAPTER                   1
#define dg_configNVMS_VES                       1
#define dg_configNVPARAM_ADAPTER                1
#define dg_configOPTIMAL_RETRAM                 (1)
#define dg_configPOWER_1V8_ACTIVE               (1)
#define dg_configPOWER_1V8_SLEEP                (1)
#define dg_configPOWER_1V8P (1) 
#define dg_configPRECHARGING_THRESHOLD              (2462) // 3.006V - starts precharge if less than this value
#define dg_configPRECHARGING_TIMEOUT                (60  * 60 * 100) // 1 hr (Unit: 0.01s)
#define dg_configQSPI_CACHED_OPTIMAL_RETRAM_0_SIZE_AE   ( 64 * 1024)
#define dg_configQSPI_CACHED_RAM_SIZE_AE                ( 32 * 1024)
#define dg_configQSPI_CACHED_RETRAM_0_SIZE_AE           ( 96 * 1024)
#define dg_configSHUFFLING_MODE                 (0x0)
#define dg_configSUOTA_SUPPORT                  (1)
#define dg_configTESTMODE_MEASURE_SLEEP_CURRENT (0)
#define dg_configTRACK_OS_HEAP (1)
#define dg_configUART_ADAPTER        (1)
#define dg_configUSB_CHARGER_POLLING_INTERVAL       (60 * 100) // 1 min (Unit: 0.01s)
#define dg_configUSE_HW_GPADC        (1)
#define dg_configUSE_HW_GPIO         (1)
#define dg_configUSE_HW_I2C          (1)
#define dg_configUSE_HW_TEMPSENS        (1)
#define dg_configUSE_HW_TIMER2       (1)
#define dg_configUSE_HW_UART        (1)
#define dg_configUSE_LP_CLK                     LP_CLK_RCX
#define dg_configUSE_NOT_ENUM_CHARGING_TIMEOUT  (0)
#define dg_configUSE_SOC                        1
#define dg_configUSE_SW_CURSOR                  (1)
#define dg_configUSE_USB_CHARGER                (1)
#define dg_configUSE_USB_ENUMERATION            (1)
#define dg_configUSE_WDOG                       (1)
#define dg_configUSER_CAN_USE_TIMER1            (0)
#define OS_FREERTOS                              /* Define this to use FreeRTOS */
#define SUOTA_HEAP_OVERHEAD     (3200)
#define SUOTA_PSM       0x81
#define SUOTA_VERSION   SUOTA_VERSION_1_3
#define USE_PARTITION_TABLE_1MB_WITH_SUOTA
#undef CONFIG_USE_FTDF


/* Include bsp default values */
#include "bsp_defaults.h"
/* Include memory layout */
#include "bsp_memory_layout.h"


#endif
