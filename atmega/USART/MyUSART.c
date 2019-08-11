#include "MyUSART.h"


void USART_init(void){

	UBRR0H = (unsigned char)(BAUD_PRESCALLER>>8);
	UBRR0L = (unsigned char)(BAUD_PRESCALLER);
	
	//UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C = (1<<USBS0) | (3<<UCSZ00) ;	
	sei();
}

unsigned char USART_receive(void){

	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;

}

void USART_send( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;

}

void USART_putstring(char* StringPtr){
	memset(buff,0,strlen(buff));
	buffer_pointer=0;	
	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
	StringPtr++;}
}