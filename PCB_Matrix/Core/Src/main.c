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
//#include "usart.h"              // << REQUIRED
#include "stm32l4xx_hal.h"      // << recommended

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NODE_PI         0x01
#define NODE_MC_encoder	0x02
#define MY_NODE_ID 	    0x03
#define NODE_MC_boven   0x04
#define NODE_MC_onder   0x05
#define NODE_BROADCAST  0xFF

#define TEST_MSG		0x00
#define READ_DISTANCE   0x12
#define READ_ACCESS 0x70

#define MATRIX_LED_ON	0x50
#define MATRIX_LED_OFF	0x51
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN PV */
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t             TxData[8];
uint8_t             RxData[8];
uint32_t            TxMailbox;

volatile uint8_t last_source = 0;


//Keypad global variables

volatile uint8_t LedAan1 = 0;
volatile uint8_t LedAan2 = 0;

char keypadBuffer[3] = {0};   // 2 chars + null terminator
uint8_t keypadIndex = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
/* USER CODE BEGIN PFP */
void matrixLedOn(void);
void matrixLedOff(void);
char ReadKeypad(void);
void TestLedActions(void);

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
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
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
    TxHeader.StdId = 0x103;
    TxHeader.ExtId = 0x00;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 8;
    TxHeader.TransmitGlobalTime = DISABLE;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
{
    char key = ReadKeypad();

    // GIT TEST COMMIT
    if (key != 0)
    {
        keypadBuffer[keypadIndex++] = key;

        if (keypadIndex >= 2)
        {
            keypadBuffer[2] = '\0';

            // Check combinaties
            if (strcmp(keypadBuffer, "1A") == 0)
            {
                LedAan1 = 1;

                TxHeader.DLC = 8;
                TxData[0] = NODE_PI;
                TxData[1] = MY_NODE_ID;
                TxData[2] = READ_ACCESS;
                TxData[3] = 0x00;
                TxData[4] = 0x01;
                TxData[5] = 0x00;
                TxData[6] = 0x00;
                TxData[7] = 0x00;

                HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);

            }
            else if (strcmp(keypadBuffer, "2A") == 0)
            {
                LedAan2 = 1;

                TxHeader.DLC = 8;
                TxData[0] = NODE_PI;
                TxData[1] = MY_NODE_ID;
                TxData[2] = READ_ACCESS;
                TxData[3] = 0x00;
                TxData[4] = 0x02;
                TxData[5] = 0x00;
                TxData[6] = 0x00;
                TxData[7] = 0x00;

                HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
            }

            // Reset buffer
            keypadIndex = 0;
            keypadBuffer[0] = 0;
            keypadBuffer[1] = 0;
        }

        // Wacht tot loslaten
        if (ReadKeypad() != 0) {
        	HAL_Delay(50);
        }

    }
//    TestLedActions();
}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
char ReadKeypad(void)
{
    const char keymap[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };

    HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_SET);

    if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin)) { HAL_Delay(20); return '1'; }
    if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)) { HAL_Delay(20); return '2'; }
    if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin)) { HAL_Delay(20); return '3'; }
    if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin)) { HAL_Delay(20); return 'A'; }

    HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_SET);

    if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin)) { HAL_Delay(20); return '4'; }
    if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)) { HAL_Delay(20); return '5'; }
    if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin)) { HAL_Delay(20); return '6'; }
    if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin)) { HAL_Delay(20); return 'B'; }

    HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_SET);

    if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin)) { HAL_Delay(20); return '7'; }
    if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)) { HAL_Delay(20); return '8'; }
    if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin)) { HAL_Delay(20); return '9'; }
    if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin)) { HAL_Delay(20); return 'C'; }

    HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_RESET);

    if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin)) { HAL_Delay(20); return '*'; }
    if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)) { HAL_Delay(20); return '0'; }
    if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin)) { HAL_Delay(20); return '#'; }
    if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin)) { HAL_Delay(20); return 'D'; }

    return 0;  // Geen toets ingedrukt
}

//void TestLedActions(void)
//{
//    const uint16_t LED_PIN = GPIO_PIN_8;
//    GPIO_TypeDef* LED_PORT = GPIOA;
//
//    // LedAan1 → LED 1x aan (1 seconde)
//    if (LedAan1 == 1)
//    {
//        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
//        HAL_Delay(1000);
//        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
//
//        LedAan1 = 0;
//    }
//
//    // LedAan2 → LED 2x aan (2 seconden totaal)
//    if (LedAan2 == 1)
//    {
//        for (int i = 0; i < 2; i++)
//        {
//            HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
//            HAL_Delay(1000);
//            HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
//            HAL_Delay(1000);
//        }
//
//        LedAan2 = 0;
//    }
//}


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
  HAL_GPIO_WritePin(GPIOA, R3_Pin|R4_Pin|R2_Pin|R1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : C4_Pin C3_Pin C2_Pin C1_Pin */
  GPIO_InitStruct.Pin = C4_Pin|C3_Pin|C2_Pin|C1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : R3_Pin R4_Pin LED0_Pin R2_Pin
                           R1_Pin */
  GPIO_InitStruct.Pin = R3_Pin|R4_Pin|LED0_Pin|R2_Pin
                          |R1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED0Button_Pin */
  GPIO_InitStruct.Pin = LED0Button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LED0Button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : VCP_RX_Pin */
  GPIO_InitStruct.Pin = VCP_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF3_USART2;
  HAL_GPIO_Init(VCP_RX_GPIO_Port, &GPIO_InitStruct);

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
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {

		char uart_buf[100];
		int len;

		uint8_t target = RxData[0];
		uint8_t source = RxData[1];
		uint8_t cmd    = RxData[2];

		if (target != MY_NODE_ID && target != NODE_BROADCAST){
			return;
		}

		switch(cmd){
		case MATRIX_LED_ON:
			last_source = source;
			matrixLedOn();
			break;

		case MATRIX_LED_OFF:
			last_source = source;
			matrixLedOff();
			break;
		}
	}
}

void matrixLedOn(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);

	TxHeader.DLC = 8;
	TxData[0] = last_source;
	TxData[1] = MY_NODE_ID;
	TxData[2] = 0x00;
	TxData[3] = 0xFF;
	TxData[4] = 0x00;
	TxData[5] = 0x00;
	TxData[6] = 0x00;
	TxData[7] = 0x00;

	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
}

void matrixLedOff(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
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
