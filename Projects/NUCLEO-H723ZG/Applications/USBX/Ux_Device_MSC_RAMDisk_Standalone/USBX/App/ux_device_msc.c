/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_msc.c
  * @author  MCD Application Team
  * @brief   USBX Device MSC applicative source file
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
#include "ux_device_msc.h"

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
/* USER CODE BEGIN PV */
extern UCHAR ram_disk_memory[RAM_DISK_SIZE];

static UCHAR sector_0[] = {
  0xEB, 0x3C, 0x90, 0x4D, 0x53, 0x44, 0x4F, 0x53, 0x35, 0x2E, 0x30, 0x00,
  0x02, 0x01, 0x06, 0x00, 0x02, 0x00, 0x02,
  /*0x13: small sectors*/0xF0, 0x00,
  0xF8, 0x01, 0x00,
  0x01, 0x00, 0x01,
  /*0x20: large sectors*/0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x00, 0x29,
  /*0x27: serial number*/0x7E, 0x0D, 0x2E, 0xF0,
  0x4E, 0x4F, 0x20, 0x4E, 0x41,
  0x4D, 0x45, 0x20, 0x20, 0x20, 0x20, 0x46, 0x41, 0x54, 0x31, 0x32, 0x20,
  0x20, 0x20, 0x33, 0xC9, 0x8E, 0xD1, 0xBC, 0xF0, 0x7B, 0x8E, 0xD9, 0xB8,
  0x00, 0x20, 0x8E, 0xC0, 0xFC, 0xBD, 0x00, 0x7C, 0x38, 0x4E, 0x24, 0x7D,
  0x24, 0x8B, 0xC1, 0x99, 0xE8, 0x3C, 0x01, 0x72, 0x1C, 0x83, 0xEB, 0x3A,
  0x66, 0xA1, 0x1C, 0x7C, 0x26, 0x66, 0x3B, 0x07, 0x26, 0x8A, 0x57, 0xFC,
  0x75, 0x06, 0x80, 0xCA, 0x02, 0x88, 0x56, 0x02, 0x80, 0xC3, 0x10, 0x73,
  0xEB, 0x33, 0xC9, 0x8A, 0x46, 0x10, 0x98, 0xF7, 0x66, 0x16, 0x03, 0x46,
  0x1C, 0x13, 0x56, 0x1E, 0x03, 0x46, 0x0E, 0x13, 0xD1, 0x8B, 0x76, 0x11,
  0x60, 0x89, 0x46, 0xFC, 0x89, 0x56, 0xFE, 0xB8, 0x20, 0x00, 0xF7, 0xE6,
  0x8B, 0x5E, 0x0B, 0x03, 0xC3, 0x48, 0xF7, 0xF3, 0x01, 0x46, 0xFC, 0x11,
  0x4E, 0xFE, 0x61, 0xBF, 0x00, 0x00, 0xE8, 0xE6, 0x00, 0x72, 0x39, 0x26,
  0x38, 0x2D, 0x74, 0x17, 0x60, 0xB1, 0x0B, 0xBE, 0xA1, 0x7D, 0xF3, 0xA6,
  0x61, 0x74, 0x32, 0x4E, 0x74, 0x09, 0x83, 0xC7, 0x20, 0x3B, 0xFB, 0x72,
  0xE6, 0xEB, 0xDC, 0xA0, 0xFB, 0x7D, 0xB4, 0x7D, 0x8B, 0xF0, 0xAC, 0x98,
  0x40, 0x74, 0x0C, 0x48, 0x74, 0x13, 0xB4, 0x0E, 0xBB, 0x07, 0x00, 0xCD,
  0x10, 0xEB, 0xEF, 0xA0, 0xFD, 0x7D, 0xEB, 0xE6, 0xA0, 0xFC, 0x7D, 0xEB,
  0xE1, 0xCD, 0x16, 0xCD, 0x19, 0x26, 0x8B, 0x55, 0x1A, 0x52, 0xB0, 0x01,
  0xBB, 0x00, 0x00, 0xE8, 0x3B, 0x00, 0x72, 0xE8, 0x5B, 0x8A, 0x56, 0x24,
  0xBE, 0x0B, 0x7C, 0x8B, 0xFC, 0xC7, 0x46, 0xF0, 0x3D, 0x7D, 0xC7, 0x46,
  0xF4, 0x29, 0x7D, 0x8C, 0xD9, 0x89, 0x4E, 0xF2, 0x89, 0x4E, 0xF6, 0xC6,
  0x06, 0x96, 0x7D, 0xCB, 0xEA, 0x03, 0x00, 0x00, 0x20, 0x0F, 0xB6, 0xC8,
  0x66, 0x8B, 0x46, 0xF8, 0x66, 0x03, 0x46, 0x1C, 0x66, 0x8B, 0xD0, 0x66,
  0xC1, 0xEA, 0x10, 0xEB, 0x5E, 0x0F, 0xB6, 0xC8, 0x4A, 0x4A, 0x8A, 0x46,
  0x0D, 0x32, 0xE4, 0xF7, 0xE2, 0x03, 0x46, 0xFC, 0x13, 0x56, 0xFE, 0xEB,
  0x4A, 0x52, 0x50, 0x06, 0x53, 0x6A, 0x01, 0x6A, 0x10, 0x91, 0x8B, 0x46,
  0x18, 0x96, 0x92, 0x33, 0xD2, 0xF7, 0xF6, 0x91, 0xF7, 0xF6, 0x42, 0x87,
  0xCA, 0xF7, 0x76, 0x1A, 0x8A, 0xF2, 0x8A, 0xE8, 0xC0, 0xCC, 0x02, 0x0A,
  0xCC, 0xB8, 0x01, 0x02, 0x80, 0x7E, 0x02, 0x0E, 0x75, 0x04, 0xB4, 0x42,
  0x8B, 0xF4, 0x8A, 0x56, 0x24, 0xCD, 0x13, 0x61, 0x61, 0x72, 0x0B, 0x40,
  0x75, 0x01, 0x42, 0x03, 0x5E, 0x0B, 0x49, 0x75, 0x06, 0xF8, 0xC3, 0x41,
  0xBB, 0x00, 0x00, 0x60, 0x66, 0x6A, 0x00, 0xEB, 0xB0, 0x42, 0x4F, 0x4F,
  0x54, 0x4D, 0x47, 0x52, 0x20, 0x20, 0x20, 0x20, 0x0D, 0x0A, 0x52, 0x65,
  0x6D, 0x6F, 0x76, 0x65, 0x20, 0x64, 0x69, 0x73, 0x6B, 0x73, 0x20, 0x6F,
  0x72, 0x20, 0x6F, 0x74, 0x68, 0x65, 0x72, 0x20, 0x6D, 0x65, 0x64, 0x69,
  0x61, 0x2E, 0xFF, 0x0D, 0x0A, 0x44, 0x69, 0x73, 0x6B, 0x20, 0x65, 0x72,
  0x72, 0x6F, 0x72, 0xFF, 0x0D, 0x0A, 0x50, 0x72, 0x65, 0x73, 0x73, 0x20,
  0x61, 0x6E, 0x79, 0x20, 0x6B, 0x65, 0x79, 0x20, 0x74, 0x6F, 0x20, 0x72,
  0x65, 0x73, 0x74, 0x61, 0x72, 0x74, 0x0D, 0x0A, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xAC, 0xCB, 0xD8, 0x55, 0xAA
};

static UCHAR sector_6_7[] = {
  0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F,
};

static const UCHAR sector_8[] = {
  0x53, 0x54, 0x4D, 0x33, 0x32, 0x20, 0x52, 0x41, 0x4D, 0x20, 0x20, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x92,
  0x81, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x20, 0x00, 0x49,
  0x00, 0x6E, 0x00, 0x66, 0x00, 0x6F, 0x00, 0x0F, 0x00, 0x72, 0x72, 0x00,
  0x6D, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x00, 0x00,
  0x6E, 0x00, 0x00, 0x00, 0x01, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74,
  0x00, 0x65, 0x00, 0x0F, 0x00, 0x72, 0x6D, 0x00, 0x20, 0x00, 0x56, 0x00,
  0x6F, 0x00, 0x6C, 0x00, 0x75, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x65, 0x00,
  0x53, 0x59, 0x53, 0x54, 0x45, 0x4D, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x16,
  0x00, 0xB5, 0x25, 0x92, 0x81, 0x52, 0x81, 0x52, 0x00, 0x00, 0x26, 0x92,
  0x81, 0x52, 0x02,
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  USBD_STORAGE_Activate
  *         This function is called when insertion of a storage device.
  * @param  storage_instance: Pointer to the storage class instance.
  * @retval none
  */
VOID USBD_STORAGE_Activate(VOID *storage_instance)
{
  /* USER CODE BEGIN USBD_STORAGE_Activate */
  UX_PARAMETER_NOT_USED(storage_instance);
  /* USER CODE END USBD_STORAGE_Activate */

  return;
}

/**
  * @brief  USBD_STORAGE_Deactivate
  *         This function is called when extraction of a storage device.
  * @param  storage_instance: Pointer to the storage class instance.
  * @retval none
  */
VOID USBD_STORAGE_Deactivate(VOID *storage_instance)
{
  /* USER CODE BEGIN USBD_STORAGE_Desactivate */
  UX_PARAMETER_NOT_USED(storage_instance);
  /* USER CODE END USBD_STORAGE_Desactivate */

  return;
}

/**
  * @brief  USBD_STORAGE_Read
  *         This function is invoked to read from media.
  * @param  storage_instance : Pointer to the storage class instance.
  * @param  lun: Logical unit number is the command is directed to.
  * @param  data_pointer: Address of the buffer to be used for reading or writing.
  * @param  number_blocks: number of sectors to read/write.
  * @param  lba: Logical block address is the sector address to read.
  * @param  media_status: should be filled out exactly like the media status
  *                       callback return value.
  * @retval status
  */
UINT USBD_STORAGE_Read(VOID *storage_instance, ULONG lun, UCHAR *data_pointer,
                       ULONG number_blocks, ULONG lba, ULONG *media_status)
{
  UINT status = UX_SUCCESS;

  /* USER CODE BEGIN USBD_STORAGE_Read */
  UX_PARAMETER_NOT_USED(storage_instance);
  UX_PARAMETER_NOT_USED(lun);
  UX_PARAMETER_NOT_USED(media_status);


  ux_utility_memory_copy(data_pointer, ram_disk_memory + lba * 512,
                         number_blocks * 512);

  status = UX_STATE_NEXT;

  /* USER CODE END USBD_STORAGE_Read */

  return status;
}

/**
  * @brief  USBD_STORAGE_Write
  *         This function is invoked to write in media.
  * @param  storage_instance : Pointer to the storage class instance.
  * @param  lun: Logical unit number is the command is directed to.
  * @param  data_pointer: Address of the buffer to be used for reading or writing.
  * @param  number_blocks: number of sectors to read/write.
  * @param  lba: Logical block address is the sector address to read.
  * @param  media_status: should be filled out exactly like the media status
  *                       callback return value.
  * @retval status
  */
UINT USBD_STORAGE_Write(VOID *storage_instance, ULONG lun, UCHAR *data_pointer,
                        ULONG number_blocks, ULONG lba, ULONG *media_status)
{
  UINT status = UX_SUCCESS;

  /* USER CODE BEGIN USBD_STORAGE_Write */
  UX_PARAMETER_NOT_USED(storage_instance);
  UX_PARAMETER_NOT_USED(lun);
  UX_PARAMETER_NOT_USED(media_status);


   ux_utility_memory_copy(ram_disk_memory + lba * 512, data_pointer,
                          number_blocks * 512);

   status = UX_STATE_NEXT;

  /* USER CODE END USBD_STORAGE_Write */

  return status;
}

/**
  * @brief  USBD_STORAGE_Flush
  *         This function is invoked to flush media.
  * @param  storage_instance : Pointer to the storage class instance.
  * @param  lun: Logical unit number is the command is directed to.
  * @param  number_blocks: number of sectors to read/write.
  * @param  lba: Logical block address is the sector address to read.
  * @param  media_status: should be filled out exactly like the media status
  *                       callback return value.
  * @retval status
  */
UINT USBD_STORAGE_Flush(VOID *storage_instance, ULONG lun, ULONG number_blocks,
                        ULONG lba, ULONG *media_status)
{
  UINT status = UX_SUCCESS;

  /* USER CODE BEGIN USBD_STORAGE_Flush */
  UX_PARAMETER_NOT_USED(storage_instance);
  UX_PARAMETER_NOT_USED(lun);
  UX_PARAMETER_NOT_USED(number_blocks);
  UX_PARAMETER_NOT_USED(lba);
  UX_PARAMETER_NOT_USED(media_status);

  /* USER CODE END USBD_STORAGE_Flush */

  return status;
}

/**
  * @brief  USBD_STORAGE_Status
  *         This function is invoked to obtain the status of the device.
  * @param  storage_instance : Pointer to the storage class instance.
  * @param  lun: Logical unit number is the command is directed to.
  * @param  media_id: is not currently used.
  * @param  media_status: should be filled out exactly like the media status
  *                       callback return value.
  * @retval status
  */
UINT USBD_STORAGE_Status(VOID *storage_instance, ULONG lun, ULONG media_id,
                         ULONG *media_status)
{
  UINT status = UX_SUCCESS;

  /* USER CODE BEGIN USBD_STORAGE_Status */
  UX_PARAMETER_NOT_USED(storage_instance);
  UX_PARAMETER_NOT_USED(lun);
  UX_PARAMETER_NOT_USED(media_id);
  UX_PARAMETER_NOT_USED(media_status);
  /* USER CODE END USBD_STORAGE_Status */

  return status;
}

/**
  * @brief  USBD_STORAGE_Notification
  *         This function is invoked to obtain the notification of the device.
  * @param  storage_instance : Pointer to the storage class instance.
  * @param  lun: Logical unit number is the command is directed to.
  * @param  media_id: is not currently used.
  * @param  notification_class: specifies the class of notification.
  * @param  media_notification: response for the notification.
  * @param  media_notification_length: length of the response buffer.
  * @retval status
  */
UINT USBD_STORAGE_Notification(VOID *storage_instance, ULONG lun, ULONG media_id,
                               ULONG notification_class, UCHAR **media_notification,
                               ULONG *media_notification_length)
{
  UINT status = UX_SUCCESS;

  /* USER CODE BEGIN USBD_STORAGE_Notification */
  UX_PARAMETER_NOT_USED(storage_instance);
  UX_PARAMETER_NOT_USED(lun);
  UX_PARAMETER_NOT_USED(media_id);
  UX_PARAMETER_NOT_USED(notification_class);
  UX_PARAMETER_NOT_USED(media_notification);
  UX_PARAMETER_NOT_USED(media_notification_length);
  /* USER CODE END USBD_STORAGE_Notification */

  return status;
}

/**
  * @brief  USBD_STORAGE_GetMediaLastLba
  *         Get Media last LBA.
  * @param  none
  * @retval last lba
  */
ULONG USBD_STORAGE_GetMediaLastLba(VOID)
{
  ULONG LastLba = 0U;

  /* USER CODE BEGIN USBD_STORAGE_GetMediaLastLba */
  LastLba = RAM_DISK_LAST_LBA;
  /* USER CODE END USBD_STORAGE_GetMediaLastLba */

  return LastLba;
}

/**
  * @brief  USBD_STORAGE_GetMediaBlocklength
  *         Get Media block length.
  * @param  none.
  * @retval block length.
  */
ULONG USBD_STORAGE_GetMediaBlocklength(VOID)
{
  ULONG MediaBlockLen = 0U;

  /* USER CODE BEGIN USBD_STORAGE_GetMediaBlocklength */
  MediaBlockLen = 512;
  /* USER CODE END USBD_STORAGE_GetMediaBlocklength */

  return MediaBlockLen;
}

/* USER CODE BEGIN 1 */

VOID USBX_STORAGE_DISK_INIT(VOID)
{
  ux_utility_memory_set(ram_disk_memory, 0, RAM_DISK_SIZE);

  ux_utility_memory_copy(ram_disk_memory + 0 * 512, (void*)sector_0, sizeof(sector_0));

  /* Update number of sectors */
  if (RAM_DISK_N_LBA >= 0x100)
  {
    ux_utility_short_put(ram_disk_memory + 0x13, 0);
    ux_utility_long_put(ram_disk_memory + 0x20, RAM_DISK_N_LBA);
  }
  else
  {
    ux_utility_short_put(ram_disk_memory + 0x13, RAM_DISK_N_LBA);
    ux_utility_long_put(ram_disk_memory + 0x20, 0);
  }

  ux_utility_memory_copy(ram_disk_memory + 6 * 512, (void*)sector_6_7, sizeof(sector_6_7));
  ux_utility_memory_copy(ram_disk_memory + 7 * 512, (void*)sector_6_7, sizeof(sector_6_7));
  ux_utility_memory_copy(ram_disk_memory + 8 * 512, (void*)sector_8, sizeof(sector_8));

}

/* USER CODE END 1 */
