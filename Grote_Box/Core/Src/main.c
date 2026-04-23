/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
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
CAN_HandleTypeDef hcan1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t ledsA = 0x00;   // GPIOA (LED 0 t/m 7)
uint8_t ledsB = 0x00;   // GPIOB (LED 8 en 9)

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t             TxData[8];
uint8_t             RxData[8];
uint32_t            TxMailbox;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_CAN1_Init(void);
/* USER CODE BEGIN PFP */
void setLedBar(uint8_t index, uint8_t on);
void setLed0(uint8_t on);
void setLed1(uint8_t on);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
  uint8_t setA = 0b00000000;
  uint8_t setB = 0b01010100;
  uint8_t pullUps = 0b01010100;

  CAN_FilterTypeDef  sFilterConfig;

    // Configure Filter: Accept ALL messages (Mask 0)
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK) {
  	  Error_Handler();
    }

    // Start the CAN peripheral
    if (HAL_CAN_Start(&hcan1) != HAL_OK) {
  	  Error_Handler();
    }

    // Activate the Notification (Interrupt) for incoming messages
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
  	  Error_Handler();
    }

    // Prepare the Transmit Header (The Envelope)
    TxHeader.StdId = 0x777;                 // Give this STM32 a recognizable ID
    TxHeader.ExtId = 0x00;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 4;                       // Sending 4 bytes
    TxHeader.TransmitGlobalTime = DISABLE;

    // Print a startup message to the PC so we know it booted safely
    char boot_msg[] = "\r\n\r\n--- STM32 CAN NODE ALIVE & RUNNING ---\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)boot_msg, strlen(boot_msg), 1000);



  // IODIRA (0x00) → alle A‑pins output
  HAL_I2C_Mem_Write(&hi2c1, 0x21 << 1, 0x00, 1, &setA, 1, 100);

  // IODIRB (0x01) → alle B‑pins setten (in en out)
  HAL_I2C_Mem_Write(&hi2c1, 0x21 << 1, 0x01, 1, &setB, 1, 100);

  // Setten pullups voor GPIOB
  HAL_I2C_Mem_Write(&hi2c1, 0x21 << 1, 0x0D, 1, &pullUps, 1, 100);




  // Eerst alle LEDS uitzetten, LAAT DIT STAAN
  HAL_I2C_Mem_Write(&hi2c1, 0x21 << 1, 0x12, 1, &ledsA, 1, 100); // GPIOA = 0
  HAL_I2C_Mem_Write(&hi2c1, 0x21 << 1, 0x13, 1, &ledsB, 1, 100); // GPIOB = 0

  setLedBar(9,1);
  setLedBar(7,1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	    uint8_t value = 0;
	    HAL_I2C_Mem_Read(&hi2c1, 0x21 << 1, 0x13, 1, &value, 1, 100);
	    uint8_t a = HAL_GPIO_ReadPin(GPIOB, LED_Button_0_Pin);
	    uint8_t b = HAL_GPIO_ReadPin(GPIOA, LED_Button_1_Pin);

	    if (!(value & (1 << 2))){
	    	setLedBar(2,1);
	    	//HAL_GPIO_WritePin(GPIOA, Servo_Onder_Pin, 1);
	    }else{
	    	setLedBar(2,0);
	    }

	    if(!(value & (1 << 4))){
	    	setLedBar(1,1);
	    }else{
	    	setLedBar(1,0);
	    }

	    if(!(value & (1 << 6))){
	    	setLedBar(0,1);
	    }else{
	    	setLedBar(0,0);
	    }

	    if(!a){
	    	setLedBar(4,1);
	    }else{
	    	setLedBar(4,0);
	    }

	    if(!b){
	    	setLedBar(3,1);
	    }else{
	    	setLedBar(3,0);
	    }

	    // Load the Heartbeat Data
	    	  TxData[0] = 0x42;
	    	  TxData[1] = 0x52;
	    	  TxData[2] = 0x62;
	    	  TxData[3] = 0x72;

	    	  // Try to send the message to the Raspberry Pi
	    	  if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) == HAL_OK) {
	    		  // Print to PC to confirm it pushed to the CAN bus
	    		  char tx_msg[] = "-> Sent Heartbeat to Pi (ID 0x777)\r\n";
	    		  HAL_UART_Transmit(&huart2, (uint8_t*)tx_msg, strlen(tx_msg), 100);
	    	  } else {
	    		  // Print to PC if it failed (e.g. Pi is disconnected)
	    		  char err_msg[] = "X Failed to send Heartbeat (No ACK from Pi)\r\n";
	    		  HAL_UART_Transmit(&huart2, (uint8_t*)err_msg, strlen(err_msg), 100);
	    	  }

	    	  HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 10;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10D19CE4;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Button_0_Pin */
  GPIO_InitStruct.Pin = LED_Button_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(LED_Button_0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Button_1_Pin */
  GPIO_InitStruct.Pin = LED_Button_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(LED_Button_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void setLedBar(uint8_t index, uint8_t on){
	if (index < 8){
		if (on){
			ledsA |= (1 << index);
		}else{
			ledsA &= ~(1 << index);
		}
		HAL_I2C_Mem_Write(&hi2c1, 0x21 << 1, 0x12, 1, &ledsA, 1, 100);
	}else if (index < 10){
		uint8_t bit = index - 8;
		if (on){
			ledsB |= (1 << bit);
		}else{
			ledsB &= ~(1 << bit);
		}
		HAL_I2C_Mem_Write(&hi2c1, 0x21 << 1, 0x13, 1, &ledsB, 1, 100);
	}

}

// This function runs automatically whenever a CAN message arrives
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {

		char uart_buf[100];
		int len;

		// 1. Print the incoming CAN ID to the PC terminal
		len = sprintf(uart_buf, "\r\n[RECEIVED FROM PI] ID: 0x%lX | DLC: %ld | Data: ", RxHeader.StdId, RxHeader.DLC);
		HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, len, 100);

		// 2. Loop through the data bytes and print them in Hex
		for (int i = 0; i < RxHeader.DLC; i++) {
			len = sprintf(uart_buf, "%02X ", RxData[i]);
			HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, len, 100);
		}

		// 3. Print a new line at the end
		len = sprintf(uart_buf, "\r\n");
		HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, len, 100);

		// 4. Toggle the LED so you have a physical visual indicator!
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
