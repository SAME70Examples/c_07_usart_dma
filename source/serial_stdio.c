#include "serial_stdio.h"

void serial_puts(void (*p_putc)(char), const char * pString){
	char newChar;
	int i ;
	for(i = 0 ; i < 81; i++){
		newChar = pString[i];
		if( newChar != '\0' ){
			p_putc(newChar);
		}else{
			break;
		}
	}
}

void serial_printf(void (*p_puts)(const char *), const char * format, ...){
	char * tempBuffer;
	int bufferSize;

	va_list args;
	va_start(args, format);

	bufferSize = vsnprintf(NULL, 0,format, args);
	bufferSize++;
	tempBuffer = (char*) malloc(bufferSize);
	vsnprintf(tempBuffer,bufferSize,format,args);

	va_end(args);

	p_puts(tempBuffer);
	free(tempBuffer);
}
