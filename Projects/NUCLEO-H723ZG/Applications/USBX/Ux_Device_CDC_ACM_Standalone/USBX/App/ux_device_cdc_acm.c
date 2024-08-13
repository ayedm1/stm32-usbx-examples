/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_cdc_acm.c
  * @author  MCD Application Team
  * @brief   USBX Device CDC ACM applicative source file
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
#include "ux_device_cdc_acm.h"

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

#define APP_RX_DATA_SIZE   2048
#define APP_TX_DATA_SIZE   2048

/* Rx/TX flag */
#define NO_NEW_DATA               0x00
#define RX_NEW_RECEIVED_DATA      0x01
#define TX_NEW_TRANSMITTED_DATA   0x02

#define CDC_ACM_READ_STATE_WAIT      0x00
#define CDC_ACM_READ_STATE_TX_START  0x01
#define CDC_ACM_READ_STATE_TX_WAIT   0x02

#define CDC_ACM_WRITE_STATE_WAIT      0x00
#define CDC_ACM_WRITE_STATE_RX_START  0x001

/* Data length for vcp */
#define VCP_WORDLENGTH8  8
#define VCP_WORDLENGTH9  9

/* the minimum baudrate */
#define MIN_BAUDRATE     9600

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

UX_SLAVE_CLASS_CDC_ACM  *cdc_acm;

#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma location = ".UsbxAppSection"
#elif defined ( __CC_ARM ) || defined(__ARMCC_VERSION) /* ARM Compiler 5/6 */
__attribute__((section(".UsbxAppSection")))
#elif defined ( __GNUC__ ) /* GNU Compiler */
__attribute__((section(".UsbxAppSection")))
#endif
uint8_t UserRxBuffer[APP_RX_DATA_SIZE];
uint8_t UserTxBuffer[APP_TX_DATA_SIZE];

uint32_t UserTxBufPtrIn;
uint32_t UserTxBufPtrOut;

static UINT read_state = CDC_ACM_READ_STATE_WAIT;
static UINT write_state = CDC_ACM_WRITE_STATE_WAIT;
__IO ULONG EventFlag = NO_NEW_DATA;

UART_HandleTypeDef *uart_handler;

UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER CDC_VCP_LineCoding =
{
  115200, /* baud rate */
  0x00,   /* stop bits-1 */
  0x00,   /* parity - none */
  0x08    /* nb. of bits 8 */
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
extern VOID USBX_APP_UART_Init(UART_HandleTypeDef **huart);
static void USBD_CDC_VCP_Config(UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER *);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  USBD_CDC_ACM_Activate
  *         This function is called when insertion of a CDC ACM device.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_Activate(VOID *cdc_acm_instance)
{
  /* USER CODE BEGIN USBD_CDC_ACM_Activate */

  /* Save the CDC instance */
  cdc_acm = (UX_SLAVE_CLASS_CDC_ACM*) cdc_acm_instance;

  /* Configure the UART peripheral */
  USBX_APP_UART_Init(&uart_handler);

  /* Get default UART parameters */
  CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_baudrate = uart_handler->Init.BaudRate;

  /* Set the UART data type : only 8bits and 9bits are supported */
  switch (uart_handler->Init.WordLength)
  {
    case UART_WORDLENGTH_8B:
    {
      /* Set UART data bit to 8 */
      CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_data_bit = VCP_WORDLENGTH8;
      break;
    }

    case UART_WORDLENGTH_9B:
    {
      /* Set UART data bit to 9 */
      CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_data_bit = VCP_WORDLENGTH9;
      break;
    }

    default :
    {
      /* By default set UART data bit to 8 */
      CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_data_bit = VCP_WORDLENGTH8;
      break;
    }
  }

  /* Get UART Parity */
  CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_parity = uart_handler->Init.Parity;

  /* Get UART StopBits */
  CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_stop_bit = uart_handler->Init.StopBits;

  /* Set device class_cdc_acm with default parameters */
  if (ux_device_class_cdc_acm_ioctl(cdc_acm, UX_SLAVE_CLASS_CDC_ACM_IOCTL_SET_LINE_CODING,
                                    &CDC_VCP_LineCoding) != UX_SUCCESS)
  {
    Error_Handler();
  }

  /* Receive an amount of data in interrupt mode */
  if (HAL_UART_Receive_IT(uart_handler, (uint8_t *)UserTxBuffer, 1) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }

  /* USER CODE END USBD_CDC_ACM_Activate */

  return;
}

/**
  * @brief  USBD_CDC_ACM_Deactivate
  *         This function is called when extraction of a CDC ACM device.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_Deactivate(VOID *cdc_acm_instance)
{
  /* USER CODE BEGIN USBD_CDC_ACM_Deactivate */
  UX_PARAMETER_NOT_USED(cdc_acm_instance);

  /* Reset the cdc acm instance */
  cdc_acm = UX_NULL;

  /* DeInitialize the UART peripheral */
  HAL_UART_DeInit(uart_handler);

  /* USER CODE END USBD_CDC_ACM_Deactivate */

  return;
}

/**
  * @brief  USBD_CDC_ACM_ParameterChange
  *         This function is invoked to manage the CDC ACM class requests.
  * @param  cdc_acm_instance: Pointer to the cdc acm class instance.
  * @retval none
  */
VOID USBD_CDC_ACM_ParameterChange(VOID *cdc_acm_instance)
{
  /* USER CODE BEGIN USBD_CDC_ACM_ParameterChange */
  UX_PARAMETER_NOT_USED(cdc_acm_instance);

  ULONG request;
  UX_SLAVE_TRANSFER *transfer_request;
  UX_SLAVE_DEVICE *device;

  /* Get the pointer to the device */
  device = &_ux_system_slave -> ux_system_slave_device;

  /* Get the pointer to the transfer request associated with the control endpoint */
  transfer_request = &device -> ux_slave_device_control_endpoint.ux_slave_endpoint_transfer_request;

  request = *(transfer_request -> ux_slave_transfer_request_setup + UX_SETUP_REQUEST);

  switch (request)
  {
    case UX_SLAVE_CLASS_CDC_ACM_SET_LINE_CODING :

      /* Get the Line Coding parameters */
      if (ux_device_class_cdc_acm_ioctl(cdc_acm, UX_SLAVE_CLASS_CDC_ACM_IOCTL_GET_LINE_CODING,
                                        &CDC_VCP_LineCoding) != UX_SUCCESS)
      {
        Error_Handler();
      }

      /* Check if baudrate < 9600) then set it to 9600 */
      if (CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_baudrate < MIN_BAUDRATE)
      {
        CDC_VCP_LineCoding.ux_slave_class_cdc_acm_parameter_baudrate = MIN_BAUDRATE;

        /* Set the new configuration of ComPort */
        USBD_CDC_VCP_Config(&CDC_VCP_LineCoding);
      }
      else
      {
        /* Set the new configuration of ComPort */
        USBD_CDC_VCP_Config(&CDC_VCP_LineCoding);
      }

      break;

    case UX_SLAVE_CLASS_CDC_ACM_GET_LINE_CODING :

      /* Set the Line Coding parameters */
      if (ux_device_class_cdc_acm_ioctl(cdc_acm, UX_SLAVE_CLASS_CDC_ACM_IOCTL_SET_LINE_CODING,
                                        &CDC_VCP_LineCoding) != UX_SUCCESS)
      {
        Error_Handler();
      }

      break;

    case UX_SLAVE_CLASS_CDC_ACM_SET_CONTROL_LINE_STATE :
    default :
      break;
  }

  /* USER CODE END USBD_CDC_ACM_ParameterChange */

  return;
}

/* USER CODE BEGIN 1 */

/**
  * @brief  USBX_DEVICE_CDC_ACM_Read_Task
  *         Function implementing USBX_DEVICE_CDC_ACM_Read_Task.
  * @param  none
  * @retval none
  */
VOID USBX_DEVICE_CDC_ACM_Read_Task(VOID)
{
  UX_SLAVE_DEVICE *device;
  UINT status;
  static ULONG actual_length;

  device = &_ux_system_slave->ux_system_slave_device;

  /* Check if device is configured */
  if ((device->ux_slave_device_state == UX_DEVICE_CONFIGURED) && (cdc_acm != UX_NULL))
  {

    switch (read_state)
    {
      case CDC_ACM_READ_STATE_WAIT:

        /* Read the received data */
        if (ux_device_class_cdc_acm_read_run(cdc_acm,
                                             (UCHAR *)UserRxBuffer, 64,
                                             &actual_length) == UX_STATE_NEXT)
        {
          if (actual_length != 0)
          {
            /* Update cdc acm read state machine */
            read_state = CDC_ACM_READ_STATE_TX_START;
          }
        }

        break;

      case CDC_ACM_READ_STATE_TX_START:

        /* Send the data via UART */
        status = HAL_UART_Transmit_DMA(uart_handler, (uint8_t *)UserRxBuffer,
                                       actual_length);
        if (status == HAL_OK)
        {
          /* Update cdc acm read state machine */
          read_state = CDC_ACM_READ_STATE_TX_WAIT;
        }
        else if (status == HAL_BUSY)
        {
          break;
        }
        else if (status != HAL_OK)
        {
          /* Transfer error in reception process */
          Error_Handler();
        }

        break;

      case CDC_ACM_READ_STATE_TX_WAIT:

        if (EventFlag & TX_NEW_TRANSMITTED_DATA)
        {
          EventFlag &= ~TX_NEW_TRANSMITTED_DATA;
        }

        /* Update cdc acm read state machine */
        read_state = CDC_ACM_READ_STATE_WAIT;

        break;

      default:
        break;
    }
  }
}

/**
  * @brief  USBX_DEVICE_CDC_ACM_Write_Task
  *         Function implementing USBX_DEVICE_CDC_ACM_Write_Task.
  * @param  thread_input: Not used
  * @retval none
  */
VOID USBX_DEVICE_CDC_ACM_Write_Task(VOID)
{
  UX_SLAVE_DEVICE *device;
  ULONG actual_length, buffptr, buffsize;

  device = &_ux_system_slave->ux_system_slave_device;

  /* Check if device is con figured */
  if ((device->ux_slave_device_state == UX_DEVICE_CONFIGURED) && (cdc_acm != UX_NULL))
  {

    switch (write_state)
    {

      case CDC_ACM_WRITE_STATE_WAIT:

        if (EventFlag & RX_NEW_RECEIVED_DATA)
        {
          /* Reset flag */
          EventFlag &= ~RX_NEW_RECEIVED_DATA;

          /* Check if there is a new data to send */
          if (UserTxBufPtrOut != UserTxBufPtrIn)
          {
            /* Check buffer overflow and Rollback */
            if (UserTxBufPtrOut > UserTxBufPtrIn)
            {
              buffsize = APP_RX_DATA_SIZE - UserTxBufPtrOut;
            }
            else
            {
              /* Calculate data size */
              buffsize = UserTxBufPtrIn - UserTxBufPtrOut;
            }

            /* Copy UserTxBufPtrOut in buffptr */
            buffptr = UserTxBufPtrOut;

            /* Send data over the class cdc_acm_write */
            if (ux_device_class_cdc_acm_write_run(cdc_acm,
                                                  (UCHAR *)(&UserTxBuffer[buffptr]),
                                                  buffsize, &actual_length) == UX_STATE_WAIT)
            {
              /* Update cdc acm write state machine */
              write_state = CDC_ACM_WRITE_STATE_RX_START;
            }

          }
        }

        break;

      case CDC_ACM_WRITE_STATE_RX_START:

        /* Check if dataset is transmitted */
        if (ux_device_class_cdc_acm_write_run(cdc_acm, UX_NULL, 0, &actual_length) == UX_STATE_NEXT)
        {
          /* Increment the UserTxBufPtrOut pointer */
          UserTxBufPtrOut += actual_length;

          /* Rollback UserTxBufPtrOut if it equal to APP_TX_DATA_SIZE */
          if (UserTxBufPtrOut == APP_TX_DATA_SIZE)
          {
            UserTxBufPtrOut = 0;
          }

          /* Update cdc acm write state machine */
          write_state = CDC_ACM_WRITE_STATE_WAIT;
        }

        break;

      default:
        break;
    }
  }
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  huart UART handle.
  * @retval none
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Set TX_NEW_TRANSMITTED_DATA flag */
  EventFlag |= TX_NEW_TRANSMITTED_DATA;
}

/**
  * @brief  Rx Transfer completed callback
  * @param  huart: UART handle
  * @retval none
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Set RX_NEW_RECEIVED_DATA flag */
  EventFlag |= RX_NEW_RECEIVED_DATA;

  /* Increment the UserTxBufPtrIn pointer */
  UserTxBufPtrIn++;

  /* Rollback the UserTxBufPtrIn if it equal to APP_TX_DATA_SIZE */
  if (UserTxBufPtrIn == APP_TX_DATA_SIZE)
  {
    UserTxBufPtrIn = 0;
  }

  /* Start another reception: provide the buffer pointer with offset and the buffer size */
  if (HAL_UART_Receive_IT(uart_handler, (uint8_t *)UserTxBuffer + UserTxBufPtrIn, 1) != HAL_OK)
  {
    /* Transfer error in reception process */
    Error_Handler();
  }

}

/**
  * @brief  UART error callbacks
            Transfer error occurred in reception and/or transmission process.
  * @param  UartHandle: UART handle
  * @retval none
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  Error_Handler();
}

/**
  * @brief  USBD_CDC_VCP_Config
            Configure the COM Port with the parameters received from host.
  * @param  UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER: linecoding struct.
  * @param  CDC_VCP_LineCoding: CDC VCP line coding.
  * @retval none
  */
static VOID USBD_CDC_VCP_Config(UX_SLAVE_CLASS_CDC_ACM_LINE_CODING_PARAMETER
                                *CDC_VCP_LineCoding)
{
  /* Deinitialization UART */
  if (HAL_UART_DeInit(uart_handler) != HAL_OK)
  {
    /* Deinitialization Error */
    Error_Handler();
  }

  /* Check stop bit parameter */
  switch (CDC_VCP_LineCoding->ux_slave_class_cdc_acm_parameter_stop_bit)
  {
    case 0:

      /* Set the UART Stop bit to 1 */
      uart_handler->Init.StopBits = UART_STOPBITS_1;

      break;

    case 2:

      /* Set the UART Stop bit to 2 */
      uart_handler->Init.StopBits = UART_STOPBITS_2;

      break;

    default :

      /* By default set the UART Stop bit to 1 */
      uart_handler->Init.StopBits = UART_STOPBITS_1;

      break;
  }

  /* Check parity parameter */
  switch (CDC_VCP_LineCoding->ux_slave_class_cdc_acm_parameter_parity)
  {
    case 0:

      /* Set the UART parity bit to none */
      uart_handler->Init.Parity = UART_PARITY_NONE;

      break;

    case 1:

      /* Set the UART parity bit to ODD */
      uart_handler->Init.Parity = UART_PARITY_ODD;

      break;

    case 2:

      /* Set the UART parity bit to even */
      uart_handler->Init.Parity = UART_PARITY_EVEN;

      break;

    default :

      /* By default set the UART parity bit to none */
      uart_handler->Init.Parity = UART_PARITY_NONE;

      break;
  }

  /* Set the UART data type : only 8bits and 9bits is supported */
  switch (CDC_VCP_LineCoding->ux_slave_class_cdc_acm_parameter_data_bit)
  {
    case 0x07:

      /* With this configuration a parity (Even or Odd) must be set */
      uart_handler->Init.WordLength = UART_WORDLENGTH_8B;

      break;

    case 0x08:

      if (uart_handler->Init.Parity == UART_PARITY_NONE)
      {
        uart_handler->Init.WordLength = UART_WORDLENGTH_8B;
      }
      else
      {
        uart_handler->Init.WordLength = UART_WORDLENGTH_9B;
      }

      break;

    default :

      uart_handler->Init.WordLength = UART_WORDLENGTH_8B;

      break;
  }

  /* Get the UART baudrate from VCP */
  uart_handler->Init.BaudRate = CDC_VCP_LineCoding->ux_slave_class_cdc_acm_parameter_baudrate;

  /* Set the UART Hw flow control to none */
  uart_handler->Init.HwFlowCtl = UART_HWCONTROL_NONE;

  /* Set the UART mode */
  uart_handler->Init.Mode = UART_MODE_TX_RX;

  /* Set the UART sampling */
  uart_handler->Init.OverSampling = UART_OVERSAMPLING_16;

  /* Initialization UART */
  if (HAL_UART_Init(uart_handler) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Start reception: provide the buffer pointer with offset and the buffer size */
  HAL_UART_Receive_IT(uart_handler, (uint8_t *)(UserTxBuffer + UserTxBufPtrIn), 1);
}

/* USER CODE END 1 */
