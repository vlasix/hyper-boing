#ifndef MENU_H_
#define MENU_H_

typedef struct PMNUBITMAPS
{
	SPRITE back;
	SPRITE title;
	SPRITE pulsa;
}PMENUBITMAPS;

/*******************************************************
	clase MENU

	Clase Hija de PAPP, es un modulo del juego.

	Es el menu principal del juego.
*********************************************************/
class PMENU : public PAPP
{
public:
	
	PMENUBITMAPS bmp;
	int xb, yb; //x e y del background
	int x, y;
	BOOL visible; // PULSA UNA TECLA
	int cont; //contador para el parpadeo 
	int Init();
	int InitBitmaps();
	void DrawTitle();
	void DrawBack();	

	void * MoveAll();
	int DrawAll();
	int Release();
};

#endif