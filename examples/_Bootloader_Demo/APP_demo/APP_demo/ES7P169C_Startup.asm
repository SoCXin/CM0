#include "ES7P169C.inc"

EXTERN	_main
EXTERN  _isr

APP_ENTRY EQU 0x0800		;�û�������ڵ�ַ

_STARTUP	CSEG 0X00	
	AJMP	APP_ENTRY
	NOP
	NOP
	PUSH
	AJMP	APP_ENTRY+4		;jump to the entry of interrupt function

_APP_STARTUP CSEG APP_ENTRY
	;SENGMENTSEL $
	CALL	 _main
	GOTO	APP_ENTRY
END