#include <windows.h>
#include "pang.h"

PAPP::PAPP()
{		
}


/*******************************************************
  Esta funcion es primordial en juego.
  Aqui se administra el numero de veces que se va a pintar, 
  como maximo el establecido, y se calcularan tantas 
  veces como se indique (variable gamespeed).

  Por ejemplo, si queremos pintar 60 fotogramas por segundo
  mediante un contador, controlaremos que  calculamos 60, 
  y si nos da tiempo pintaremos 60, en caso de que tengamos
  un equipo mas lento, tan solo pintaremos los que nuestro
  ordenador pueda, pero la velocidad "virtual" del juego
  seguira siendo de 60 fps.
*******************************************************/
void * PAPP::DoTick()
{
	static short framestatus = 0;	
	static short cont =0;
	static long tick, lasttick;

	if (goback)
	{
		goback = FALSE;
		gameinf.menu = TRUE;
		return (PAPP*) new PMENU;
	}

	if(framestatus==0)
	{
		time1=GetTickCount();
		diftime2=time1-time2;
		if(diftime2< gamespeed) return NULL;
		time2=time1;
		diftime1+=diftime2;
		framestatus=1;
		return FALSE;
	}

	if (framestatus==1)
	{
		if(diftime1 < gamespeed)
		{
			framestatus=2;
			return NULL;
		}		
		void * newscreen = (PAPP*) MoveAll();
		diftime1-=gamespeed;
		return (PAPP*) newscreen;
	}
	
	if(framestatus==2)
	{
		DrawAll();
		framestatus=0;
		tick=GetTickCount();
		if(tick-lasttick>1000)
		{
		  fps = cont;
			cont = 0;
			lasttick = tick;
		}
		else
			cont++; //Frames por segundo
	}

	return NULL;
}

/*******************************************************
	Si hacemos una pausa tenemos que actualizar estos datos
	de manera que la funcion DoTick actue correctamente una vez
	reanudemos el juego.
*******************************************************/

void PAPP::DoPause()
{	
	diftime1=0; 
	diftime2=gamespeed;
	time1 = GetTickCount()+gamespeed;
	time2 = GetTickCount();

	return;
}

/*******************************************************
	Ajustamos la velocidad de pintado en fotogramas por segundo
	y lo transformamos a los milisegundos que equivalen
	por cada fotograma, que al fin y al cabo es el dato que
	nos va a hacer falta.
*********************************************************/
void PAPP::SetGameSpeed(int speed)
{
	gamespeed = 1000/speed;
}

/*******************************************************
	Indicamos si nuestra aplicacion esta o no activa
*********************************************************/
void PAPP::SetActive(BOOL b)
{
	active = b;
}

void PAPP::SetPause(BOOL b)
{
	pause = b;
}

BOOL PAPP::IsActive()
{
	return active;
}

BOOL PAPP::IsPaused()
{
	return pause;
}

