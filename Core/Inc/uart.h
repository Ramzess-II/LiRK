#ifndef INC_UART_H_
#define INC_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

//----------------------- подключим файлы ------------------------------------//
#include "stm32h5xx_hal.h"

//----------------------- дефайним значения ----------------------------------//
#define ENTRY         0
#define WAIT_DATA     1
#define DATA_RECIVE   2
#define DATA_OK       3
#define RECIVE_START  4

#define BUF_SIZE      12300
#define TIMEOUT_MS    2000 			   // Тайм-аут в миллисекундах 2000 за это время передадим 230 000 тыс бит

//----------------------- объявим функции ------------------------------------//
void init_uart(void);
void sendOk (void);
uint32_t parsingData (void);
void check_timeout(void);

//----------------------- объявим структуры ----------------------------------//



#ifdef __cplusplus
}
#endif



#endif /* INC_UART_H_ */
