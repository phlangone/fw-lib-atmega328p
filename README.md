# fw-lib-atmega328p
Bibliotecas HAL para ATmega328P

Para incluir a biblioteca no Atmel Studio selecione:

Disco local C > Arquivos de Programas (x86) > Atmel > Studio > 7.0 > toolchain > avr8 > avr8-gnu-toolchain > avr > include > E crie uma nova pasta para incluir os arquivos deste diretório.

Exemplo: Você criou uma pasta chamada "user_libs", na folha da aplicação basta incluir <user_libs/arquivo_exemplo.h>

Folhas já implementadas:

* usart.h ---> Funções para utilização do periférico USART do ATmega328P
  
* adc.h   ---> Funções para utilização do periférico ADC do ATmega328P

* gpio.h ---> Funções para utilização dos GPIOs do ATmega328P

* lcd.h ---> Funções para utilização do display LCD baseado no driver HD44780
