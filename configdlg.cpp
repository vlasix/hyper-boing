//#include <windows.h>
//#include "resource.h"
#include "pang.h"



#define NUMKEYS 98

extern HINSTANCE hInst;
extern HWND hWndMain;
extern HWND hConfigDialog;
extern PAPP *screen;

void SelectComboText(HWND hwnd, int item, char *text);

/********************************************************
  clase KEYTABLE

  Esta clase contiene informacion con todas las teclas disponibles
  y configurables para el juego, con su equivalente en VIRTUAL KEYS
  y su equivalente en TEXTO.

  La idea era que se pudieran configurar las teclas ya sea
  seleccionandolas de un ComboBox, o bien mediante la pulsacion
  de una tecla del teclado, pero por lo visto los Dialogos no
  reciben mensajes de WM_CHAR o WM_KEYDOWN.
********************************************************/
class KEYTABLE
{
public:
	UINT dik[NUMKEYS];
	int vk[NUMKEYS];
	char kname[NUMKEYS][16];

	int GetDIKeyText(UINT param, char *text);
	char * GetDIKeyText(UINT param);
	int GetVrtKeyText(int param, char *text);
	char * GetVrtKeyText(int param);
	char * GetText(int index);

	UINT GetDIK(char *text);	
	
	UINT ToDI(int vkey);
	int ToVK(UINT dikey);

	KEYTABLE();
};


BOOL CALLBACK ConfigDlgMsgProc(HWND hWndDlg, WORD Message, WORD wParam, LONG lParam)
{
	int res;
	KEYTABLE keytbl;
	char keys1[3][32], keys2[3][32];	

 switch(Message)
   {
    case WM_INITDIALOG:
		int i, index;

        res = keytbl.GetDIKeyText(gameinf.keys[PLAYER1].left, keys1[0]);	
		res = keytbl.GetDIKeyText(gameinf.keys[PLAYER1].right, keys1[1]);
		res = keytbl.GetDIKeyText(gameinf.keys[PLAYER1].shoot, keys1[2]);
		res = keytbl.GetDIKeyText(gameinf.keys[PLAYER2].left, keys2[0]);	
		res = keytbl.GetDIKeyText(gameinf.keys[PLAYER2].right, keys2[1]);
		res = keytbl.GetDIKeyText(gameinf.keys[PLAYER2].shoot, keys2[2]);
		for(i=0; i<NUMKEYS; i++)
		{
			SendDlgItemMessage(hWndDlg, IDC_KEYCONF1_LEFT, CB_ADDSTRING,
                                 0,  (LPARAM) (LPCTSTR) keytbl.GetText(i)) ;
			SendDlgItemMessage(hWndDlg, IDC_KEYCONF1_RIGHT, CB_ADDSTRING,
                                 0,  (LPARAM) (LPCTSTR) keytbl.GetText(i)) ;
			SendDlgItemMessage(hWndDlg, IDC_KEYCONF1_SHOOT, CB_ADDSTRING,
                                 0,  (LPARAM) (LPCTSTR) keytbl.GetText(i)) ;
			SendDlgItemMessage(hWndDlg, IDC_KEYCONF2_LEFT, CB_ADDSTRING,
                                 0,  (LPARAM) (LPCTSTR) keytbl.GetText(i)) ;
			SendDlgItemMessage(hWndDlg, IDC_KEYCONF2_RIGHT, CB_ADDSTRING,
                                 0,  (LPARAM) (LPCTSTR) keytbl.GetText(i)) ;
			SendDlgItemMessage(hWndDlg, IDC_KEYCONF2_SHOOT, CB_ADDSTRING,
                                 0,  (LPARAM) (LPCTSTR) keytbl.GetText(i)) ;
		}

		SelectComboText(hWndDlg, IDC_KEYCONF1_LEFT, keys1[0]);
		SelectComboText(hWndDlg, IDC_KEYCONF1_RIGHT, keys1[1]);
		SelectComboText(hWndDlg, IDC_KEYCONF1_SHOOT, keys1[2]);
		SelectComboText(hWndDlg, IDC_KEYCONF2_LEFT, keys2[0]);
		SelectComboText(hWndDlg, IDC_KEYCONF2_RIGHT, keys2[1]);
		SelectComboText(hWndDlg, IDC_KEYCONF2_SHOOT, keys2[2]);
         break; /* Fin de WM_INITDIALOG */

    case WM_CLOSE:
         /* Cerrar el dialogo es lo mismo que cancelar  */
         PostMessage(hWndDlg, WM_COMMAND, IDCANCEL, 0L);
         break;

    case WM_COMMAND:
         switch(wParam)
           {
            case IDOK:
				 screen->SetPause(FALSE);
				 GetDlgItemText(hWndDlg, IDC_KEYCONF1_LEFT, keys1[0], 256);
				 GetDlgItemText(hWndDlg, IDC_KEYCONF1_RIGHT, keys1[1], 256);
				 GetDlgItemText(hWndDlg, IDC_KEYCONF1_SHOOT, keys1[2], 256);
				 gameinf.keys[PLAYER1].SetLeft(keytbl.GetDIK(keys1[0]));
				 gameinf.keys[PLAYER1].SetRight(keytbl.GetDIK(keys1[1]));
				 gameinf.keys[PLAYER1].SetShoot(keytbl.GetDIK(keys1[2]));
				 GetDlgItemText(hWndDlg, IDC_KEYCONF2_LEFT, keys2[0], 256);
				 GetDlgItemText(hWndDlg, IDC_KEYCONF2_RIGHT, keys2[1], 256);
				 GetDlgItemText(hWndDlg, IDC_KEYCONF2_SHOOT, keys2[2], 256);
				 gameinf.keys[PLAYER2].SetLeft(keytbl.GetDIK(keys2[0]));
				 gameinf.keys[PLAYER2].SetRight(keytbl.GetDIK(keys2[1]));
				 gameinf.keys[PLAYER2].SetShoot(keytbl.GetDIK(keys2[2]));				
				 
                 EndDialog(hWndDlg, TRUE);
                 break;

			case IDCANCEL:
				screen->SetPause(FALSE);
				EndDialog(hWndDlg, TRUE);
			break;
				
           }
         break;

    default:
        return FALSE;
   }
 return TRUE;
}


void SelectComboText(HWND hwnd, int item, char *text)
{
	int index;
	
	
	index=SendDlgItemMessage(hwnd, item, CB_FINDSTRINGEXACT, -1, (LPARAM)(LPCSTR) text);
	if ( index != LB_ERR )
		SendDlgItemMessage(hwnd, item, CB_SETCURSEL,
                                 index,  0) ;
		
}



/**************************************************************************/


/**********************************************************
	CONSTRUCTOR

  Inicia las tablas de equivalencias de teclas.
***********************************************************/


KEYTABLE::KEYTABLE()
{
	dik[0] = DIK_ESCAPE; vk[0]=  VK_ESCAPE; strcpy(kname[0], "ESCAPE");
	dik[1] = DIK_1; vk[1] = '1'; strcpy(kname[1], "1");
	dik[2] = DIK_2; vk[2] = '2'; strcpy(kname[2], "2");
	dik[3] = DIK_3; vk[3] = '3'; strcpy(kname[3], "3");
	dik[4] = DIK_4; vk[4] = '4'; strcpy(kname[4], "4");
	dik[5] = DIK_5; vk[5] = '5'; strcpy(kname[5], "5");
	dik[6] = DIK_6; vk[6] = '6'; strcpy(kname[6], "6");
	dik[7] = DIK_7; vk[7] = '7'; strcpy(kname[7], "7");
	dik[8] = DIK_8; vk[8] = '8'; strcpy(kname[8], "8");
	dik[9] = DIK_9; vk[9] = '9'; strcpy(kname[9], "9");
	dik[10] = DIK_0; vk[10] = '0'; strcpy(kname[10], "0");
	dik[11] = DIK_MINUS; vk[11] = VK_SUBTRACT; strcpy(kname[11], "tecla -");
	dik[12] = DIK_EQUALS; vk[12] = 0; strcpy(kname[12], "");
	dik[13] = DIK_BACK; vk[13] = VK_BACK; strcpy(kname[13], "BORRAR");
	dik[14] = DIK_TAB; vk[14] = VK_TAB; strcpy(kname[14], "TAB");
	dik[15] = DIK_Q; vk[15] = 'Q'; strcpy(kname[15], "Q");
	dik[16] = DIK_W; vk[16] = 'W'; strcpy(kname[16], "W");
	dik[17] = DIK_E; vk[17] = 'E'; strcpy(kname[17], "E");
	dik[18] = DIK_R; vk[18] = 'R'; strcpy(kname[18], "R");
	dik[19] = DIK_T; vk[19] = 'T'; strcpy(kname[19], "T");
	dik[20] = DIK_Y; vk[20] = 'Y'; strcpy(kname[20], "Y");
	dik[21] = DIK_U; vk[21] = 'U'; strcpy(kname[21], "U");
	dik[22] = DIK_I; vk[22] = 'I'; strcpy(kname[22], "I");
	dik[23] = DIK_O; vk[23] = 'O'; strcpy(kname[23], "O");
	dik[24] = DIK_P; vk[24] = 'P'; strcpy(kname[24], "P");
	dik[25] = DIK_LBRACKET; vk[25] = 0 ; strcpy(kname[25], "tecla [");
	dik[26] = DIK_RBRACKET; vk[26] = 0; strcpy(kname[26], "tecla ]");
	dik[27] = DIK_RETURN; vk[27] = VK_RETURN; strcpy(kname[27], "RETORNO");
	dik[28] = DIK_LCONTROL; vk[28] = VK_CONTROL; strcpy(kname[28], "CONTROL IZQ");
	dik[29] = DIK_A; vk[29] = 'A'; strcpy(kname[29], "A");
	dik[30] = DIK_S; vk[30] = 'S'; strcpy(kname[30], "S");
	dik[31] = DIK_D; vk[31] = 'D'; strcpy(kname[31], "D");
	dik[32] = DIK_F; vk[32] = 'F'; strcpy(kname[32], "F");
	dik[33] = DIK_G; vk[33] = 'G'; strcpy(kname[33], "G");
	dik[34] = DIK_H; vk[34] = 'H'; strcpy(kname[34], "H");
	dik[35] = DIK_J; vk[35] = 'J'; strcpy(kname[35], "J");
	dik[36] = DIK_K; vk[36] = 'K'; strcpy(kname[36], "K");
	dik[37] = DIK_L; vk[37] = 'L'; strcpy(kname[37], "L");
	dik[38] = DIK_SEMICOLON; vk[38] = 0; strcpy(kname[38], "tecla ,");
	dik[39] = DIK_APOSTROPHE; vk[39] = 0; strcpy(kname[39], "tecla '");
	dik[40] = DIK_GRAVE; vk[40] = 0; strcpy(kname[40], "tecla ´");
	dik[41] = DIK_LSHIFT; vk[41] = VK_SHIFT; strcpy(kname[41], "SHIFT IZQ");
	dik[42] = DIK_BACKSLASH; vk[42] = 0; strcpy(kname[42], "BACKSLASH");
	dik[43] = DIK_Z; vk[43] = 'Z'; strcpy(kname[43], "Z");
	dik[44] = DIK_X; vk[44] = 'X'; strcpy(kname[44], "X");
	dik[45] = DIK_C; vk[45] = 'C'; strcpy(kname[45], "C");
	dik[46] = DIK_V; vk[46] = 'V'; strcpy(kname[46], "V");
	dik[47] = DIK_B; vk[47] = 'B'; strcpy(kname[47], "B");
	dik[48] = DIK_N; vk[48] = 'N'; strcpy(kname[48], "N");
	dik[49] = DIK_M; vk[49] = 'M'; strcpy(kname[49], "M");
	dik[50] = DIK_COMMA; vk[50] = 0; strcpy(kname[50], "tecla ,");
	dik[51] = DIK_PERIOD; vk[51] = 0; strcpy(kname[51], "PERIOD");
	dik[52] = DIK_SLASH; vk[52] = 0; strcpy(kname[52], "SLASH");
	dik[53] = DIK_RSHIFT; vk[53] = VK_SHIFT; strcpy(kname[53], "SHIFT DER");
	dik[54] = DIK_MULTIPLY; vk[54] = VK_MULTIPLY; strcpy(kname[54], "tecla *");
	dik[55] = DIK_LMENU; vk[55] = VK_MENU ; strcpy(kname[55], "ALT");
	dik[56] = DIK_SPACE; vk[56] = VK_SPACE; strcpy(kname[56], "ESPACIO");
	dik[57] = DIK_CAPITAL; vk[57] = VK_CAPITAL; strcpy(kname[57], "MAYUSCULAS");
	dik[58] = DIK_F1; vk[58] = VK_F1; strcpy(kname[58], "F1");
	dik[59] = DIK_F2; vk[59] = VK_F2; strcpy(kname[59], "F2");
	dik[60] = DIK_F3; vk[60] = VK_F3; strcpy(kname[60], "F3");
	dik[61] = DIK_F4; vk[61] = VK_F4; strcpy(kname[61], "F4");
	dik[62] = DIK_F5; vk[62] = VK_F5; strcpy(kname[62], "F5");
	dik[63] = DIK_F6; vk[63] = VK_F6; strcpy(kname[63], "F6");
	dik[64] = DIK_F7; vk[64] = VK_F7; strcpy(kname[64], "F7");
	dik[65] = DIK_F8; vk[65] = VK_F8; strcpy(kname[65], "F8");
	dik[66] = DIK_F9; vk[66] = VK_F9; strcpy(kname[66], "F9");
	dik[67] = DIK_F10; vk[67] = VK_F10; strcpy(kname[67], "F10");
	dik[68] = DIK_NUMLOCK; vk[68] = VK_NUMLOCK; strcpy(kname[68], "NUMLOCK");
	dik[69] = DIK_SCROLL; vk[69] = VK_SCROLL; strcpy(kname[69], "SCROLL");
	dik[70] = DIK_NUMPAD7; vk[70] = VK_NUMPAD7; strcpy(kname[70], "NUMPAD7");
	dik[71] = DIK_NUMPAD8; vk[71] = VK_NUMPAD8; strcpy(kname[71], "NUMPAD8");
	dik[72] = DIK_NUMPAD9; vk[72] = VK_NUMPAD9; strcpy(kname[72], "NUMPAD9");
	dik[73] = DIK_SUBTRACT; vk[73] = VK_SUBTRACT; strcpy(kname[73], "NUMPAD -");
	dik[74] = DIK_NUMPAD4; vk[74] = VK_NUMPAD4; strcpy(kname[74], "NUMPAD4");
	dik[75] = DIK_NUMPAD5; vk[75] = VK_NUMPAD5; strcpy(kname[75], "NUMPAD5");
	dik[76] = DIK_NUMPAD6; vk[76] = VK_NUMPAD6; strcpy(kname[76], "NUMPAD6");
	dik[77] = DIK_ADD; vk[77] = VK_ADD; strcpy(kname[77], "NUMPAD +");
	dik[78] = DIK_NUMPAD1; vk[78] = VK_NUMPAD1; strcpy(kname[78], "NUMPAD1");
	dik[79] = DIK_NUMPAD2; vk[79] = VK_NUMPAD2; strcpy(kname[79], "NUMPAD2");
	dik[80] = DIK_NUMPAD3; vk[80] = VK_NUMPAD3; strcpy(kname[80], "NUMPAD3");
	dik[81] = DIK_NUMPAD0; vk[81] = VK_NUMPAD0; strcpy(kname[81], "NUMPAD0");
	dik[82] = DIK_DECIMAL; vk[82] = 0; strcpy(kname[82], "NUMPAD .");
	dik[83] = DIK_OEM_102; vk[83] = 0; strcpy(kname[83], "tecla 102");
	dik[84] = DIK_F11; vk[84] = VK_F11; strcpy(kname[84], "F11");
	dik[85] = DIK_F12; vk[85] = VK_F12; strcpy(kname[85], "F12");
	dik[86] = DIK_RMENU; vk[86] = VK_MENU; strcpy(kname[86], "ALT GR");/* right Alt */
	dik[87] = DIK_PAUSE; vk[87] = VK_PAUSE; strcpy(kname[87], "PAUSA");
	dik[88] = DIK_HOME; vk[88] = VK_HOME; strcpy(kname[88], "INICIO");
	dik[89] = DIK_UP; vk[89] = VK_UP; strcpy(kname[89], "ARRIBA");
	dik[90] = DIK_PRIOR; vk[90] = VK_PRIOR; strcpy(kname[90], "RE PAG");/* PgUp on arrow keypad */
	dik[91] = DIK_LEFT; vk[91] = VK_LEFT; strcpy(kname[91], "IZQUIERDA");
	dik[92] = DIK_RIGHT; vk[92] = VK_RIGHT; strcpy(kname[92], "DERECHA");
	dik[93] = DIK_END; vk[93] = VK_END; strcpy(kname[93], "FIN");
	dik[94] = DIK_DOWN; vk[94] = VK_DOWN; strcpy(kname[94], "ABAJO");
	dik[95] = DIK_NEXT; vk[95] = VK_NEXT; strcpy(kname[95], "AV PAG");/* PgDn on arrow keypad */
	dik[96] = DIK_INSERT; vk[96] = VK_INSERT; strcpy(kname[96], "INSERT");
	dik[97] = DIK_DELETE; vk[97] = VK_DELETE; strcpy(kname[97], "SUPR");
}	


/********************************************************
 devuelve el texto de un indice de la tabla de nombres
********************************************************/
char * KEYTABLE::GetText(int index)
{
	return kname[index];
}


/********************************************************
 Devuelve la tecla DirectInput correspondiente al nombre
 de la tecla
********************************************************/
UINT KEYTABLE::GetDIK(char *text)
{
	int i;

	for(i=0;i< NUMKEYS;i++)
		if(!strcmp(text,kname[i]))
		{			
			return dik[i];
		}

	return 0;
}

/********************************************************
	Guarda en <text> el nombre de una tecla DirectInput
   Y devuelve el indice correspondiente en la tabla.
********************************************************/
int KEYTABLE::GetDIKeyText(UINT param, char *text)
{
	int i;

	for(i=0;i< NUMKEYS;i++)
		if(param == dik[i])
		{
			strcpy(text, kname[i]);
			return i;
		}

	return -1;
}

/********************************************************
 Dada una tecla DirectInput, devuelve su equivalente en texto
********************************************************/
char * KEYTABLE::GetDIKeyText(UINT param)
{
	int i;

	for(i=0;i< NUMKEYS;i++)
		if(param == dik[i])
		{
			return kname[i];
		}

	return NULL;
}

/********************************************************
 Dada una tecla VirtualKey guarda en <text> su equivalente
 en texto, y devuelve el indice de la tabla
********************************************************/
int KEYTABLE::GetVrtKeyText(int param, char *text)
{
	int i;

	for(i=0;i< NUMKEYS;i++)
		if(param == vk[i])
		{
			strcpy(text, kname[i]);
			return i;
		}

	return -1;
}


/********************************************************
 Dada una tevla VirtualKey devuelve su equivalente en texto
********************************************************/
char * KEYTABLE::GetVrtKeyText(int param)
{
	int i;

	for(i=0;i< NUMKEYS;i++)
		if(param == vk[i])
		{
			return kname[i];
		}

	return NULL;
}

/********************************************************
 Devuelve el equivalente de una tecla VirtualKey en DirectInput
********************************************************/
UINT KEYTABLE::ToDI(int vkey)
{
	int i;

	for(i=0;i< NUMKEYS;i++)
		if(vkey == vk[i])
			return dik[i];

	return 0;
}

/********************************************************
 Devuelve el equivalente de una tecla DirectInput en VirtualKey
********************************************************/
int KEYTABLE::ToVK(UINT dikey)
{
int i;

	for(i=0;i< NUMKEYS;i++)
		if(dikey == dik[i])
			return vk[i];

	return 0;
}

