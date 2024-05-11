#include "pangmap.h"
#include "resource.h"
//#include "pang.h"


void PANGMAP::New()
{
	//lpfnDIALOGSMsgProc = MakeProcInstance((FARPROC)DIALOG1MsgProc, hInst);
      if(!IsWindow(hToolDialog))
	  tools = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWndMain, (DLGPROC) DIALOG1MsgProc);
	  else
		  SetFocus(tools);
}