#ifndef INC_WORK_H_
#define INC_WORK_H_

#ifdef __cplusplus
extern "C" {
#endif

//----------------------- подключим файлы ------------------------------------//
#include "stm32h5xx_hal.h"
#include "uart.h"
//----------------------- дефайним значения ----------------------------------//
#define BLINK_TIME         1000

#define out0_Pin GPIO_PIN_0
#define out0_GPIO_Port GPIOB
#define out1_Pin GPIO_PIN_1
#define out1_GPIO_Port GPIOB
#define out2_Pin GPIO_PIN_2
#define out2_GPIO_Port GPIOB
#define led_Pin GPIO_PIN_8
#define led_GPIO_Port GPIOA
#define out3_Pin GPIO_PIN_3
#define out3_GPIO_Port GPIOB
#define out4_Pin GPIO_PIN_4
#define out4_GPIO_Port GPIOB
#define out5_Pin GPIO_PIN_5
#define out5_GPIO_Port GPIOB
#define out6_Pin GPIO_PIN_6
#define out6_GPIO_Port GPIOB
#define out7_Pin GPIO_PIN_7
#define out7_GPIO_Port GPIOB

//----------------------- объявим функции ------------------------------------//
void doWork (void);
void work (void);
void blinkLed (void);
void startprogramm (void);
void MyDMACpltCallback(DMA_HandleTypeDef *hdma);
//----------------------- объявим структуры ----------------------------------//



#ifdef __cplusplus
}
#endif

#endif /* INC_WORK_H_ */
