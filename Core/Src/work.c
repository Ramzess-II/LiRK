#include "work.h"
//----------------------- переменные из других файлов --------------------------------//
extern DMA_HandleTypeDef handle_GPDMA2_Channel0;
extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef handle_GPDMA1_Channel0;
extern UART_HandleTypeDef huart1;
extern uint8_t dataNotEmpty, outVoltage;
extern uint32_t lenght;
extern uint32_t *ptrBufTime;
extern uint32_t *ptrbufGpio;
//----------------------- переменные из этого файла ----------------------------------//
uint32_t ledblinkTime = 0;
//------------------------------ функции ---------------------------------------------//
void doWork (void) {
	  init_uart();                                                                                        // запускаем прием
	  HAL_DMA_RegisterCallback(&handle_GPDMA2_Channel0, HAL_DMA_XFER_CPLT_CB_ID, MyDMACpltCallback);      // Регистрация колбека
	  HAL_GPIO_WritePin(voltage3_3_GPIO_Port, voltage3_3_Pin, GPIO_PIN_SET);                              // по умолчанию 3.3В
	  outVoltage = VOLTAGE3_3;
}

void work (void) {
	  blinkLed();                                          // моргаем светиком
	  check_timeout ();                                    // проверим таймауты
	  checkButton ();                                      // проверим кнопку
	  switch (parsingData()) {                             // проверим новые данные
	  case DATA_OK:                                        // после принятых данных ответить
		 sendOk();
	  break;
	  case RECIVE_START:
		 sendOk();                                         // ответим по юарту
		 HAL_Delay(500);                                   // костыль но пока так, подождать пока работает юарт, чтоб не выйти из сна от передачи данных
		 startprogramm();                                  // стартуем выполнять цикл
	  break;
	  }
}

void blinkLed (void) {
	if (HAL_GetTick() - ledblinkTime >= BLINK_TIME) {      // моргание без делея
		HAL_GPIO_TogglePin(led1_GPIO_Port, led1_Pin);
		ledblinkTime = HAL_GetTick();
	}
}

void MyDMACpltCallback(DMA_HandleTypeDef *hdma) {          					 // Обработать завершение передачи DMA
	if (hdma->Instance == GPDMA2_Channel0) {
		HAL_ResumeTick();                                  					 // возобновим таймер
		HAL_TIM_Base_Stop_DMA(&htim2);                     					 // остановим дма, без этого не хотело нормально работать
		HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_RESET);         // погасим светик
	}
}

void startprogramm (void) {
	uint32_t bsrrAddress = (uint32_t)&(GPIOB->BSRR);                                                          // адрес типа в памяти куда нужно передать данные по дма
	HAL_GPIO_WritePin(led2_GPIO_Port, led2_Pin, GPIO_PIN_SET);                                                // включим светик на момент выполнения
	if (outVoltage == VOLTAGE3_3) {
		HAL_GPIO_WritePin(voltage5_GPIO_Port, voltage5_Pin, GPIO_PIN_RESET);                                  // сбросим 5В
		HAL_GPIO_WritePin(voltage3_3_GPIO_Port, voltage3_3_Pin, GPIO_PIN_SET);                                // включим 3.3В
	} else if (outVoltage == VOLTAGE5) {
		HAL_GPIO_WritePin(voltage3_3_GPIO_Port, voltage3_3_Pin, GPIO_PIN_RESET);                              // сбросим 3.3В
		HAL_GPIO_WritePin(voltage5_GPIO_Port, voltage5_Pin, GPIO_PIN_SET);                                    // включим 5В
	}
	TIM2->ARR = *ptrBufTime;                                                                                  // напрямую разыменовываем первый элемент буффера
	GPIOB->BSRR = *ptrbufGpio;                                                                                // и напрямую выставим состояния ног из буффера
	HAL_TIM_Base_Start_DMA(&htim2, ptrBufTime + 1, ((lenght - 4) - 8));                                       // передавать количество байт а не слов! -8 по тому что 4 вначале не используем и на 4 меньше чем gpio
	HAL_DMA_Start_IT(&handle_GPDMA2_Channel0, (uint32_t)(ptrbufGpio + 1), bsrrAddress, ((lenght - 4) - 4));   // запуск дма на передачу по триггеру от таймера должно быть на 1 больше чем время но так как в 4 в начале то 4 отнимаем
// костыль переделать
	HAL_SuspendTick();                                                                                        // остановим систик
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);                                         // упадем в сон чтоб не мешать дма
}

void checkButton (void) {
	if ((!HAL_GPIO_ReadPin(keyStart_GPIO_Port, keyStart_Pin)) &&  dataNotEmpty) {                             // если нажали кнопку и данные уже загружены
		 startprogramm();                                                                                     // стартуем выполнять цикл
	}
}
