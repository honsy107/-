#include "main.h"

#define DS18B20_Port GPIOA
#define DS18B20_Pin  GPIO_PIN_10


void __PORT_SET(int cs);
int MY_DS18B20_RESET(void);
void MY_DS18B20_SEND(uint8_t data);
uint8_t MY_DS18B20_READ(void);
float MY_DS18B20_TR(void);
