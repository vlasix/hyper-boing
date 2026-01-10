#include "pang.h"
#include <fstream>
#include <sstream>

void BMNUMFONT::Init(SPRITE *sprite)
{
    spr = sprite;

    numchars = 10;
}

void BMNUMFONT::SetValue(int number, int xoff)
{
    if(number >= 0 && number <=9 )
        offsets[number] = xoff;
}

void BMNUMFONT::SetValues(int *xoff)
{
    int i;

    for(i=0; i<numchars;i++)	
        offsets[i] = xoff[i];
}

RECT BMNUMFONT::GetRect( char numchr)
{
    RECT rcchr;
    int code;

    /* los codigos ascii van del 48 al 57 */
    code = numchr;
    code -= 48;

    if(code >= 0 && code <= 9)
    {
        rcchr.top = 0;
        rcchr.bottom = spr->sy;
        rcchr.left = offsets[code];
        if(code == numchars-1) rcchr.right = spr->sx - 1;
            else rcchr.right = offsets[code + 1];		
    }
    else
        SetRect(&rcchr, 0,0,0,0);

    return rcchr;
}

/********************************************************
  BMFontLoader Implementation
********************************************************/

BMFontLoader::BMFontLoader()
{
    lineHeight = 0;
    base = 0;
    scaleW = 0;
    scaleH = 0;
    pages = 0;
}

bool BMFontLoader::Load(const char* fntFilePath)
{
    std::ifstream file(fntFilePath);
    if (!file.is_open())
    {
        printf("Error: No se pudo abrir el archivo BMFont: %s\n", fntFilePath);
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (!ParseLine(line))
        {
            // Continue even if one line fails
        }
    }

    file.close();
    return true;
}

bool BMFontLoader::ParseLine(const std::string& line)
{
    if (line.empty()) return true;

    std::istringstream iss(line);
    std::string type;
    iss >> type;

    if (type == "common")
    {
        std::string token;
        while (iss >> token)
        {
            size_t pos = token.find('=');
            if (pos != std::string::npos)
            {
                std::string key = token.substr(0, pos);
                int value = atoi(token.substr(pos + 1).c_str());

                if (key == "lineHeight") lineHeight = value;
                else if (key == "base") base = value;
                else if (key == "scaleW") scaleW = value;
                else if (key == "scaleH") scaleH = value;
                else if (key == "pages") pages = value;
            }
        }
    }
    else if (type == "page")
    {
        std::string token;
        while (iss >> token)
        {
            size_t pos = token.find('=');
            if (pos != std::string::npos)
            {
                std::string key = token.substr(0, pos);
                std::string value = token.substr(pos + 1);

                if (key == "file")
                {
                    // Remove quotes
                    if (value.front() == '"') value = value.substr(1);
                    if (value.back() == '"') value.pop_back();
                    fontTexture = value;
                }
            }
        }
    }
    else if (type == "char")
    {
        BMFontChar character;
        character.id = 0;
        character.x = 0;
        character.y = 0;
        character.width = 0;
        character.height = 0;
        character.xoffset = 0;
        character.yoffset = 0;
        character.xadvance = 0;
        character.page = 0;

        std::string token;
        while (iss >> token)
        {
            size_t pos = token.find('=');
            if (pos != std::string::npos)
            {
                std::string key = token.substr(0, pos);
                int value = atoi(token.substr(pos + 1).c_str());

                if (key == "id") character.id = value;
                else if (key == "x") character.x = value;
                else if (key == "y") character.y = value;
                else if (key == "width") character.width = value;
                else if (key == "height") character.height = value;
                else if (key == "xoffset") character.xoffset = value;
                else if (key == "yoffset") character.yoffset = value;
                else if (key == "xadvance") character.xadvance = value;
                else if (key == "page") character.page = value;
            }
        }

        characters[character.id] = character;
    }

    return true;
}

BMFontChar* BMFontLoader::GetChar(int charId)
{
    if (characters.find(charId) != characters.end())
    {
        return &characters[charId];
    }
    return nullptr;
}

/********************************************************
  BMFontRenderer Implementation
********************************************************/

BMFontRenderer::BMFontRenderer()
{
    font = nullptr;
    fontTexture = nullptr;
    graph = nullptr;
    colorR = 255;
    colorG = 255;
    colorB = 255;
    colorA = 255;
    scale = 1.0f;
}

void BMFontRenderer::Init(GRAPH* gr, BMFontLoader* bmFont, SPRITE* texture)
{
    graph = gr;
    font = bmFont;
    fontTexture = texture;
    colorR = 255;
    colorG = 255;
    colorB = 255;
    colorA = 255;
    scale = 1.0f;
}

void BMFontRenderer::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    colorR = r;
    colorG = g;
    colorB = b;
    colorA = a;
}

void BMFontRenderer::SetScale(float s)
{
    scale = s;
}

void BMFontRenderer::Text(const char* texto, int x, int y)
{
    if (!font || !fontTexture || !graph) return;

    // Set texture color modulation
    SDL_SetTextureColorMod(fontTexture->bmp, colorR, colorG, colorB);
    SDL_SetTextureAlphaMod(fontTexture->bmp, colorA);

    int currentX = x;
    
    for (int i = 0; texto[i] != '\0'; i++)
    {
        int charId = (int)((unsigned char)texto[i]);
        BMFontChar* ch = font->GetChar(charId);

        if (ch != nullptr && ch->width > 0 && ch->height > 0)
        {
            SDL_Rect srcRect;
            srcRect.x = ch->x;
            srcRect.y = ch->y;
            srcRect.w = ch->width;
            srcRect.h = ch->height;

            SDL_Rect dstRect;
            dstRect.x = currentX + (int)(ch->xoffset * scale);
            dstRect.y = y + (int)(ch->yoffset * scale);
            dstRect.w = (int)(ch->width * scale);
            dstRect.h = (int)(ch->height * scale);

            SDL_RenderCopy(graph->renderer, fontTexture->bmp, &srcRect, &dstRect);

            currentX += (int)(ch->xadvance * scale);
        }
        else
        {
            // Character not found, advance by a default amount
            currentX += (int)(font->lineHeight * 0.5f * scale);
        }
    }

    // Reset color modulation to white
    SDL_SetTextureColorMod(fontTexture->bmp, 255, 255, 255);
    SDL_SetTextureAlphaMod(fontTexture->bmp, 255);
}

int BMFontRenderer::GetTextWidth(const char* texto)
{
    if (!font) return 0;

    int width = 0;
    
    for (int i = 0; texto[i] != '\0'; i++)
    {
        int charId = (int)((unsigned char)texto[i]);
        BMFontChar* ch = font->GetChar(charId);

        if (ch != nullptr)
        {
            width += (int)(ch->xadvance * scale);
        }
        else
        {
            width += (int)(font->lineHeight * 0.5f * scale);
        }
    }

    return width;
}

int BMFontRenderer::GetTextHeight()
{
    if (!font) return 0;
    return (int)(font->lineHeight * scale);
}

void BMFontRenderer::Release()
{
    font = nullptr;
    fontTexture = nullptr;
    graph = nullptr;
}