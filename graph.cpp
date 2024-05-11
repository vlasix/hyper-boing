#include <stdio.h>
#include "pang.h"
#include "wingdi.h"


void GRAPH::GetDC()
{
	lpDDSBack->GetDC(&hdc);
}

void GRAPH::ReleaseDC()
{
	lpDDSBack->ReleaseDC(hdc);
}

int GRAPH::Init(HWND _hwnd, int _mode)
{
	mode = _mode;
	hwnd = _hwnd;

	if (mode==RENDERMODE_NORMAL) return InitNormal();
	else return InitEx();
}

int GRAPH::InitEx()
{
  DDSURFACEDESC2 ddsd;
  DDSCAPS2 ddscaps;
  HRESULT ddrval; 
  int i;  
  

  // Creamos el objeto DirectDraw
  ddrval = DirectDrawCreateEx(NULL, (VOID**)&lpDD, IID_IDirectDraw7, NULL);  
  if( ddrval != DD_OK )
  {
	  MessageBox(hwnd, "Error Creando el objeto directX", 0,0);
	  return 0;
  }
  // Definimos el comportamiento de la aplicacion
  ddrval = lpDD->SetCooperativeLevel (hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
  if( ddrval != DD_OK )
  {
	  MessageBox(hwnd, "Error en SetCooperativeLevel", 0,0);
	  return 0;
  }

  // Ajustamos el modo de video
  ddrval = lpDD->SetDisplayMode (RES_X, RES_Y, 16, 0,0);
  if(ddrval != DD_OK)
  {	  
	  MessageBox(hwnd, "Error cambiando el modo de video", 0,0);
	  return 0;
  }

  // Creamos la superficie primaria
  ZeroMemory (&ddsd, sizeof(ddsd));
  ddsd.dwSize  = sizeof(ddsd);
  ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | 
						DDSCAPS_FLIP | 
						DDSCAPS_COMPLEX;
  ddsd.dwBackBufferCount = 1; 
  ddrval = lpDD->CreateSurface (&ddsd, &lpDDSPrimary, NULL);
  if( ddrval != DD_OK )
  {
	  MessageBox(hwnd, "Error creando la superficie Primaria", 0,0);
	  return 0;
  }

  // Creamos el BackBuffer
  ZeroMemory(&ddscaps, sizeof(ddscaps));  
  ddscaps.dwCaps = DDSCAPS_BACKBUFFER; 
  ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack);
  if( ddrval != DD_OK )
  {
	  MessageBox(hwnd, "Error creando la superficie Secundaria", 0,0);
	  return 0;
  }


	return 1;
}

int GRAPH::InitNormal()
{
	HRESULT      ddrval;
	DDSURFACEDESC2 ddsd;
	DDSCAPS2 ddscaps;	
	
	ddrval = DirectDrawCreateEx (NULL, (void**)&lpDD,IID_IDirectDraw7, NULL);
	if(ddrval != DD_OK) // La superficie no se ha creado
		return 0; 

	//Determinar el comportamiento de la aplicacion	
	ddrval = lpDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL); 
	if(ddrval != DD_OK) 
		return 0;

	  // Creamos la superficie primaria
	ZeroMemory (&ddsd, sizeof(ddsd));
	ddsd.dwSize  = sizeof(ddsd);	
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;	
	ddrval = lpDD->CreateSurface (&ddsd, &lpDDSPrimary, NULL);
	if( ddrval != DD_OK )
	{
		int mal;
		switch(ddrval)
		{
		case DDERR_INCOMPATIBLEPRIMARY:
				mal=1; break;
		case DDERR_INVALIDCAPS:
				mal=2; break;
		case DDERR_INVALIDPARAMS:
				mal=4; break;
		case DDERR_INVALIDPIXELFORMAT:
				mal=5; break;
		}
		return 0;
	}

	   // Crear el Clipper para la superficie primaria
	ddrval = lpDD->CreateClipper (0, &lpClipper, NULL);
	if( ddrval != DD_OK ) return 0;

	ddrval = lpClipper->SetHWnd (0, hwnd);
	if( ddrval != DD_OK ) return 0;

	ddrval = lpDDSPrimary->SetClipper (lpClipper);
	if( ddrval != DD_OK ) return 0;

	  // Crear el BACK-buffer
	  ZeroMemory(&ddsd, sizeof(ddsd));
	  ddsd.dwSize = sizeof(ddsd);
	  ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	  ddsd.dwWidth = RES_X;
	  ddsd.dwHeight = RES_Y;
	  ddrval = lpDD->CreateSurface(&ddsd, (LPDIRECTDRAWSURFACE7 FAR *)&lpDDSBack, NULL);
	  if( ddrval != DD_OK ) return 0;

	  return 1;

}


void GRAPH::Draw(LPDIRECTDRAWSURFACE7 lpdds, int x, int y)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = 81;
	rc.bottom = 81;



	lpDDSBack->BltFast(x , y, lpdds,
			&rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
}

void GRAPH::Draw(SPRITE *spr, int x, int y)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = spr->sx;
	rc.bottom = spr->sy;


	lpDDSBack->BltFast(x + spr->xoff, y + spr->yoff, spr->bmp,
			&rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);	
}

void GRAPH::Draw(LPDIRECTDRAWSURFACE7 lpdds, RECT *rc, int x, int y)
{
	lpDDSBack->BltFast(x , y ,lpdds,
			rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);	
}

void GRAPH::DrawClipped(LPDIRECTDRAWSURFACE7 lpdds, RECT *rc, int x, int y)
{
	int sx = rc->right - rc->left;
	int sy = rc->bottom - rc->top;

	if(x<0){ rc->left = -x; x = 0; }
	if(x + sx > 639) rc->right -= x + sx - 640;
	if(y<0) { rc->top = -y; y = 0; }
	if(y + sy > 479) rc->bottom -= y + sy - 480;

	lpDDSBack->BltFast(x , y , lpdds,
			rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);	
}

void GRAPH::DrawClipped(SPRITE *spr, int x, int y)
{
	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = spr->sx;
	rc.bottom = spr->sy;

	if(x<0){ rc.left = -x; x = 0; }
	if(x + spr->sx > 640) rc.right -= x + spr->sx - 640;
	if(y<0) { rc.top = -y; y = 0; }
	if(y + spr->sy > 480) rc.bottom -= y +spr->sy - 480;

	lpDDSBack->BltFast(x + spr->xoff, y + spr->yoff, spr->bmp,
			&rc, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);	
}

void GRAPH::Draw(BMNUMFONT *font, int num, int x, int y)
{
	RECT rc;
	char cad[16];
	int n, i;
	int esp = 0;

	sprintf(cad, "%d", num);
	n = strlen(cad);

	for(i=0;i<n;i++)
	{
		rc = font->GetRect(cad[i]);		
		Draw(font->spr->bmp, &rc, x+esp, y);
		esp += rc.right - rc.left;
	}
}

void GRAPH::Draw(BMNUMFONT *font, char cad[], int x, int y)
{
	RECT rc;	
	int n, i;
	int esp = 0;
	
	n = strlen(cad);

	for(i=0;i<n;i++)
	{
		rc = font->GetRect(cad[i]);		
		Draw(font->spr->bmp, &rc, x+esp, y);
		esp += rc.right - rc.left;
	}
}

void GRAPH::DrawClipped(BMNUMFONT *font, char cad[], int x, int y)
{
	RECT rc;	
	int n, i;
	int esp = 0;
	
	n = strlen(cad);

	for(i=0;i<n;i++)
	{
		rc = font->GetRect(cad[i]);		
		DrawClipped(font->spr->bmp, &rc, x+esp, y);
		esp += rc.right - rc.left;
	}
}



void GRAPH::Flip()
{
    RECT destRect;
    HRESULT hresult;
    POINT pt;

	if (mode == RENDERMODE_EXCLUSIVE) //modo a pantalla completa
	{  
		hresult = lpDDSPrimary->Flip (NULL, DDFLIP_WAIT);
		if (hresult == DDERR_SURFACELOST)
		{
			lpDDSPrimary->Restore();
			lpDDSBack->Restore();
				lpDDSPrimary->Flip (NULL, DDFLIP_WAIT);
		}
	}
	else //modo ventana
	{
		pt.x = pt.y = 0;
		ClientToScreen (hwnd, &pt);
		SetRect (&destRect, pt.x, pt.y, pt.x+RES_X, pt.y+RES_Y);
		hresult = lpDDSPrimary->Blt (&destRect, lpDDSBack, NULL, DDBLT_WAIT, NULL);
	}

	if (hresult == DDERR_SURFACELOST)
	{
		lpDDSPrimary->Restore();
		lpDDSBack->Restore(); 		
	}	
}


void GRAPH::Text(char texto[], int x, int y)
{
	SetBkColor(hdc, RGB(255, 255, 255)); 
	SetTextColor(hdc, RGB(50, 0, 0)); 
	TextOut(hdc, x, y, texto, strlen(texto)); 
}

void GRAPH::Ellipse(int a, int b, int c, int d)
{
	::Ellipse( hdc, a, b, c, d);
}


void GRAPH::Rectangle(int a, int b, int c, int d)
{
	::Rectangle( hdc, a, b, c, d);
}

void GRAPH::MoveTo(int x, int y)
{
	::MoveToEx(hdc, x, y, NULL);	
}

void GRAPH::LineTo(int x, int y)
{
	::LineTo(hdc, x, y);
}

void GRAPH::Release(void) 
{ 
	if(lpDD != NULL) 
	{ 
		if(lpDDSPrimary != NULL) 
		{ 
			lpDDSPrimary->Release(); 
			lpDDSPrimary = NULL; 
		} 
		lpDD->Release(); 
		lpDD = NULL; 
	} 
} // finiObjects




/****************************************************************/
/***				OPERACIONES CON BITMAPS					*****/
/****************************************************************/

void GRAPH::LoadBitmap(SPRITE *spr, LPCSTR szBitmap)
{
    HBITMAP                 hbm;
    BITMAP                  bm;
    DDSURFACEDESC2          ddsd;


    //  Intentamos cargar el bitmap como Recurso, sino lo cargamos desde un fichero
    hbm = (HBITMAP) LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0,
                              0, LR_CREATEDIBSECTION);
    if (hbm == NULL)
        hbm = (HBITMAP) LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
                                  LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hbm == NULL)
        return;
    // Tamaño del bitmap
    GetObject(hbm, sizeof(bm), &bm);

    // Creamos la superficie donde guardamos el bitmap
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    spr->sx = ddsd.dwWidth = bm.bmWidth;
    spr->sy = ddsd.dwHeight = bm.bmHeight;
    if (lpDD->CreateSurface(&ddsd, &spr->bmp, NULL) != DD_OK)
        return;
    this->CopyBitmap(spr->bmp, hbm, 0, 0, 0, 0);
    DeleteObject(hbm);
    
}

/********************************************************
  CopyBitmap()

  Dibuja un bitmap en una Superficie DirectDraw.
********************************************************/
HRESULT 
GRAPH::CopyBitmap(IDirectDrawSurface7 * pdds, HBITMAP hbm, int x, int y,
             int dx, int dy)
{
    HDC                     hdcImage;
    HDC                     hdc;
    BITMAP                  bm;
    DDSURFACEDESC2          ddsd;
    HRESULT                 hr;

    if (hbm == NULL || pdds == NULL)
        return E_FAIL;
    // Por si acaso...
    pdds->Restore();

    // Creamos un contexto de dispositivo compatible donde
	// guardaremos el bitmap	
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        OutputDebugString("createcompatible dc failed\n");
    SelectObject(hdcImage, hbm);
    //
    // Obtenemos las dimensiones del bitmap
    //
    GetObject(hbm, sizeof(bm), &bm);
    dx = dx == 0 ? bm.bmWidth : dx;
    dy = dy == 0 ? bm.bmHeight : dy;
    //
    // Obtenemos el tamaño de la superficie
    //
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    pdds->GetSurfaceDesc(&ddsd);

    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y,
                   dx, dy, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }
    DeleteDC(hdcImage);
    return hr;
}


/********************************************************
  ColorMatch()

  Convierte un color RGB a un color fisico.
  Esta funcion es util para cuando vamos a leer colores en
  modos de video menores de 24bits.
********************************************************/

DWORD
GRAPH::ColorMatch(IDirectDrawSurface7 * pdds, COLORREF rgb)
{
    COLORREF                rgbT;
    HDC                     hdc;
    DWORD                   dw = CLR_INVALID;
    DDSURFACEDESC2          ddsd;
    HRESULT                 hres;

    //
    //  Usamos la funcion SetPixel de la GDI
    //
    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
        SetPixel(hdc, 0, 0, rgb);       // Set our value
        pdds->ReleaseDC(hdc);
    }
    //
    // Bloqueamos la superfici de manera que podamos leer el color convertido	
    //
    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING)
        ;
    if (hres == DD_OK)
    {
        dw = *(DWORD *) ddsd.lpSurface;                 // Get DWORD
        if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
            dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount) - 1;  // Mask it to bpp
        pdds->Unlock(NULL);
    }
    

    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
        SetPixel(hdc, 0, 0, rgbT);
        pdds->ReleaseDC(hdc);
    }
    return dw;
}

/********************************************************
  SetColorKey()

  Pone el color-key para una superficie, dado un RGB.
  Este color es el color transparente en los sprites.
********************************************************/
HRESULT
GRAPH::SetColorKey(IDirectDrawSurface7 * pdds, COLORREF rgb)
{
    DDCOLORKEY              ddck;

    ddck.dwColorSpaceLowValue = ColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}




//================================================
void SPRITE::Init(GRAPH *gr, char file[], int offx, int offy)
{
	graph = gr;
	graph->LoadBitmap( this, file);

	xoff = offx;
	yoff = offy;
}