#include "pang.h"


BOOL CALLBACK InitDlgMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam)
{
	static int mode = -1;

 switch(Message)
   {
    case WM_CLOSE:
         PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
         break;

    case WM_COMMAND:
         switch(wParam)
           {         
            case IDC_RADIO1:
					mode = RENDERMODE_NORMAL;					
				break;

			case IDC_RADIO2:
					mode = RENDERMODE_EXCLUSIVE;
				break;
			case IDOK:
				 if(mode < 0)
					 MessageBox(hWndDlg, "Debe seleccionar una opcion", "ERROR!", MB_OK);
				 else
				 {
					 globalmode = mode;				  
					// inicio del modo grafico				
					EndDialog(hWndDlg, TRUE);
				 }
			break;

			case IDCANCEL:
				EndDialog(hWndDlg, FALSE);
				break;
				
           }
         break;    /* Fin de WM_COMMAND                                 */

    default:
        return FALSE;
   }
 return TRUE;
}