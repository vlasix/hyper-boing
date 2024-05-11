//#include "pang.h"
#include <windows.h>

extern HINSTANCE hInst;
extern HWND hWndMain;
extern HWND hToolDialog;


BOOL FAR PASCAL DIALOG1MsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam)
{

 switch(Message)
   {
    case WM_CLOSE:
         PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
         break; 

    case WM_COMMAND:
         switch(wParam)
           {
            case 202: /* Edit Control                                   */
                 break;

            case IDOK:
                 EndDialog(hWndDlg, TRUE);
                 break;
           }
         break;                                */

    default:
        return FALSE;
   }
 return TRUE;
}