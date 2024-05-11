#ifndef ITEM_H_
#define ITEM_H_


/********************************************************
  clase ITEM

  son los items del juego, que se pueden coger durante el
  transcurso de la partida, y modifican las armas, paran 
  el movimiento de las pelotas, etc.
********************************************************/

/*   CLASE NO IMPLEMENTADA    */

class ITEM
{
public:
	int x, y;
	int id;

	ITEM(int _x, int _y, int _id);
	~ITEM();
};

#endif