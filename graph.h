#ifndef GRAPH_H_
#define GRAPH_H_

//#include <windows.h>

#define RENDERMODE_NORMAL 1
#define RENDERMODE_EXCLUSIVE 2

#define RES_X 640
#define RES_Y 480

class GRAPH;
class BMNUMFONT;

/********************************************************
  clase SPRITE

  Es el objeto Sprite, como su nombre indica. Este contiene
  informacion sobre el bitmap, sus dimensiones para gestionar
  colisiones, y el desplazamiento "relativo".
  xoff yoff > Se utilizan para representar secuencias de animacion
	por ejemplo, si un personaje esta andando, al caminar todos
	sus fotogramas deben quedar alineados con su cuerpo, de lo contrario
	parecerá que este haciendo el baile del sambito. Xoff e Yoff
	son valores que ayudan a ajustar y corregir esto.
********************************************************/
class SPRITE
{
public:
	LPDIRECTDRAWSURFACE7 bmp;
	int sx, sy;
	int xoff, yoff; //desplazamiento relativo
	GRAPH * graph;

	void Init(GRAPH *gr, char file[], int offx=0, int offy=0);
};



/********************************************************
  clase GRAPH

  Es la clase que realiza las funciones "primitivas" de pintado.
  De la misma forma, inicia el modo grafico, y lo restaura.
  Tambien carga imagenes BMP.
  Todo ello usando DirectX.
********************************************************/
class GRAPH
{
public:	
	HWND hwnd;
	HDC hdc;
	LPDIRECTDRAW7 lpDD ;
	LPDIRECTDRAWSURFACE7 lpDDSPrimary ;
	LPDIRECTDRAWSURFACE7 lpDDSBack ;
	LPDIRECTDRAWCLIPPER lpClipper;       // clipper para primary
	int mode;

	int Init(HWND _hwnd, int _mode);
	int InitNormal();
	int InitEx();
	void GetDC();
	void ReleaseDC();
	void Flip();
	void Draw(LPDIRECTDRAWSURFACE7 lpdds, int x, int y);
	void Draw(SPRITE *spr, int x, int y);
	void Draw(LPDIRECTDRAWSURFACE7 lpdds, RECT *rc, int x, int y);
	void DrawClipped(LPDIRECTDRAWSURFACE7 lpdds, RECT *rc, int x, int y);
	void Draw(BMNUMFONT *numfont, int num, int x, int y);
	void Draw(BMNUMFONT *font, char cad[], int x, int y);
	void DrawClipped(BMNUMFONT *font, char cad[], int x, int y);
	void DrawClipped(SPRITE *spr, int x, int y);
	void Text(char texto[], int x, int y);
	void Ellipse(int a, int b, int c, int d);
	void Rectangle(int a, int b, int c, int d);
	void MoveTo(int x, int y);
	void LineTo(int x, int y);

	HRESULT SetColorKey(IDirectDrawSurface7 * pdds, COLORREF rgb);
	HRESULT CopyBitmap(IDirectDrawSurface7 * pdds, HBITMAP hbm, int x, int y, int dx, int dy);
	IDirectDrawSurface7 * LoadBitmap(LPCSTR szBitmap, int dx, int dy);	
	DWORD ColorMatch(IDirectDrawSurface7 * pdds, COLORREF rgb);
	void LoadBitmap(SPRITE *spr, LPCSTR szBitmap);



	void Release();
};

#endif