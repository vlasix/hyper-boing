#ifndef BMFONT_H_
#define BMFONT_H_

#include <map>
#include <string>

/********************************************************
  clase BMNUMFONT

  BitMap Number Font - Es un bitmap que contiene una 
  serie de numeros, del 0 al 9 y por este orden.
  offsets son las posiciones <X> de cada uno, es decir
  donde comienzan.
********************************************************/


class BMNUMFONT
{
public:
    SPRITE *spr;
    int offsets[10];
    int numchars;	
        
    void Init(SPRITE *sprite);
    RECT GetRect( char numchr);
    void SetValue(int number, int xoff);
    void SetValues(int *xoff);

};

/********************************************************
  struct BMFontChar

  Estructura que almacena la información de un carácter
  individual del BMFont según el formato .fnt
********************************************************/
struct BMFontChar
{
    int id;           // Character ID
    int x;            // X position in texture
    int y;            // Y position in texture
    int width;        // Width of character in texture
    int height;       // Height of character in texture
    int xoffset;      // X offset when drawing
    int yoffset;      // Y offset when drawing
    int xadvance;     // How much to advance after drawing this character
    int page;         // Texture page (for multi-page fonts)
};

/********************************************************
  clase BMFontLoader

  Carga archivos .fnt generados por BMFont y almacena
  la información de los caracteres y páginas de textura.
********************************************************/
class BMFontLoader
{
public:
    std::map<int, BMFontChar> characters;
    std::string fontTexture;
    int lineHeight;
    int base;
    int scaleW;
    int scaleH;
    int pages;

    BMFontLoader();
    bool Load(const char* fntFilePath);
    BMFontChar* GetChar(int charId);

private:
    bool ParseLine(const std::string& line);
};

/********************************************************
  clase BMFontRenderer

  Renderiza texto usando la información cargada por 
  BMFontLoader. Soporta color y escalado.
********************************************************/
class BMFontRenderer
{
public:
    BMFontLoader* font;
    SPRITE* fontTexture;
    GRAPH* graph;
    
    // Color del texto (R, G, B, A)
    unsigned char colorR;
    unsigned char colorG;
    unsigned char colorB;
    unsigned char colorA;
    
    // Escala del texto
    float scale;

    BMFontRenderer();
    void Init(GRAPH* gr, BMFontLoader* bmFont, SPRITE* texture);
    void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void SetScale(float s);
    void Text(const char* texto, int x, int y);
    int GetTextWidth(const char* texto);
    int GetTextHeight();
    void Release();
};

#endif