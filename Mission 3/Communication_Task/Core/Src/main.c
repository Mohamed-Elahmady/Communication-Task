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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Drivers/mpu6500.h"
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

/* USER CODE BEGIN PV */
UART_HandleTypeDef huart1;
MPU6500_t my_mpu;
MPU6500_AllData_t sensor_data;
char uart_tx_buf[256];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void Format_Float(char *buf, size_t max_len, float val, int decimals)
{
  int sign = (val < 0.0f) ? -1 : 1;
  float abs_val = (val < 0.0f) ? -val : val;
  int int_part = (int)abs_val;

  int multiplier = 1;
  for (int i = 0; i < decimals; i++) multiplier *= 10;

  int frac_part = (int)((abs_val - (float)int_part) * (float)multiplier + 0.5f);
  if (frac_part >= multiplier)
  {
    int_part += 1;
    frac_part = 0;
  }

  if (sign < 0 && (int_part != 0 || frac_part != 0))
  {
    snprintf(buf, max_len, "-%d.%0*d", int_part, decimals, frac_part);
  }
  else
  {
    snprintf(buf, max_len, "+%d.%0*d", int_part, decimals, frac_part);
  }
}
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_Delay(500); /* Wait for power & serial to stabilize */

  /* Send Welcome Banner over USART1 (PA9 TX, PA10 RX) */
  const char *banner = "\r\n==============================================\r\n"
                       "    MPU-6500 Motion Telemetry via USART1      \r\n"
                       "==============================================\r\n";
  HAL_UART_Transmit(&huart1, (uint8_t *)banner, strlen(banner), 200);

  /* Initialize MPU-6500 on I2C1 using PB6 (SCL) and PB7 (SDA) at 100kHz standard mode */
  MPU6500_Status_e mpu_status = MPU6500_InitHardwareI2C(&my_mpu,
                                                        I2C1,
                                                        GPIOB, GPIO_PIN_6,   /* SCL: PB6 (I2C1_SCL) */
                                                        GPIOB, GPIO_PIN_7,   /* SDA: PB7 (I2C1_SDA) */
                                                        100000,              /* 100kHz Standard Mode */
                                                        MPU6500_I2C_ADDR_AD0_LOW);

  if (mpu_status == MPU6500_OK)
  {
    const char *ok_msg = "[SUCCESS] MPU-6500 Initialized Successfully!\r\n\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t *)ok_msg, strlen(ok_msg), 100);
  }
  else
  {
    const char *err_msg = "[WARNING] MPU-6500 Initial Check Pending! Retrying in loop...\r\n\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t *)err_msg, strlen(err_msg), 100);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  char ax_str[16], ay_str[16], az_str[16];
  char gx_str[16], gy_str[16], gz_str[16];
  char temp_str[16];
  uint32_t loop_count = 0;

  while (1)
  {
    loop_count++;

    /* Read all sensor channels */
    if (MPU6500_ReadAllData(&my_mpu, &sensor_data) == MPU6500_OK)
    {
      Format_Float(ax_str, sizeof(ax_str), sensor_data.scaled.accel_x_g, 2);
      Format_Float(ay_str, sizeof(ay_str), sensor_data.scaled.accel_y_g, 2);
      Format_Float(az_str, sizeof(az_str), sensor_data.scaled.accel_z_g, 2);

      Format_Float(gx_str, sizeof(gx_str), sensor_data.scaled.gyro_x_dps, 1);
      Format_Float(gy_str, sizeof(gy_str), sensor_data.scaled.gyro_y_dps, 1);
      Format_Float(gz_str, sizeof(gz_str), sensor_data.scaled.gyro_z_dps, 1);

      Format_Float(temp_str, sizeof(temp_str), sensor_data.scaled.temp_c, 1);

      int len = snprintf(uart_tx_buf, sizeof(uart_tx_buf),
                         "[ACCEL(g)] X:%s Y:%s Z:%s  ||  [GYRO(dps)] X:%s Y:%s Z:%s  ||  [TEMP] %s C\r\n",
                         ax_str, ay_str, az_str,
                         gx_str, gy_str, gz_str,
                         temp_str);

      HAL_UART_Transmit(&huart1, (uint8_t *)uart_tx_buf, len, 100);
      HAL_Delay(100); /* 10Hz stream */
    }
    else
    {
      /* Read failed - Report diagnostics over UART and retry initializing sensor */
      uint8_t who = 0;
      MPU6500_ReadWhoAmI(&my_mpu, &who);
      
      int len = snprintf(uart_tx_buf, sizeof(uart_tx_buf),
                         "[%lu] [DIAG] Waiting for MPU-6500 on I2C1 (PB6/PB7)... (WHO_AM_I: 0x%02X, Addr: 0x%02X)\r\n",
                         loop_count, who, my_mpu.pins.i2c_addr);
      HAL_UART_Transmit(&huart1, (uint8_t *)uart_tx_buf, len, 100);

      /* Retry init */
      MPU6500_Init(&my_mpu);
      HAL_Delay(1000); /* Retry every 1s */
    }
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  /* Enable USART1 clock */
  __HAL_RCC_USART1_CLK_ENABLE();

  huart1.Instance          = USART1;
  huart1.Init.BaudRate     = 9600;
  huart1.Init.WordLength   = UART_WORDLENGTH_8B;
  huart1.Init.StopBits     = UART_STOPBITS_1;
  huart1.Init.Parity       = UART_PARITY_NONE;
  huart1.Init.Mode         = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
