#include <dinput.h>
#include "pang.h"


MINPUT::MINPUT()
{
	ZeroMemory(buffer, sizeof(buffer));
}

MINPUT::~MINPUT()
{
	if(lpDIDK) lpDIDK->Release();
}

/////////////////////////////////////////////
// InitInput
// Funcion que inicia DirectInput
/////////////////////////////////////////////
BOOL MINPUT::Init(HINSTANCE hInst, HWND hwnd)
{
	if ( DirectInput8Create (hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpDI, NULL)==DI_OK)
		if(InitKeyboard(hwnd)) return TRUE;
	return FALSE;
}

/////////////////////////////////////////////
//Funcion que inicia el teclado
/////////////////////////////////////////////
BOOL MINPUT::InitKeyboard (HWND hwnd)
{
	if (lpDI->CreateDevice(GUID_SysKeyboard, &lpDIDK, NULL)!=DI_OK)
	{
		return FALSE;
	}

	if (lpDIDK->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)!=DI_OK)
	{
		lpDIDK->Release();
		return FALSE;
	}

	if (lpDIDK->SetDataFormat(&c_dfDIKeyboard)!=DI_OK)
	{
		lpDIDK->Release();
		return FALSE;
	}

	if (lpDIDK->Acquire()!=DI_OK)
	{
		lpDIDK->Release();
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////
//  ReadKeyboard()
//
//  Lee el buffer del teclado y lo guarda en un buffer nuestro
/////////////////////////////////////////////////////////////
BOOL MINPUT::ReadKeyboard()
{
	 HRESULT  hr;

   hr = lpDIDK->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 

   if (hr!=DI_OK)
         return FALSE;
	 return TRUE;
}

//////////////////////////////////////////////////////////////
//  Key()
//
//  comprueba si la tecla <k> esta en el buffer del teclado
/////////////////////////////////////////////////////////////
BOOL MINPUT::Key(UINT k)
{
	return buffer[k] & 0x80;
}


//////////////////////////////////////////////////////////////
//  ReacquireInput()
//
//  Readquiere el dispositivo de entrada actual. Si la funcion
//	Acquire devuelve S_FALSE significa que el dispositivo ya esta
//	adquirido y no hace falta readquirirlo.
/////////////////////////////////////////////////////////////
BOOL MINPUT::ReacquireInput(void)
{
    HRESULT hRes;

	if(lpDIDK)
	{
		 // adquirirmos el dispositivo	
		hRes = IDirectInputDevice_Acquire(lpDIDK);
		if(SUCCEEDED(hRes))		
		   return TRUE;
		else

		   return FALSE;
	}

}
