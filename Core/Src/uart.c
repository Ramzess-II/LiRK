#include "uart.h"
//----------------------- переменные из других файлов --------------------------------//
extern UART_HandleTypeDef huart1;
extern uint32_t *ptrBufTime;    // указатель на буфер
extern uint32_t *ptrbufGpio;
//----------------------- переменные из этого файла ----------------------------------//
uint32_t bufTime[BUF_SIZE / 4];        // Буфер для данных (32 бита)
uint32_t bufGpio[BUF_SIZE / 4];        // Буфер для данных (32 бита)
uint32_t *ptrBufTime = bufTime;        // указатель на буфер
uint32_t *ptrbufGpio = bufGpio;
uint8_t txBufUart [10] = { 0 };
uint8_t rxBufUart [10] = { 0 };
uint8_t calbackOk, timeoutStart = 0;
uint8_t uartProcess = WAIT_DATA;
uint32_t start_time = 0;  			// Для отслеживания тайм-аута
uint32_t lenght = 0;

//------------------------------ функции ---------------------------------------------//

void init_uart(void) {
    HAL_UART_Receive_IT(&huart1, rxBufUart, 4);     // Начальный прием заголовка
}

void sendOk (void) {                                // ответить ок
	txBufUart [0] = 0xAA;
	txBufUart [1] = 0xBB;
	HAL_UART_Transmit_IT(&huart1, txBufUart, 2);    // отправка по прерываниям
}

uint32_t parsingData (void) {                                                                     // парсинг данных
	if (calbackOk) {                                                                              // если сработал колбек
		calbackOk = 0;																			  // сбросим его
		if (uartProcess == WAIT_DATA) {															  // если ожидание данных было
			if (rxBufUart[0] == 0xAA ){                                                           // проверим нулевой байт
				if (rxBufUart[1] == 0x55 || rxBufUart[1] == 0x33) {                               // проверим первый байт это данные или команда
					timeoutStart = 0;                                                             // тут сбросим второй тайм аут, который нужен для приема первых 4 байт
					lenght = rxBufUart [2] << 8;                                                  // соберем длительность
					lenght |= rxBufUart [3];
					if (lenght > BUF_SIZE) {                                                      // если она больше буфера
						HAL_UART_Receive_IT(&huart1, rxBufUart, 4);                               // на новый захват
						return ENTRY;                                                             // выйдем
					}
					start_time = HAL_GetTick();                                                   // если не вышли взводим второй таймаут на большую посылку
				}
				switch (rxBufUart[1]) {                                                           // в зависимиости от данных во втором байте выполняем действия
				case 0x55:
					HAL_UART_Receive_IT(&huart1, (uint8_t*) ptrBufTime, lenght - 4);              // в буфер времени принимаем нужное количество
					uartProcess = DATA_RECIVE;                                                    // ставим статус приема данных
					return DATA_RECIVE;
				break;
				case 0x33:
					HAL_UART_Receive_IT(&huart1, (uint8_t*) ptrbufGpio, lenght - 4);              // тоже самое но в другой буфер
					uartProcess = DATA_RECIVE;
					return DATA_RECIVE;
				break;
				case 0x44:                                                                        // это команда старта
					HAL_UART_Receive_IT(&huart1, rxBufUart, 4);              				      // Перезапуск приема заголовка
					return RECIVE_START;
				break;
				}
			} else {
				HAL_UART_Receive_IT(&huart1, rxBufUart, 4);              						  // Перезапуск приема заголовка
			}
		} else if (uartProcess == DATA_RECIVE){                                                   // если принимали данные
			HAL_UART_Receive_IT(&huart1, rxBufUart, 4);              						      // Перезапуск приема заголовка
			uartProcess = WAIT_DATA;                                                              // переключимся в прием заголовка
			return DATA_OK;
		}
	}
	return ENTRY;
}

void check_timeout(void) {                                                             // таймауты для юарта
	if (!timeoutStart && huart1.RxXferCount < 4 ) {                                    // это если первые 4 байта по какой то причине долго идут
		start_time = HAL_GetTick();                                                    // захват времени
		timeoutStart = 1;                                                              // взводим флаг
	}
	if (timeoutStart && (HAL_GetTick() - start_time >= 5)) {                           // 5 при скорости 115200 может передаться 500 бит
		timeoutStart = 0;                                                              // сбросим флаг
    	HAL_UART_AbortReceive_IT(&huart1);                                             // сбросим прием
        HAL_UART_Receive_IT(&huart1, rxBufUart, 4);   			                       // Перезапуск приема заголовка
	}
    if (uartProcess == DATA_RECIVE && (HAL_GetTick() - start_time >= TIMEOUT_MS)) {    // а это уже для приема большого буфера
    	uartProcess = WAIT_DATA;                                                       // вернемся в прием первых 4 байт
    	HAL_UART_AbortReceive_IT(&huart1);                                             // сбросим прием
        HAL_UART_Receive_IT(&huart1, rxBufUart, 4);   			                       // Перезапуск приема заголовка
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
    	calbackOk = 1;                                                                 // скажем что колбек сработал
    }
}
