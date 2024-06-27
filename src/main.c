/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdlib.h> 

/** @addtogroup STM32F3_Discovery_Peripheral_Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;
__IO uint32_t TimingDelay = 0;
__IO uint32_t UserButtonPressed = 0;
uint8_t currentState = 0;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);
void ToggleLEDsInCircle(void);
void AllLEDsOff(void);
void ToggleLEDsHalfPattern(void);
void ToggleLEDsInCirclePattern(void);
void ToggleRandomLEDs(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  /* Initialize LEDs and User Button on STM32F3-Discovery board */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  STM_EVAL_LEDInit(LED7);
  STM_EVAL_LEDInit(LED8);
  STM_EVAL_LEDInit(LED9);
  STM_EVAL_LEDInit(LED10);
  
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

  /* Reset UserButton_Pressed variable */
  UserButtonPressed = 0x00; 

  /* Infinite loop */
  while (1)
  {
    switch (currentState)
    {
      case 0:
        AllLEDsOff();
        break;
      case 1:
        ToggleLEDsInCircle();
        break;
      case 2:
        ToggleLEDsHalfPattern();
        break;
      case 3:
        ToggleLEDsInCirclePattern();
        break;
      case 4:
        ToggleRandomLEDs();
        break;
    }

    /* Check if the user button is pressed */
    if (UserButtonPressed == 0x01)
    {
      /* Reset the UserButtonPressed variable */
      UserButtonPressed = 0x00;
      
      /* Change the state */
      currentState = (currentState + 1) % 5;  // 5 states 
    }
  }
}

void AllLEDsOff(void)
{
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
  STM_EVAL_LEDOff(LED5);
  STM_EVAL_LEDOff(LED6);
  STM_EVAL_LEDOff(LED7);
  STM_EVAL_LEDOff(LED8);
  STM_EVAL_LEDOff(LED9);
  STM_EVAL_LEDOff(LED10);
}

void ToggleLEDsInCircle(void)
{
  /* LEDs Off */
  AllLEDsOff();

  /* Toggle LEDs in circle */
  STM_EVAL_LEDToggle(LED3);
  Delay(25);
  STM_EVAL_LEDToggle(LED5);
  Delay(25);
  STM_EVAL_LEDToggle(LED7);
  Delay(25);
  STM_EVAL_LEDToggle(LED9);
  Delay(25);
  STM_EVAL_LEDToggle(LED10);
  Delay(25);
  STM_EVAL_LEDToggle(LED8);
  Delay(25);
  STM_EVAL_LEDToggle(LED6);
  Delay(25);
  STM_EVAL_LEDToggle(LED4);
  Delay(25);
}

void ToggleLEDsHalfPattern(void)
{
  AllLEDsOff();

  /* Turn on LEDs at positions 3, 5, 7, 9 */
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED5);
  STM_EVAL_LEDOn(LED7);
  STM_EVAL_LEDOn(LED9);
  
  Delay(100);

  /* Turn off LEDs at positions 3, 5, 7, 9 */
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED5);
  STM_EVAL_LEDOff(LED7);
  STM_EVAL_LEDOff(LED9);

  /* Turn on LEDs at positions 4, 6, 8, 10 */
  STM_EVAL_LEDOn(LED4);
  STM_EVAL_LEDOn(LED6);
  STM_EVAL_LEDOn(LED8);
  STM_EVAL_LEDOn(LED10);
  
  Delay(100);

  /* Turn off LEDs at positions 4, 6, 8, 10 */
  STM_EVAL_LEDOff(LED4);
  STM_EVAL_LEDOff(LED6);
  STM_EVAL_LEDOff(LED8);
  STM_EVAL_LEDOff(LED10);
}

void ToggleLEDsInCirclePattern(void)
{
  AllLEDsOff();

  /* Toggle every third LED in circle */
  STM_EVAL_LEDToggle(LED3);
  Delay(25);
  STM_EVAL_LEDToggle(LED7);
  Delay(25);
  STM_EVAL_LEDToggle(LED10);
  Delay(25);
  STM_EVAL_LEDToggle(LED6);
  Delay(25);
  
  AllLEDsOff();
  Delay(25);

  /* Toggle every second LED in circle */
  STM_EVAL_LEDToggle(LED5);
  Delay(25);
  STM_EVAL_LEDToggle(LED9);
  Delay(25);
  STM_EVAL_LEDToggle(LED8);
  Delay(25);
  STM_EVAL_LEDToggle(LED4);
  Delay(25);
}

void ToggleRandomLEDs(void)
{
  AllLEDsOff();

  /* Toggle random LEDs */
  for (int i = 0; i < 4; ++i) // Toggle 4 random LEDs
  {
    int randomIndex = rand() % 8 + 3; // from 3 to 10 (LED3 to LED10)
    STM_EVAL_LEDToggle(randomIndex);
    Delay(10); 
  }
}

void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void EXTI0_IRQHandler(void)
{
  if (EXTI_GetITStatus(USER_BUTTON_EXTI_LINE) != RESET)
  {
    /* Set the UserButtonPressed variable */
    UserButtonPressed = 0x01;

    /* Clear the EXTI line pending bit */
    EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
