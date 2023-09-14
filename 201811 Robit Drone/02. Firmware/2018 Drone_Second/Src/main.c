/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

#include "MPU9250.h"
#include "Kalman.h"
#include "PID_Control.h"
#include <math.h>

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

int test = 0;

// Variables about Kalman Filtering
double Angle_X, Angle_Y, Angle_Z;
double pastAngle_X, pastAngle_Y, pastAngle_Z;
double accAngle_X, accAngle_Y, accAngle_Z;
int16_t Accel_X, Accel_Y, Accel_Z;
int16_t Gyro_X, Gyro_Y, Gyro_Z;
int16_t Mag_X, Mag_Y, Mag_Z;
Kalman Kal_X;
Kalman Kal_Y;

// Motor PWM signal(400Hz) : 900(init), 1100 to 2000(with prop, operate 4 at the same time)
float motorInit = 900;
float motorPWM1 = 900;
float motorPWM2 = 900;
float motorPWM3 = 900;
float motorPWM4 = 900;

// Reference Value : RC Throttle maybe
float motorValue = 1400;

// Variable for PID Attitude Control
DPID attitude_X;
double target_Angle_X = 0.0;

DPID attitude_Y;
double target_Angle_Y = 0.0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM6_Init(void);
                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

// MPU9250 Applying Functions
void MPU9250_Write(uint8_t Address, uint8_t data);
void MPU9250_Write_bits(uint8_t Address, uint8_t bitStart, uint8_t length, uint8_t data);
uint8_t MPU9250_Read(uint8_t Address);
void init_MPU9250(void);
void read_MPU9250_data(void);

// Kalman Filtering Functions
double getKalman(Kalman* kal, double acc, double gyro, double dt);
void kalman_Init(Kalman* kal, double angle_f, double gyro, double measure);

// Complement Filtering Function
double getComple(double acc, double gyro, double dt);

// PID Attitude Control Function
//void PID_Control(DPID* dst, double target, double input);
void Angular_Control(double target_X, double target_Y);

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

#define MPU9250 MPU9250_DEFAULT_ADDRESS

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  test = 0;
  
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  
  kalman_Init(&Kal_X, 0.3, 0.03, 0.08);
  kalman_Init(&Kal_Y, 0.3, 0.03, 0.08);
  
  //PID_Control_Initialize(&attitude_X, 45.0, 0.55, 10.0);
  PID_Control_Initialize(&attitude_X, 1.0, 0.0, 0.0);

  //PID_Control_Initialize(&attitude_Y, 45.0, 0.55, 10.0);
  PID_Control_Initialize(&attitude_Y, 1.0, 0.0, 0.0);
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM6_Init();

  /* USER CODE BEGIN 2 */

  init_MPU9250();
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_4);
  
  TIM1->CCR1 = (unsigned int)motorInit;
  TIM1->CCR2 = (unsigned int)motorInit;
  TIM1->CCR3 = (unsigned int)motorInit;
  TIM1->CCR4 = (unsigned int)motorInit;
  
  HAL_TIM_Base_Start_IT(&htim6);
  
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

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 179;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 2499;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim1);

}

/* TIM6 init function */
static void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 89;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1999;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM6)
  {
    read_MPU9250_data();
    Angle_X = getKalman(&Kal_X, Accel_X, Gyro_X, 0.01);
    Angle_Y = getKalman(&Kal_Y, Accel_Y, Gyro_Y, 0.01);
    
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) != 0)
    {
      if(test == 5737)
      {
        Angular_Control(target_Angle_X, target_Angle_Y);
        TIM1->CCR1 = (unsigned int)motorPWM1;
        TIM1->CCR2 = (unsigned int)motorPWM2;
        TIM1->CCR3 = (unsigned int)motorPWM3;
        TIM1->CCR4 = (unsigned int)motorPWM4;
      }
    } 
    else
    {
      TIM1->CCR1 = (unsigned int)motorInit;
      TIM1->CCR2 = (unsigned int)motorInit;
      TIM1->CCR3 = (unsigned int)motorInit;
      TIM1->CCR4 = (unsigned int)motorInit;
      motorPWM1 = motorInit;
      motorPWM2 = motorInit;
      motorPWM3 = motorInit;
      motorPWM4 = motorInit;
      test = 5709;
    } 
  }
}

void MPU9250_Write(uint8_t Address, uint8_t data)
{
  HAL_I2C_Mem_Write(&hi2c1, MPU9250, Address, 1, (uint8_t*)&data, 1, 10);
}

void MPU9250_Write_bits(uint8_t Address, uint8_t bitStart, uint8_t length, uint8_t data)
{
  uint8_t temp = 0;
  
  HAL_I2C_Mem_Read(&hi2c1, MPU9250, Address, 1, (uint8_t*)&temp, 1, 10);
  uint8_t mask = 0;
  switch(length)
  {
  case 1:
    mask = 0x01;
    break;
  case 2:
    mask = 0x03;
    break;
  case 3:
    mask = 0x07;
    break;
  case 4:
    mask = 0x0F;
    break;
  case 5:
    mask = 0x1F;
    break;
  case 6:
    mask = 0x3F;
    break;
  case 7:
    mask = 0x7F;
    break;
  case 8:
    mask = 0xFF;
    break;
  }
  
  temp &= ~(mask << bitStart);
  temp |= (data << bitStart);
  HAL_I2C_Mem_Write(&hi2c1, MPU9250, Address, 1, (uint8_t*)&temp, 1, 10);
}

uint8_t MPU9250_Read(uint8_t Address)
{
  uint8_t data = 0;
  HAL_I2C_Mem_Read(&hi2c1, MPU9250, Address, 1, (uint8_t*)&data, 1, 10);
  return data;
}

void init_MPU9250(void)
{
  uint8_t temp = MPU9250_Read(MPU9250_RA_WHO_AM_I);
  HAL_Delay(100);
  MPU9250_Write_bits(MPU9250_RA_PWR_MGMT_1, MPU9250_PWR1_SLEEP_BIT, 0x01, DISABLE);
  HAL_Delay(10);
  MPU9250_Write_bits(MPU9250_RA_PWR_MGMT_1, MPU9250_PWR1_CLKSEL_BIT, MPU9250_PWR1_CLKSEL_LENGTH, MPU9250_CLOCK_PLL_XGYRO);
  MPU9250_Write_bits(MPU9250_RA_GYRO_CONFIG, MPU9250_GCONFIG_FS_SEL_BIT, MPU9250_GCONFIG_FS_SEL_LENGTH, MPU9250_GYRO_FS_250);
  MPU9250_Write_bits(MPU9250_RA_ACCEL_CONFIG, MPU9250_ACONFIG_AFS_SEL_BIT, MPU9250_ACONFIG_AFS_SEL_LENGTH, MPU9250_ACCEL_FS_2);
}

void read_MPU9250_data(void)
{
  Accel_X = (MPU9250_Read(MPU9250_RA_ACCEL_XOUT_H) << 8) | MPU9250_Read(MPU9250_RA_ACCEL_XOUT_L);
  Accel_Y = (MPU9250_Read(MPU9250_RA_ACCEL_YOUT_H) << 8) | MPU9250_Read(MPU9250_RA_ACCEL_YOUT_L);
  Accel_Z = (MPU9250_Read(MPU9250_RA_ACCEL_ZOUT_H) << 8) | MPU9250_Read(MPU9250_RA_ACCEL_ZOUT_L);
  Gyro_X = (MPU9250_Read(MPU9250_RA_GYRO_XOUT_H) << 8) | MPU9250_Read(MPU9250_RA_GYRO_XOUT_L);
  Gyro_Y = (MPU9250_Read(MPU9250_RA_GYRO_YOUT_H) << 8) | MPU9250_Read(MPU9250_RA_GYRO_YOUT_L);
  Gyro_Z = (MPU9250_Read(MPU9250_RA_GYRO_ZOUT_H) << 8) | MPU9250_Read(MPU9250_RA_GYRO_ZOUT_L);
  //Mag_X = (MPU9250_Read(MPU9250_RA_MAG_XOUT_H) << 8) | MPU9250_Read(MPU9250_RA_MAG_XOUT_L);
  //Mag_Y = (MPU9250_Read(MPU9250_RA_MAG_YOUT_H) << 8) | MPU9250_Read(MPU9250_RA_MAG_YOUT_L);
  //Mag_Z = (MPU9250_Read(MPU9250_RA_MAG_ZOUT_H) << 8) | MPU9250_Read(MPU9250_RA_MAG_ZOUT_L);
}

double getKalman(Kalman* kal, double acc, double gyro, double dt)
{
  kal->angle += dt * (gyro - kal->bias);

  kal->P[0][0] += dt * (dt * kal->P[1][1] - kal->P[0][1] - kal->P[1][0] * kal->Q_Angle);
  kal->P[0][1] -= dt * kal->P[1][1];
  kal->P[1][0] -= dt * kal->P[1][1];
  kal->P[1][1] += kal->Q_Gyro * dt;

  double S = kal->P[0][0] + kal->R_Measure;
  kal->K[0] = kal->P[0][0] / S;
  kal->K[1] = kal->P[1][0] / S;

  double y = acc - kal->angle;
  kal->angle += kal->K[0] * y;
  kal->bias += kal->K[1] * y;

  double P_temp[2] = {kal->P[0][0], kal->P[0][1]};
  kal->P[0][0] -= kal->K[0] * P_temp[0];
  kal->P[0][1] -= kal->K[0] * P_temp[1];
  kal->P[1][0] -= kal->K[1] * P_temp[0];
  kal->P[1][1] -= kal->K[1] * P_temp[1];

  return kal->angle;
}

void kalman_Init(Kalman* kal, double angle_f, double bias_f, double measure)
{
  kal->Q_Angle = angle_f;
  kal->Q_Gyro = bias_f;
  kal->R_Measure = measure;

  kal->angle = 0;
  kal->bias = 0;

  kal->P[0][0] = 0;
  kal->P[0][1] = 0;
  kal->P[1][0] = 0;
  kal->P[1][1] = 0;
}

double getComple(double accAngle, double gyroAngle, double dt)
{
  double accelDegree = accAngle;
  return accelDegree;
}

void PID_Control_Initialize(DPID* dst, double gain_P, double gain_I, double gain_D)
{
  dst->errorSum = 0.0;
  dst->errorSumLimit = 10000.0;
  dst->outerP = 1.0;
  dst->kP = gain_P;
  dst->kI = gain_I;
  dst->kD = gain_D;
  dst->pastAngleError = 0.0;
  dst->underOfPoint = 5000.0;
  dst->outputLimit = 8000.0;
}

void PID_Control(DPID* dst, double target, double input, double Gyro_V)
{
  // Angle P control
  dst->targetAngle = target;
  dst->nowAngle = input;
  dst->angleError = dst->targetAngle - dst->nowAngle;
  
  // Angular Velocity Control
  // P Control
  dst->nowASError = (dst->outerP * dst->angleError) - Gyro_V;
  // I Control
  dst->asErrorSum += dst->nowASError;
  // D Control
  dst->asErrorDiff = dst->nowASError - dst->pastASError;
  
  if(dst->errorSumLimit != 0)
  {
    if(dst->asErrorSum > dst->errorSumLimit)
    {
      dst->asErrorSum = dst->errorSumLimit;
    }
    else if(dst->asErrorSum < -dst->errorSumLimit)
    {
      dst->asErrorSum = -dst->errorSumLimit;
    }
  }
  
  dst->nowOutput = (long)(dst->kP * dst->nowASError) +
                   (long)(dst->kI * dst->asErrorSum) +
                   (long)(dst->kD * dst->asErrorDiff);
  
  if(dst->underOfPoint == 0)
    return;
  
  dst->nowOutput /= dst->underOfPoint;
  
  dst->pastAngle = dst->nowAngle;
  dst->pastAngleError = dst->angleError;
  dst->pastASError = dst->nowASError;
  
  if(dst->outputLimit != 0)
  {
    if(dst->nowOutput > dst->outputLimit)
      dst->nowOutput = dst->outputLimit;
    else if(dst->nowOutput < -dst->outputLimit)
      dst->nowOutput = -dst->outputLimit;
  }
}

void Angular_Control(double target_X, double target_Y)
{
  PID_Control(&attitude_X, target_X, Angle_X, Gyro_X);
  PID_Control(&attitude_Y, target_Y, Angle_Y, Gyro_Y);
    
  motorPWM1 = ((motorValue - ((double)attitude_Y.nowOutput * 0.5)) * 0.5) + ((motorValue + ((double)attitude_X.nowOutput * 0.5)) * 0.5);
  motorPWM3 = ((motorValue + ((double)attitude_Y.nowOutput * 0.5)) * 0.5) + ((motorValue + ((double)attitude_X.nowOutput * 0.5)) * 0.5);
  motorPWM2 = ((motorValue + ((double)attitude_Y.nowOutput * 0.5)) * 0.5) + ((motorValue - ((double)attitude_X.nowOutput * 0.5)) * 0.5);
  motorPWM4 = ((motorValue - ((double)attitude_Y.nowOutput * 0.5)) * 0.5) + ((motorValue - ((double)attitude_X.nowOutput * 0.5)) * 0.5);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
    test = 404;
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
