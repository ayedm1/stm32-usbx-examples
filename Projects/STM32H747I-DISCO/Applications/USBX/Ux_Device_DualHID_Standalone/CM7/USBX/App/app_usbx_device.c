/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_usbx_device.c
  * @author  MCD Application Team
  * @brief   USBX Device applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_usbx_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN UX_Device_Memory_Buffer */

/* USER CODE END UX_Device_Memory_Buffer */
#if defined ( __ICCARM__ )
#pragma data_alignment=4
#endif
__ALIGN_BEGIN static UCHAR ux_device_memory_buffer[USBX_DEVICE_APP_MEMORY_BUFFER_SIZE] __ALIGN_END;

static ULONG hid_mouse_interface_number;
static ULONG hid_mouse_configuration_number;
static ULONG hid_keyboard_interface_number;
static ULONG hid_keyboard_configuration_number;
static UX_SLAVE_CLASS_HID_PARAMETER hid_mouse_parameter;
static UX_SLAVE_CLASS_HID_PARAMETER hid_keyboard_parameter;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application USBX Device Initialization.
  * @param  none
  * @retval status
  */
UINT MX_USBX_Device_Init(VOID)
{
  UINT ret = UX_SUCCESS;
  UCHAR *device_framework_high_speed;
  UCHAR *device_framework_full_speed;
  ULONG device_framework_hs_length;
  ULONG device_framework_fs_length;
  ULONG string_framework_length;
  ULONG language_id_framework_length;
  UCHAR *string_framework;
  UCHAR *language_id_framework;
  UCHAR *pointer;

  /* USER CODE BEGIN MX_USBX_Device_Init0 */

  /* USER CODE END MX_USBX_Device_Init0 */

  pointer = ux_device_memory_buffer;

  /* Initialize USBX Memory */
  if (ux_system_initialize(pointer, USBX_DEVICE_MEMORY_STACK_SIZE, UX_NULL, 0) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_SYSTEM_INITIALIZE_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_SYSTEM_INITIALIZE_ERROR */
  }

  /* Get Device Framework High Speed and get the length */
  device_framework_high_speed = USBD_Get_Device_Framework_Speed(USBD_HIGH_SPEED,
                                                                &device_framework_hs_length);

  /* Get Device Framework Full Speed and get the length */
  device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED,
                                                                &device_framework_fs_length);

  /* Get String Framework and get the length */
  string_framework = USBD_Get_String_Framework(&string_framework_length);

  /* Get Language Id Framework and get the length */
  language_id_framework = USBD_Get_Language_Id_Framework(&language_id_framework_length);

  /* Install the device portion of USBX */
  if (ux_device_stack_initialize(device_framework_high_speed,
                                 device_framework_hs_length,
                                 device_framework_full_speed,
                                 device_framework_fs_length,
                                 string_framework,
                                 string_framework_length,
                                 language_id_framework,
                                 language_id_framework_length,
                                 UX_NULL) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_DEVICE_INITIALIZE_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_DEVICE_INITIALIZE_ERROR */
  }

  /* Initialize the hid mouse class parameters for the device */
  hid_mouse_parameter.ux_slave_class_hid_instance_activate         = USBD_HID_Mouse_Activate;
  hid_mouse_parameter.ux_slave_class_hid_instance_deactivate       = USBD_HID_Mouse_Deactivate;
  hid_mouse_parameter.ux_device_class_hid_parameter_report_address = USBD_HID_ReportDesc(INTERFACE_HID_MOUSE);
  hid_mouse_parameter.ux_device_class_hid_parameter_report_length  = USBD_HID_ReportDesc_length(INTERFACE_HID_MOUSE);
  hid_mouse_parameter.ux_device_class_hid_parameter_report_id      = UX_FALSE;
  hid_mouse_parameter.ux_device_class_hid_parameter_callback       = USBD_HID_Mouse_SetReport;
  hid_mouse_parameter.ux_device_class_hid_parameter_get_callback   = USBD_HID_Mouse_GetReport;

  /* USER CODE BEGIN HID_MOUSE_PARAMETER */

  /* USER CODE END HID_MOUSE_PARAMETER */

  /* Get hid mouse configuration number */
  hid_mouse_configuration_number = USBD_Get_Configuration_Number(CLASS_TYPE_HID, INTERFACE_HID_MOUSE);

  /* Find hid mouse interface number */
  hid_mouse_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_HID, INTERFACE_HID_MOUSE);

  /* Initialize the device hid Mouse class */
  if (ux_device_stack_class_register(_ux_system_slave_class_hid_name,
                                     ux_device_class_hid_entry,
                                     hid_mouse_configuration_number,
                                     hid_mouse_interface_number,
                                     &hid_mouse_parameter) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_DEVICE_HID_MOUSE_REGISTER_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_DEVICE_HID_MOUSE_REGISTER_ERROR */
  }

  /* Initialize the hid keyboard class parameters for the device */
  hid_keyboard_parameter.ux_slave_class_hid_instance_activate         = USBD_HID_Keyboard_Activate;
  hid_keyboard_parameter.ux_slave_class_hid_instance_deactivate       = USBD_HID_Keyboard_Deactivate;
  hid_keyboard_parameter.ux_device_class_hid_parameter_report_address = USBD_HID_ReportDesc(INTERFACE_HID_KEYBOARD);
  hid_keyboard_parameter.ux_device_class_hid_parameter_report_length  = USBD_HID_ReportDesc_length(INTERFACE_HID_KEYBOARD);
  hid_keyboard_parameter.ux_device_class_hid_parameter_report_id      = UX_FALSE;
  hid_keyboard_parameter.ux_device_class_hid_parameter_callback       = USBD_HID_Keyboard_SetReport;
  hid_keyboard_parameter.ux_device_class_hid_parameter_get_callback   = USBD_HID_Keyboard_GetReport;

  /* USER CODE BEGIN HID_KEYBOARD_PARAMETER */

  /* USER CODE END HID_KEYBOARD_PARAMETER */

  /* Get hid keyboard configuration number */
  hid_keyboard_configuration_number = USBD_Get_Configuration_Number(CLASS_TYPE_HID, INTERFACE_HID_KEYBOARD);

  /* Find hid keyboard interface number */
  hid_keyboard_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_HID, INTERFACE_HID_KEYBOARD);

  /* Initialize the device hid keyboard class */
  if (ux_device_stack_class_register(_ux_system_slave_class_hid_name,
                                     ux_device_class_hid_entry,
                                     hid_keyboard_configuration_number,
                                     hid_keyboard_interface_number,
                                     &hid_keyboard_parameter) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_DEVICE_HID_KEYBOARD_REGISTER_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_DEVICE_HID_KEYBOARD_REGISTER_ERROR */
  }

  /* USER CODE BEGIN MX_USBX_Device_Init1 */

  /* USER CODE END MX_USBX_Device_Init1 */

  return ret;
}

/**
  * @brief  _ux_utility_interrupt_disable
  *         USB utility interrupt disable.
  * @param  none
  * @retval none
  */
ALIGN_TYPE _ux_utility_interrupt_disable(VOID)
{
  UINT interrupt_save;

  /* USER CODE BEGIN _ux_utility_interrupt_disable */
  interrupt_save = __get_PRIMASK();
  __disable_irq();
  /* USER CODE END _ux_utility_interrupt_disable */

  return interrupt_save;
}

/**
  * @brief  _ux_utility_interrupt_restore
  *         USB utility interrupt restore.
  * @param  flags
  * @retval none
  */
VOID _ux_utility_interrupt_restore(ALIGN_TYPE flags)
{
  /* USER CODE BEGIN _ux_utility_interrupt_restore */
  __set_PRIMASK(flags);
  /* USER CODE END _ux_utility_interrupt_restore */
}

/**
  * @brief  _ux_utility_time_get
  *         Get Time Tick for host timing.
  * @param  none
  * @retval time tick
  */
ULONG _ux_utility_time_get(VOID)
{
  ULONG time_tick = 0U;

  /* USER CODE BEGIN _ux_utility_time_get */

  /* USER CODE END _ux_utility_time_get */

  return time_tick;
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
