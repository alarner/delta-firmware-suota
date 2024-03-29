/**
 * \addtogroup BSP
 * \{
 * \addtogroup ADAPTERS
 * \{
 * \addtogroup ADAPTER_CONFIGURATION
 *
 * \brief Adapter related configurations
 *
 * \{
 */

/**
 ****************************************************************************************
 *
 * @file platform_devices.h
 *
 * @brief Configuration of devices connected to board
 *
 * Copyright (C) 2015 Dialog Semiconductor.
 * This computer program includes Confidential, Proprietary Information
 * of Dialog Semiconductor. All Rights Reserved.
 *
 ****************************************************************************************
 */

#ifndef PLATFORM_DEVICES_H_
#define PLATFORM_DEVICES_H_

#define CONFIG_SPI_ONE_DEVICE_ON_BUS 0

#define CONFIG_SPI_EXCLUSIVE_OPEN 0

#include <ad_spi.h>
#include <ad_i2c.h>
#include <ad_uart.h>
#include <ad_gpadc.h>
#include <ad_temp_sens.h>
#include <ad_battery.h>

#ifdef __cplusplus
extern "C" {
#endif

#if dg_configUART_ADAPTER

UART_BUS(UART1, SERIAL1, HW_UART_BAUDRATE_115200, HW_UART_DATABITS_8, HW_UART_PARITY_NONE,
         HW_UART_STOPBITS_1, 0, 0, HW_DMA_CHANNEL_1, HW_DMA_CHANNEL_0, 0, 0)

UART_BUS(UART2, RFID_UART, HW_UART_BAUDRATE_9600, HW_UART_DATABITS_8, HW_UART_PARITY_NONE,
        HW_UART_STOPBITS_1, 0, 1, HW_DMA_CHANNEL_3, HW_DMA_CHANNEL_2, 0, 0)

#endif /* dg_configUART_ADAPTER */


#if dg_configSPI_ADAPTER

/*
 * Define devices connected to SPI bus
 *
 * Following section describes devices connected to SPI.
 * Each SPI_SLAVE_DEVICE entry describes parameters that must be configured to access
 * device. Some of them can be found in data sheet of device (bus width, max speed allowed,
 * polarity and phase of the clock). Other parameters describe connection specifics (GPIO
 * port and pin number for chip select).
 */
SPI_BUS(SPI1)
#ifdef CONFIG_AT45DB011D
        SPI_SLAVE_DEVICE(SPI1, AT45DB011D, HW_GPIO_PORT_2, HW_GPIO_PIN_0, HW_SPI_WORD_8BIT,
                                HW_SPI_POL_LOW, HW_SPI_PHA_MODE_0, HW_SPI_FREQ_DIV_14, 0);
#endif
#ifdef CONFIG_SPI_SLAVE
        SPI_SLAVE_TO_EXT_MASTER(SPI1, SLAVE1, false, HW_SPI_WORD_8BIT, HW_SPI_POL_LOW,
                                                                        HW_SPI_PHA_MODE_0, -1);
#endif
SPI_BUS_END

SPI_BUS(SPI2)
#if CONFIG_SOME_SLAVE_DEVICE
        SPI_SLAVE_DEVICE(SPI2, SP1_SLAVE, HW_GPIO_PORT_3, HW_GPIO_PIN_1, HW_SPI_WORD_8BIT,
                                        HW_SPI_POL_LOW, HW_SPI_PHA_MODE_0, HW_SPI_FREQ_DIV_14, -1);
#endif
SPI_BUS_END

#endif /* dg_configSPI_ADAPTER */


#if dg_configI2C_ADAPTER

/*
 * Define devices connected to I2C bus
 *
 * Following section describes devices connected to I2C.
 * Each I2C_SLAVE_DEVICE entry describes parameters that must be configured to access
 * the device. Some of them can be found in data sheet of device (max speed allowed, address length
 * address or some of it bits). Device address can be also determined by some pins allowing to
 * connect several devices of same kind.
 */
I2C_BUS(I2C1)
#ifdef CONFIG_LSM6DS3
  /* always-on 3D accelerometer and 3D gyroscope */
  I2C_SLAVE_DEVICE_DMA(I2C1, LSM6DS3, 0x6A, HW_I2C_ADDRESSING_7B, HW_I2C_SPEED_STANDARD, 2);
#endif
#ifdef CONFIG_MPL3115A2
  /* precision pressure sensor with altimetry */
  I2C_SLAVE_DEVICE_DMA(I2C1, MPL3115A2, 0x60, HW_I2C_ADDRESSING_7B, HW_I2C_SPEED_STANDARD, 2);
#endif
#ifdef CONFIG_PCF85263A
  /* Tiny Real-Time Clock/calendar */
  I2C_SLAVE_DEVICE_DMA(I2C1, PCF85263A, 0x51, HW_I2C_ADDRESSING_7B, HW_I2C_SPEED_STANDARD, 2);
#endif
I2C_BUS_END

I2C_BUS(I2C2)
I2C_BUS_END

#endif /* dg_configI2C_ADAPTER */


#if dg_configGPADC_ADAPTER

/*
 * Define sources connected to GPADC
 */

GPADC_SOURCE(TEMP_SENSOR, HW_GPADC_CLOCK_INTERNAL, HW_GPADC_INPUT_MODE_SINGLE_ENDED,
                                HW_GPADC_INPUT_SE_TEMPSENS, 5, false, HW_GPADC_OVERSAMPLING_1_SAMPLE,
                                                                HW_GPADC_INPUT_VOLTAGE_UP_TO_1V2)
#if dg_configUSE_SOC
GPADC_SOURCE(BATTERY_LEVEL, HW_GPADC_CLOCK_INTERNAL, HW_GPADC_INPUT_MODE_SINGLE_ENDED,
                                HW_GPADC_INPUT_SE_VBAT, 15, true, HW_GPADC_OVERSAMPLING_16_SAMPLES,
                                HW_GPADC_INPUT_VOLTAGE_UP_TO_1V2)
#else
GPADC_SOURCE(BATTERY_LEVEL, HW_GPADC_CLOCK_INTERNAL, HW_GPADC_INPUT_MODE_SINGLE_ENDED,
                                HW_GPADC_INPUT_SE_VBAT, 15, true, HW_GPADC_OVERSAMPLING_4_SAMPLES,
                                HW_GPADC_INPUT_VOLTAGE_UP_TO_1V2)
#endif

#endif /* dg_configGPADC_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_DEVICES_H_ */

/**
 * \}
 * \}
 * \}
 */
