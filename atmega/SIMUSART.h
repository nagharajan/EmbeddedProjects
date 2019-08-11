
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>

#define SBAUDRATE 9600
#define SBAUD_PRESCALLER (((F_CPU / (SBAUDRATE * 16UL))) - 1)

void SIMUSART_init(void){

	UBRR2H = (unsigned char)(SBAUD_PRESCALLER>>8);
	UBRR2L = (unsigned char)(SBAUD_PRESCALLER);
	
	UCSR2B = (1<<RXEN2)|(1<<TXEN2)|(1<<RXCIE2);
	UCSR2C = (1<<USBS2) | (3<<UCSZ20) ;
	sei();
}

unsigned char SIMUSART_receive(void){

	while(!(UCSR2A & (1<<RXC2)));
	return UDR2;

}

void SIMUSART_send( unsigned char data){
	
	while(!(UCSR2A & (1<<UDRE2)));
	UDR2 = data;

}

void SIMUSART_putstring(char* StringPtr){
	memset(buff,0,strlen(buff));
	buffer_pointer=0;
	while(*StringPtr != 0x00){
		SIMUSART_send(*StringPtr);
	StringPtr++;}
}
