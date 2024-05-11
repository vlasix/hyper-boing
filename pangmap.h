#ifndef _PANGMAP_H_
#define _PANGMAP_H_

#include <windows.h>

extern BOOL FAR PASCAL DIALOG1MsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam);

extern HINSTANCE hInst;
extern HWND hWndMain;
extern FARPROC lpfnDIALOGSMsgProc;

class PANGMAP
{
public:
	
	HWND tools;

	int nballs; //numero de bolas que aparecen
	int nitems; // numero de items que aparecen
	int nfloor; //numero de cachos de suelo


	void New();
};

#endif