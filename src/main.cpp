/**
 ****************************************************************************************
 *
 * @file main.c
 *
 * @brief FreeRTOS template application with retarget
 *
 * Copyright (C) 2015. Dialog Semiconductor, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <black.orca.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

extern "C" {
  #include "ad_i2c.h"
  #include "ad_nvms.h"
  #include "ad_uart.h"
  #include "ble_mgr.h"
  #include "hw_cpm.h"
  #include "hw_gpio.h"
  #include "platform_devices.h"
  #include "resmgmt.h"
  #include "sys_clock_mgr.h"
  #include "sys_power_mgr.h"
  #include "sys_watchdog.h"
}

#include "pin_port_mapping.h"

#include "bluetooth_task.h"

#if (dg_configTRACK_OS_HEAP == 1)
/*
 * ConstantsVariables used for Tasks Stack and OS Heap tracking
 * Declared global to avoid IDLE stack Overflows
 */
#define mainMAX_NB_OF_TASKS           10
#define mainMIN_STACK_GUARD_SIZE      8 /* words */
#define mainTOTAL_HEAP_SIZE_GUARD     64 /*bytes */

TaskStatus_t pxTaskStatusArray[mainMAX_NB_OF_TASKS];
uint32_t ulTotalRunTime;
#endif /* (dg_configTRACK_OS_HEAP == 1) */

#if defined CONFIG_RETARGET
extern "C" void retarget_init(void);
#endif

#if dg_configUSE_WDOG
INITIALISED_PRIVILEGED_DATA int8_t idle_task_wdog_id = -1;
#endif

/**
 * @brief Initialize the peripherals domain after power-up.
 */
static void peripheralInitialization(void)
{
  hw_gpio_set_pin_function(HAPTIC_PORT, HAPTIC_PIN, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_GPIO);

  hw_gpio_set_pin_function(UART_TX_PORT, UART_TX_PIN, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_UART_TX);
  hw_gpio_set_pin_function(UART_RX_PORT, UART_RX_PIN, HW_GPIO_MODE_INPUT, HW_GPIO_FUNC_UART_RX);

  hw_gpio_set_pin_function(I2C1_SDA_PORT, I2C1_SDA_PIN, HW_GPIO_MODE_INPUT, HW_GPIO_FUNC_I2C_SDA);
  hw_gpio_set_pin_function(I2C1_SCL_PORT, I2C1_SCL_PIN, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_I2C_SCL);

  hw_gpio_set_pin_function(RFID_UART_TX_PORT, RFID_UART_TX_PIN, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_UART2_TX);
  hw_gpio_set_pin_function(RFID_UART_RX_PORT, RFID_UART_RX_PIN, HW_GPIO_MODE_INPUT, HW_GPIO_FUNC_UART2_RX);

  hw_gpio_set_pin_function(BUTTON_PORT, BUTTON_PIN, HW_GPIO_MODE_INPUT_PULLUP, HW_GPIO_FUNC_GPIO);

  hw_gpio_set_pin_function(RFID_VPD_PORT, RFID_VPD_PIN, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_GPIO);
  hw_gpio_set_pin_function(RFID_ENABLE_PORT, RFID_ENABLE_PIN, HW_GPIO_MODE_OUTPUT, HW_GPIO_FUNC_GPIO);

  hw_gpio_set_active(RFID_VPD_PORT, RFID_VPD_PIN);
  hw_gpio_set_active(RFID_ENABLE_PORT, RFID_ENABLE_PIN);
}

/**
 * @brief System Initialization Task
 */
static void systemInitialization(void *taskParameters)
{
  /**
   * Prepare clocks. Note: cm_cpu_clk_set() and cm_sys_clk_set() can be called
   * only from a task since they will suspend the task until the XTAL16M has
   * settled and, maybe, the PLL is locked.
   */
  cm_sys_clk_init(sysclk_XTAL16M);
  cm_apb_set_clock_divider(apb_div1);
  cm_ahb_set_clock_divider(ahb_div1);
  cm_lp_clk_init();


#if dg_configUSE_WDOG
  sys_watchdog_init();

  // Register the Idle task first.
  idle_task_wdog_id = sys_watchdog_register(false);
  ASSERT_WARNING(idle_task_wdog_id != -1);
  sys_watchdog_configure_idle_id(idle_task_wdog_id);
#endif

  /* Prepare the hardware to run this demo. */
  pm_system_init(peripheralInitialization);

  /* init resources */
  resource_init();

#if defined CONFIG_RETARGET
  retarget_init();
#endif

  /* Set the desired sleep mode. */
  pm_set_wakeup_mode(true);
  pm_set_sleep_mode(pm_mode_idle);

  printf("Initialize adapters: begin\n");
  ad_nvms_init();
  ad_uart_init();
  ad_i2c_init();
  ble_mgr_init();
  printf("Initialize adapters: complete\n\n");

  printf("Create bluetooth task: begin\n");
  OS_TASK handle;
  OS_TASK_CREATE(
    "Bluetooth Task", /* The text name assigned to the task, for debug only; not used by the kernel. */
    bleConnectionTask,           /* The function that implements the task. */
    NULL,    /* The parameter passed to the task. */
    250 * OS_STACK_WORD_SIZE,    /* The number of bytes to allocate to the stack of the task. */
    (OS_TASK_PRIORITY_NORMAL + 2),     /* The priority assigned to the task. */
    handle      /* The task handle */
  );
  OS_ASSERT(handle);
  printf("Create bluetooth task: complete\n\n");


  OS_TASK_DELETE(OS_GET_CURRENT_TASK());
}

/**
 * @brief Template main creates a System Initialization task, which initializes
 * peripherals and creates all subtasks that operate the device.
 */
int main(void)
{
  OS_TASK systemInitializationHandle;
  OS_BASE_TYPE status;

  cm_clk_init_low_level(); /* Basic clock initializations. */

  /* Start the two tasks as described in the comments at the top of this file. */
  status = OS_TASK_CREATE(
    "System Initialization",   /* The text name assigned to the task, for debug only; not used by the kernel. */
    systemInitialization,      /* The System Initialization task. */
    NULL,                      /* The parameter passed to the task. */
    250 * OS_STACK_WORD_SIZE, /* The number of bytes to allocate to the stack of the task. */
    OS_TASK_PRIORITY_HIGHEST,  /* The priority assigned to the task. */
    systemInitializationHandle /* The task handle */
  );
  OS_ASSERT(status == OS_TASK_CREATE_SUCCESS);

  /* Start the tasks and timer running. */
  vTaskStartScheduler();

  /**
   * If all is well, the scheduler will now be running, and the following line
   * will never be reached.  If the following line does execute, then there was
   * insufficient FreeRTOS heap memory available for the idle and/or timer
   * tasks  to be created.  See the memory management section on the FreeRTOS
   * web site for more details.
   */
  printf("Something went wrong...\n");
  for (;;);
}

/**
 * @brief Malloc fail hook
 */
extern "C" void vApplicationMallocFailedHook( void )
{
        /* vApplicationMallocFailedHook() will only be called if
  configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
  function that will get called if a call to pvPortMalloc() fails.
  pvPortMalloc() is called internally by the kernel whenever a task, queue,
  timer or semaphore is created.  It is also called by various parts of the
  demo application.  If heap_1.c or heap_2.c are used, then the size of the
  heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
  FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
  to query the size of free heap space that remains (although it does not
  provide information on how the remaining heap might be fragmented). */
        taskDISABLE_INTERRUPTS();
        for( ;; );
}

/**
 * @brief Application idle task hook
 */
extern "C" void vApplicationIdleHook( void )
{
        /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
           to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
           task. It is essential that code added to this hook function never attempts
           to block in any way (for example, call xQueueReceive() with a block time
           specified, or call vTaskDelay()).  If the application makes use of the
           vTaskDelete() API function (as this demo application does) then it is also
           important that vApplicationIdleHook() is permitted to return to its calling
           function, because it is the responsibility of the idle task to clean up
           memory allocated by the kernel to any task that has since been deleted. */

#if (dg_configTRACK_OS_HEAP == 1)
        OS_BASE_TYPE i = 0;
        OS_BASE_TYPE uxMinimumEverFreeHeapSize;

        // Generate raw status information about each task.
        UBaseType_t uxNbOfTaskEntries = uxTaskGetSystemState(pxTaskStatusArray,
                                                        mainMAX_NB_OF_TASKS, &ulTotalRunTime);

        for (i = 0; i < uxNbOfTaskEntries; i++) {
                /* Check Free Stack*/
                OS_BASE_TYPE uxStackHighWaterMark;

                uxStackHighWaterMark = uxTaskGetStackHighWaterMark(pxTaskStatusArray[i].xHandle);
                OS_ASSERT(uxStackHighWaterMark >= mainMIN_STACK_GUARD_SIZE);
        }

        /* Check Minimum Ever Free Heap against defined guard. */
        uxMinimumEverFreeHeapSize = xPortGetMinimumEverFreeHeapSize();
        OS_ASSERT(uxMinimumEverFreeHeapSize >= mainTOTAL_HEAP_SIZE_GUARD);
#endif /* (dg_configTRACK_OS_HEAP == 1) */

#if dg_configUSE_WDOG
        sys_watchdog_notify(idle_task_wdog_id);
#endif
}

/**
 * @brief Application stack overflow hook
 */
extern "C" void vApplicationStackOverflowHook( OS_TASK pxTask, char *pcTaskName )
{
        ( void ) pcTaskName;
        ( void ) pxTask;

        /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
  function is called if a stack overflow is detected. */
        taskDISABLE_INTERRUPTS();
        for( ;; );
}

/**
 * @brief Application tick hook
 */
extern "C" void vApplicationTickHook( void )
{

        OS_POISON_AREA_CHECK( OS_POISON_ON_ERROR_HALT, result );

}
