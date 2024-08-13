/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_audio.c
  * @author  MCD Application Team
  * @brief   USBX Device Audio applicative source file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "ux_device_audio.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  USBD_AUDIO_Activate
  *         This function is called when insertion of a Audio device.
  * @param  audio_instance: Pointer to the audio class instance.
  * @retval none
  */
VOID USBD_AUDIO_Activate(VOID *audio_instance)
{
  /* USER CODE BEGIN USBD_AUDIO_Activate */
  UX_PARAMETER_NOT_USED(audio_instance);
  /* USER CODE END USBD_AUDIO_Activate */

  return;
}

/**
  * @brief  USBD_AUDIO_Deactivate
  *         This function is called when extraction of a Audio device.
  * @param  audio_instance: Pointer to the audio class instance.
  * @retval none
  */
VOID USBD_AUDIO_Deactivate(VOID *audio_instance)
{
  /* USER CODE BEGIN USBD_AUDIO_Deactivate */
  UX_PARAMETER_NOT_USED(audio_instance);
  /* USER CODE END USBD_AUDIO_Deactivate */

  return;
}

/**
  * @brief  USBD_AUDIO_ControlProcess
  *         This function is invoked to manage the UAC class requests.
  * @param  audio_instance: Pointer to Audio class stream instance.
  * @param  transfer: Pointer to the transfer request.
  * @retval status
  */
UINT USBD_AUDIO_ControlProcess(UX_DEVICE_CLASS_AUDIO *audio_instance,
                               UX_SLAVE_TRANSFER *transfer)
{
  UINT status  = UX_SUCCESS;

  /* USER CODE BEGIN USBD_AUDIO_ControlProcess */
  UX_PARAMETER_NOT_USED(audio_instance);
  UX_PARAMETER_NOT_USED(transfer);
  /* USER CODE END USBD_AUDIO_ControlProcess */

  return status;
}

/**
  * @brief  USBD_AUDIO_SetControlValues
  *         This function is invoked to Set audio control values.
  * @param  none
  * @retval none
  */
VOID USBD_AUDIO_SetControlValues(VOID)
{
  /* USER CODE BEGIN USBD_AUDIO_SetControlValues */

  /* USER CODE END USBD_AUDIO_SetControlValues */

  return;
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
