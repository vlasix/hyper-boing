#include "pang.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

extern int quit;

int PMENU::InitBitmaps()
{
    bmp.title.Init(&graph, "graph\\title.png", 0, 0);
    graph.SetColorKey(bmp.title.bmp, RGB(0,255,0));
    
    // Inicializar el fondo compartido
    PAPP::InitSharedBackground();
    
    // Cargar las nuevas opciones del menú (si no existen, se manejarán como NULL)
    bmp.optionPlay.Init(&graph, "graph\\menu_jugar.png", 0, 0);
    if(bmp.optionPlay.bmp)
        graph.SetColorKey(bmp.optionPlay.bmp, RGB(255,0,255));
        
    bmp.optionConfig.Init(&graph, "graph\\menu_config.png", 0, 0);
    if(bmp.optionConfig.bmp)
        graph.SetColorKey(bmp.optionConfig.bmp, RGB(255,0,255));
        
    bmp.optionExit.Init(&graph, "graph\\menu_salir.png", 0, 0);
    if(bmp.optionExit.bmp)
        graph.SetColorKey(bmp.optionExit.bmp, RGB(255,0,255));

    // Cargar la fuente BMFont para el menú
    if(fontLoader.Load("graph\\ThaleahFat.fnt"))
    {
        bmp.menuFont.Init(&graph, "graph\\ThaleahFat_0.png", 0, 0);
        fontRenderer.Init(&graph, &fontLoader, &bmp.menuFont);
        fontRenderer.SetColor(255, 255, 0, 255); // Amarillo
        fontRenderer.SetScale(2.0f); // Escala 2x para que sea legible
    }

    return 1;
}


int PMENU::Init()
{
    // Llamar a la inicialización base
    PAPP::Init();
    
    gameinf.menu = TRUE;
    InitBitmaps();
    x = 180; // Centrar mejor el título
    y = -300; // Empezar más arriba

    selectedOption = 0; // Empezar en JUGAR
    
    visible = FALSE;
    cont = 30;

    CloseMusic();
    OpenMusic("music\\menu.mid");
    PlayMusic();

    return 1;
}

int PMENU::Release()
{
    bmp.title.Release();
    bmp.optionPlay.Release();
    bmp.optionConfig.Release();
    bmp.optionExit.Release();
    bmp.menuFont.Release();
    
    fontRenderer.Release();

    CloseMusic();
    
    delete this;

    return 1;
}

void PMENU::DrawTitle()
{
    // Centrar el título horizontalmente
    int titleX = (RES_X - bmp.title.sx) / 2;
    int titleY = y;
    
    graph.DrawClipped(&bmp.title, titleX, titleY);
}

void PMENU::DrawMenu()
{
    // Calcular posiciones centradas
    int menuStartY = 320; // Empezar las opciones más abajo
    int spacing = 50;
    
    // Usar BMFont si está disponible
    if(fontRenderer.font && fontRenderer.fontTexture)
    {
        const char* options[3] = {"JUGAR", "CONFIGURACION", "SALIR"};
        
        for(int i = 0; i < 3; i++)
        {
            int optionY = menuStartY + (spacing * i);
            
            // Calcular el ancho del texto para centrarlo
            int textWidth = fontRenderer.GetTextWidth(options[i]);
            int textX = (RES_X - textWidth) / 2;
            
            // Dibujar solo si no está seleccionada o si debe ser visible
            if(selectedOption != i || visible)
            {
                fontRenderer.Text(options[i], textX, optionY);
            }
        }
        
        // Dibujar indicador de selección a la izquierda
        if(visible)
        {
            int indicatorY = menuStartY + (spacing * selectedOption);
            int textWidth = fontRenderer.GetTextWidth(options[selectedOption]);
            int textX = (RES_X - textWidth) / 2;
            int indicatorX = textX - 30; // Posición del indicador
            fontRenderer.Text(">", indicatorX, indicatorY);
        }
    }
    else
    {
        // Fallback a imágenes o texto por defecto
        // Centrar las opciones horizontalmente
        int option1X = (RES_X - (bmp.optionPlay.bmp ? bmp.optionPlay.sx : 150)) / 2;
        int option2X = (RES_X - (bmp.optionConfig.bmp ? bmp.optionConfig.sx : 200)) / 2;
        int option3X = (RES_X - (bmp.optionExit.bmp ? bmp.optionExit.sx : 150)) / 2;
        
        // Draw menu options
        // All bitmaps are resized to 200x40.
        // X position for center: (640 - 200) / 2 = 220
        int menuCenterX = 220; 
        
        // OPCIÓN 1: JUGAR
        if (bmp.optionPlay.bmp)
        {
            if (selectedOption == 0 && visible)
                graph.DrawScaled(&bmp.optionPlay, menuCenterX, menuStartY, 200, 40);
            else if(selectedOption != 0)
                graph.DrawScaled(&bmp.optionPlay, menuCenterX, menuStartY, 200, 40);
        }
        else
        {
            if (selectedOption != 0 || visible)
                graph.Text("JUGAR", menuCenterX, menuStartY);
        }
        
        // OPCIÓN 2: CONFIGURACIÓN
        if (bmp.optionConfig.bmp)
        {
            if (selectedOption == 1 && visible)
                graph.DrawScaled(&bmp.optionConfig, menuCenterX, menuStartY + spacing, 200, 40);
            else if(selectedOption != 1)
                graph.DrawScaled(&bmp.optionConfig, menuCenterX, menuStartY + spacing, 200, 40);
        }
        else
        {
            if (selectedOption != 1 || visible)
                graph.Text("CONFIGURACION", menuCenterX, menuStartY + spacing);
        }
            
        // OPCIÓN 3: SALIR
        if (bmp.optionExit.bmp)
        {
            if (selectedOption == 2 && visible)
             graph.DrawScaled(&bmp.optionExit, menuCenterX, menuStartY + spacing * 2, 200, 40);
            else if(selectedOption != 2)
                graph.DrawScaled(&bmp.optionExit, menuCenterX, menuStartY + spacing * 2, 200, 40);
        }
        else
        {
            if (selectedOption != 2 || visible)
             graph.Text("SALIR", menuCenterX, menuStartY + spacing * 2);
        }
        
        // Dibujar indicador de selección a la izquierda
        if(visible)
        {
            int indicatorY = menuStartY + (spacing * selectedOption) + 12; // Adjusted for new height
            int indicatorX = menuCenterX - 20; // Position relative to center
            graph.Text(">", indicatorX, indicatorY);
        }
    }
}

int PMENU::DrawAll()
{
    PAPP::DrawScrollingBackground();
    DrawTitle();
    DrawMenu();
    
    // Dibujar información de debug si está activada
    DrawDebugOverlay(); // Base: FPS, pause state
    
    graph.Flip();
    
    return 1;
}

void PMENU::DrawDebugOverlay()
{
    if (!debugMode) return;
    
    PAPP::DrawDebugOverlay(); // Base: FPS, pause state
    
    // Debug específico del menú
    char cadena[256];
    int y = 80; // Después del overlay base
    int lineHeight = 20;
    
    sprintf(cadena, "Title Y = %d  Selected = %d", this->y, selectedOption);
    graph.Text(cadena, 20, y);
    y += lineHeight;
    
    sprintf(cadena, "Scroll X=%d Y=%d", PAPP::scrollX, PAPP::scrollY);
    graph.Text(cadena, 20, y);
}

void * PMENU::MoveAll()
{
    static bool upPressed = false;
    static bool downPressed = false;
    static bool enterPressed = false;

    if(cont>0) cont--;
    else cont = 30;

    // Actualizar el fondo scrolling compartido
    PAPP::UpdateScrollingBackground();

    // Mover el título hacia abajo más rápido y detenerse más arriba
    if(y<50) y+=10; // Posición final en y=50, más rápido
    else
        if(!cont) visible = !visible;

    // Navegación del menú
    if(y >= 50) // Solo permitir navegación cuando el título está en posición
    {
        if(input.Key(SDL_SCANCODE_UP) || input.Key(gameinf.keys[PLAYER1].left))
        {
            if(!upPressed)
            {
                selectedOption--;
                if(selectedOption < 0)
                    selectedOption = 2;
                upPressed = true;
            }
        }
        else
            upPressed = false;
            
        if(input.Key(SDL_SCANCODE_DOWN) || input.Key(gameinf.keys[PLAYER1].right))
        {
            if(!downPressed)
            {
                selectedOption++;
                if(selectedOption > 2)
                    selectedOption = 0;
                downPressed = true;
            }
        }
        else
            downPressed = false;
        
        // Seleccionar opción
        if(input.Key(SDL_SCANCODE_RETURN) || input.Key(gameinf.keys[PLAYER1].shoot))
        {
            if(!enterPressed)
            {
                switch(selectedOption)
                {
                    case 0: // JUGAR
                        return new PSELECT();
                    
                    case 1: // CONFIGURACIÓN
                        return new PCONFIGSCREEN();
                    
                    case 2: // SALIR
                        quit = 1;
                        return NULL;
                }
                enterPressed = true;
            }
        }
        else
            enterPressed = false;
    }
    
    return NULL;
}