/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ILI9341_GFX.h"
#include "fonts.h"
#include "img.h"
#include "xpt2046_touch.h"

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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  	__HAL_SPI_ENABLE(DISP_SPI_PTR); // включаем SPI

    DISP_CS_UNSELECT;
    TOUCH_CS_UNSELECT;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ILI9341_Init(); // инициализация дисплея

    /////////////////////////// далее демонстрируются различные пользовательские функции ////////////////////////////
    ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1); // установка ориентации экрана (варианты в файле ILI9341_GFX.h)

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ILI9341_Fill_Screen(BLACK); // заливка всего экрана цветом (цвета в файле ILI9341_GFX.h)

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* вывод строк разными шрифтами (шрифты определены в файле fonts.h, а массивы шрифтов в файле fonts.c)
    первый и второй аргумент это начало координат (справа, сверху), четвёртый аргумент шрифт
    два последних аргумента это цвет шрифта и цвет фона шрифта */

    ILI9341_WriteString(10, 10, "Hello World", Font_7x10, WHITE, BLACK); // можно передавать непосредственно текст
    ILI9341_WriteString(20, 30, "Hello World", Font_11x18, WHITE, MYFON);
    ILI9341_WriteString(30, 60, "Hello World", Font_16x26, BLUE, DARKGREEN);

    char txt_buf[] = "Hello World";
    ILI9341_WriteString(40, 96, txt_buf, Font_16x26, RED, GREEN); // можно передавать массив

    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ILI9341_Draw_Pixel(100, 100, WHITE); // рисует пиксель (координаты и цвет)
    HAL_Delay(300);
    ILI9341_Draw_Pixel(102, 100, MAROON);
    HAL_Delay(300);
    ILI9341_Draw_Pixel(100, 102, BLUE);
    HAL_Delay(300);
    ILI9341_Draw_Pixel(102, 102, RED);
    HAL_Delay(300);

    for(uint8_t i = 0; i < 100; i++)
    {
            ILI9341_Draw_Pixel(i, 20, WHITE);
            HAL_Delay(10);
    }

    for(uint8_t i = 0; i < 100; i++)
    {
            ILI9341_Draw_Pixel(40, i, BLUE);
            HAL_Delay(10);
    }
//
    for(uint8_t i = 0; i < 100; i++)
    {
            ILI9341_Draw_Pixel(i, i, RED);
            HAL_Delay(10);
    }

    HAL_Delay(1000);
//
    ILI9341_Fill_Screen(MYFON);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ILI9341_Draw_Rectangle(10, 10, 50, 70, WHITE); // рисует закрашеный прямоугольник (первые два аргумента это начальные координаты, а следующие два это ширина и высота)

    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ILI9341_Draw_Horizontal_Line(10, 10, 200, WHITE); // рисует горизонтальную линию (первые два аргумента это начальные координаты, а третий длина)

    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ILI9341_Draw_Vertical_Line(10, 10, 200, WHITE); // рисует вертикальную линию (первые два аргумента это начальные координаты, а третий длина)

    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ILI9341_Random_line(160, 120, 50, 175, WHITE); // рисует произвольную линию (первые два аргумента это начальные координаты, а третий и четвёртый - конечные)

    HAL_Delay(1000);

    ILI9341_Random_line(123, 180, 150, 75, WHITE);

    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ILI9341_Draw_Hollow_Circle(100, 100, 50, WHITE); // рисует прозрачный круг (первые два аргумента это координаты центра, а третий радиус)

    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ILI9341_Draw_Filled_Circle(150, 100, 40, WHITE); // рисует закрашеный круг (первые два аргумента это координаты центра, а третий радиус)

    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ILI9341_Draw_Hollow_Rectangle_Coord(10, 10, 50, 70, WHITE); // рисует прозрачный прямоугольник (первые два аргумента это начальные координаты, а вторые два конечные)

    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ILI9341_Draw_Filled_Rectangle_Coord(20, 20, 70, 60, WHITE); // рисует закрашеный прямоугольник (первые два аргумента это начальные координаты, а вторые два конечные)

    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);

    ////////////////////////////////// ВЫВОД КАРТ�?НК�? �?З МАСС�?ВА //////////////////////////////////////////
    uint16_t size_img = sizeof(img_logo); // размер картинки в байтах (картинка лежит в файле img.h)

    ILI9341_Draw_Image(img_logo, 30, 30, 80, 80, size_img); // подробности см. в статье

    HAL_Delay(1000);

    //////////////////////////////////// смена ориентации экрана ///////////////////////////////////////

    ILI9341_Fill_Screen(MYFON);
    ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
    ILI9341_Draw_Image(img_logo, 30, 30, 80, 80, size_img);
    ILI9341_WriteString(30, 120, "SCREEN_VERTICAL_1", Font_11x18, WHITE, MYFON);
    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);
    ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
    ILI9341_Draw_Image(img_logo, 30, 30, 80, 80, size_img);
    ILI9341_WriteString(30, 120, "SCREEN_HORIZONTAL_1", Font_11x18, WHITE, MYFON);
    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);
    ILI9341_Set_Rotation(SCREEN_VERTICAL_2);
    ILI9341_Draw_Image(img_logo, 30, 30, 80, 80, size_img);
    ILI9341_WriteString(30, 120, "SCREEN_VERTICAL_2", Font_11x18, WHITE, MYFON);
    HAL_Delay(1000);

    ILI9341_Fill_Screen(MYFON);
    ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
    ILI9341_Draw_Image(img_logo, 30, 30, 80, 80, size_img);
    ILI9341_WriteString(30, 120, "SCREEN_HORIZONTAL_2", Font_11x18, WHITE, MYFON);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  huart2.Init.BaudRate = 38400;
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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|TFT_CS_Pin|TFT_RST_Pin|TOUCH_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin TFT_CS_Pin TFT_RST_Pin TOUCH_CS_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|TFT_CS_Pin|TFT_RST_Pin|TOUCH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_DC_Pin */
  GPIO_InitStruct.Pin = TFT_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TFT_DC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : IRQ_Pin */
  GPIO_InitStruct.Pin = IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IRQ_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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

#ifdef  USE_FULL_ASSERT
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
