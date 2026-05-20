#include "main.h"

/* Private variables */
TIM_HandleTypeDef htim3;

/* Function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM3_Init();

    // Start PWM op kanaal 1
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

    char buffer[50];
    sprintf(buffer, "Dit is een bewijs dat het programma runt.");
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    while (1)
    {
        // Niets doen → buzzer moet continu een toon geven
    }
}

/* ================= CLOCK ================= */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

/* ================= GPIO ================= */
static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // PB0 als alternate function (PWM)
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3; // ⚠️ belangrijk!

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* ================= TIMER ================= */
static void MX_TIM3_Init(void)
{
    __HAL_RCC_TIM3_CLK_ENABLE();

    TIM_OC_InitTypeDef sConfigOC = {0};

    htim3.Instance = TIM3;

    // Timer frequentie instellen:
    // Stel: 4 MHz klok → delen naar ~2 kHz
    htim3.Init.Prescaler = 40 - 1;   // deelt clock
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 100 - 1;     // bepaalt frequentie
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_PWM_Init(&htim3);

    // Duty cycle 50%
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 50;  // 50% van 100
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;

    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
}


