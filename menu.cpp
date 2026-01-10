#include "pang.h"
#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <cstring>

extern int quit;

int Menu::initBitmaps()
{
    bmp.title.init(&graph, "graph\\title.png", 0, 0);
    graph.setColorKey(bmp.title.getBmp(), 0x00FF00);
    
    // Initialize shared background
    App::initSharedBackground();

    // Load BMFont for menu
    if (fontLoader.load("graph\\font\\thickfont_grad_64.fnt"))
    {
        bmp.menuFont.init(&graph, "graph\\font\\thickfont_grad_64.png", 0, 0);
        fontRenderer.init(&graph, &fontLoader, &bmp.menuFont);
        //fontRenderer.setColor(255, 255, 0, 255); // Yellow
        //fontRenderer.setScale(2.0f); // Scale 2x for readability
    }

    return 1;
}

Menu::Menu()
    : xPos(0), yPos(0), selectedOption(0), visible(false), blinkCounter(0)
{
}

int Menu::init()
{
    App::init();
    
    gameinf.isMenu() = true;
    initBitmaps();
    xPos = 180;
    yPos = -300;

    selectedOption = 0; // PLAY
    
    visible = false;
    blinkCounter = 30;

    CloseMusic();
    OpenMusic("music\\menu.mid");
    PlayMusic();

    return 1;
}

int Menu::release()
{
    bmp.title.release();
    bmp.menuFont.release();
    fontRenderer.release();

    CloseMusic();
    
    return 1;
}

void Menu::drawTitle()
{
    int titleX = (RES_X - bmp.title.getWidth()) / 2;
    int titleY = yPos;
    
    graph.drawClipped(&bmp.title, titleX, titleY);
}

void Menu::drawMenu()
{
    int menuStartY = 320;
    int spacing = 50;
    
    if (fontRenderer.getFont() && fontRenderer.getFontTexture())
    {
        const char* options[3] = {"NEW GAME", "OPTIONS", "EXIT"};
        
        for (int i = 0; i < 3; i++)
        {
            int optionY = menuStartY + (spacing * i);
            int textWidth = fontRenderer.getTextWidth(options[i]);
            int textX = (RES_X - textWidth) / 2;
            
            if (selectedOption != i || visible)
            {
                fontRenderer.text(options[i], textX, optionY);
            }
        }
        
        if (visible)
        {
            int indicatorY = menuStartY + (spacing * selectedOption);
            int textWidth = fontRenderer.getTextWidth(options[selectedOption]);
            int textX = (RES_X - textWidth) / 2;
            int indicatorX = textX - 30;
            fontRenderer.text(">", indicatorX, indicatorY);
        }
    }
}

int Menu::drawAll()
{
    App::drawScrollingBackground();
    drawTitle();
    drawMenu();
    drawDebugOverlay();
    graph.flip();
    return 1;
}

void Menu::drawDebugOverlay()
{
    if (!debugMode) return;
    App::drawDebugOverlay();
    char cadena[256];
    int y = 80;
    int lineHeight = 20;
    std::sprintf(cadena, "Title Y = %d  Selected = %d", yPos, selectedOption);
    graph.text(cadena, 20, y);
    y += lineHeight;
    std::sprintf(cadena, "Scroll X=%d Y=%d", App::scrollX, App::scrollY);
    graph.text(cadena, 20, y);
}

void* Menu::moveAll()
{
    static bool upPressed = false;
    static bool downPressed = false;
    static bool enterPressed = false;

    if (blinkCounter > 0) blinkCounter--;
    else blinkCounter = 30;

    App::updateScrollingBackground();

    if (yPos < 50) yPos += 10;
    else if (!blinkCounter) visible = !visible;

    if (yPos >= 50)
    {
        if (input.key(SDL_SCANCODE_UP) || input.key(gameinf.getKeys()[PLAYER1].left))
        {
            if (!upPressed)
            {
                selectedOption--;
                if (selectedOption < 0) selectedOption = 2;
                upPressed = true;
            }
        }
        else upPressed = false;
            
        if (input.key(SDL_SCANCODE_DOWN) || input.key(gameinf.getKeys()[PLAYER1].right))
        {
            if (!downPressed)
            {
                selectedOption++;
                if (selectedOption > 2) selectedOption = 0;
                downPressed = true;
            }
        }
        else downPressed = false;
        
        if (input.key(SDL_SCANCODE_RETURN) || input.key(gameinf.getKeys()[PLAYER1].shoot))
        {
            if (!enterPressed)
            {
                switch (selectedOption)
                {
                    case 0: return new SelectSync(); // Select class
                    case 1: return new ConfigScreen();
                    case 2: quit = 1; return nullptr;
                }
                enterPressed = true;
            }
        }
        else enterPressed = false;
    }
    
    return nullptr;
}