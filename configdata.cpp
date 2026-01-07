#include "pang.h"
#include "configdata.h"
#include <SDL.h>
#include <stdio.h>
#include <string.h>

extern PGAMEINFO gameinf;
extern int globalmode;

CONFIGDATA::CONFIGDATA()
{
    InitPath();
}

CONFIGDATA::~CONFIGDATA()
{
}

void CONFIGDATA::InitPath()
{
    char *basePath = SDL_GetPrefPath("Savioret", "Pang");
    if (basePath) {
        snprintf(configPath, sizeof(configPath), "%spang_config.dat", basePath);
        SDL_free(basePath);
    } else {
        // Fallback to local directory if PrefPath fails
        strcpy(configPath, "pang_config.dat");
    }
}

void CONFIGDATA::SetDefaults()
{
    // Default keys (P1: Left, Right, Shift. P2: A, D, Ctrl)
    gameinf.keys[0].left = SDL_SCANCODE_LEFT;
    gameinf.keys[0].right = SDL_SCANCODE_RIGHT;
    gameinf.keys[0].shoot = SDL_SCANCODE_SPACE; // Changed from LSHIFT for better default in SDL
    
    gameinf.keys[1].left = SDL_SCANCODE_A;
    gameinf.keys[1].right = SDL_SCANCODE_D;
    gameinf.keys[1].shoot = SDL_SCANCODE_LCTRL;

    globalmode = RENDERMODE_NORMAL;
}

int CONFIGDATA::Load()
{
    FILE* fp = fopen(configPath, "r");
    if (!fp)
    {
        SetDefaults();
        return 0; // Config not found/loaded, used defaults
    }

    char line[256];
    while (fgets(line, sizeof(line), fp))
    {
        // Ignorar líneas vacías y comentarios
        if (line[0] == '\n' || line[0] == '#' || line[0] == ';')
            continue;
            
        // Parsear línea en formato "clave=valor"
        char key[128], value[128];
        if (sscanf(line, "%127[^=]=%127s", key, value) == 2)
        {
            // Player 1 keys
            if (strcmp(key, "P1_Left") == 0)
                gameinf.keys[0].left = (SDL_Scancode)atoi(value);
            else if (strcmp(key, "P1_Right") == 0)
                gameinf.keys[0].right = (SDL_Scancode)atoi(value);
            else if (strcmp(key, "P1_Shoot") == 0)
                gameinf.keys[0].shoot = (SDL_Scancode)atoi(value);
            // Player 2 keys
            else if (strcmp(key, "P2_Left") == 0)
                gameinf.keys[1].left = (SDL_Scancode)atoi(value);
            else if (strcmp(key, "P2_Right") == 0)
                gameinf.keys[1].right = (SDL_Scancode)atoi(value);
            else if (strcmp(key, "P2_Shoot") == 0)
                gameinf.keys[1].shoot = (SDL_Scancode)atoi(value);
            // Render mode
            else if (strcmp(key, "RenderMode") == 0)
                globalmode = atoi(value);
        }
    }

    fclose(fp);
    return 1;
}

int CONFIGDATA::Save()
{
    FILE* fp = fopen(configPath, "w");
    if (!fp) return 0;

    // Escribir configuración en formato INI
    fprintf(fp, "# Pang Game Configuration\n");
    fprintf(fp, "# Generated automatically - Edit with care\n\n");
    
    fprintf(fp, "[Player1]\n");
    fprintf(fp, "P1_Left=%d\n", gameinf.keys[0].left);
    fprintf(fp, "P1_Right=%d\n", gameinf.keys[0].right);
    fprintf(fp, "P1_Shoot=%d\n\n", gameinf.keys[0].shoot);
    
    fprintf(fp, "[Player2]\n");
    fprintf(fp, "P2_Left=%d\n", gameinf.keys[1].left);
    fprintf(fp, "P2_Right=%d\n", gameinf.keys[1].right);
    fprintf(fp, "P2_Shoot=%d\n\n", gameinf.keys[1].shoot);
    
    fprintf(fp, "[Graphics]\n");
    fprintf(fp, "RenderMode=%d  # 1=Windowed, 2=Fullscreen\n", globalmode);
    
    fclose(fp);
    return 1;
}
