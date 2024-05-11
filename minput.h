#ifndef MINPUT_H_
#define MINPUT_H_

#include "dinput.h"

#define KEYDOWN(name,key) (name[key] & 0x80)  


/********************************************************
 clase MINPUT

  clase que inicializa la libreria DirectInput (DX)
  y realiza consultas del buffer del teclado.
********************************************************/

class MINPUT
{
	char buffer[256];
	LPDIRECTINPUT lpDI;  //Objeto DirectInput
	LPDIRECTINPUTDEVICE lpDIDK; //Dispositivo DirectInput para el Keyboard

public:
	MINPUT();
	~MINPUT();
	BOOL Init(HINSTANCE hInst, HWND hwnd);
	BOOL InitKeyboard (HWND hwnd);
	BOOL ReadKeyboard();
	BOOL Key(UINT k);
	BOOL ReacquireInput(void);
};

#endif

