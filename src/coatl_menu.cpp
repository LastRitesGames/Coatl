#include "coatl_pch.hpp"

#include "coatl_menu.hpp"

#include "coatl_screen.hpp"
#include "coatl_sprite.hpp"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    #define MASK_R 0xff000000
    #define MASK_G 0x00ff0000
    #define MASK_B 0x0000ff00
    #define MASK_A 0x000000ff
#else
    #define MASK_R 0x000000ff
    #define MASK_G 0x0000ff00
    #define MASK_B 0x00ff0000
    #define MASK_A 0xff000000
#endif

namespace Coatl
{
    Menu::Menu(Game& game)
        :   GameAccess(game),
            m_sprite(NULL)
    {
        SDL_Color color = {255, 0, 0, 255};
        const unsigned int count = 6;
        Sprite* lines[count];
        lines[0] = Sprite::CreateFromText(game.GetGfxManager(), "menu", "ARROW KEYS   CHANGE DIRECTION", color);
        lines[1] = Sprite::CreateFromText(game.GetGfxManager(), "menu", "F2           NEW GAME", color);
        lines[2] = Sprite::CreateFromText(game.GetGfxManager(), "menu", "P            PAUSE", color);
        lines[3] = Sprite::CreateFromText(game.GetGfxManager(), "menu", "S            SOUND ON/OFF", color);
        lines[4] = Sprite::CreateFromText(game.GetGfxManager(), "menu", "F11          SCREENSHOT", color);
        lines[5] = Sprite::CreateFromText(game.GetGfxManager(), "menu", "ESC          MENU/EXIT", color);
        unsigned int width = 0;
        unsigned int height = 0;
        for (unsigned int i = 0; i < count; ++i)
        {
            if (width < lines[i]->GetWidth())
            {
                width = lines[i]->GetWidth();
            }
            height += lines[i]->GetHeight();
        }
        SDL_Surface* sdl_surface = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, game.GetScreen().GetBitsPerPixel(), MASK_R, MASK_G, MASK_B, MASK_A);
        SDL_FillRect(sdl_surface, NULL, SDL_MapRGBA(game.GetScreen().GetSDLSurface()->format, 0, 0, 0, 0));
        height = 0;
        for (unsigned int i = 0; i < count; ++i)
        {
            SDL_Rect src = lines[i]->GetSDLRect();
            SDL_Rect dst = {0, static_cast<Sint16>(height), src.w, src.h};
            SDL_SetAlpha(lines[i]->GetSDLSurface(), 0, SDL_ALPHA_OPAQUE);
            SDL_BlitSurface(lines[i]->GetSDLSurface(), &src, sdl_surface, &dst);
            height += lines[i]->GetHeight();
        }
        SDL_Rect sdl_rect = {0, 0, static_cast<Uint16>(sdl_surface->w), static_cast<Uint16>(sdl_surface->h)};
        m_sprite = Sprite::CreateFromSDLSurface(sdl_surface, sdl_rect, true);
    }

    Menu::~Menu()
    {
        Sprite::DestroySprite(m_sprite);
    }

    void Menu::Render()
    {
        Screen& screen = GetGame().GetScreen();
        m_sprite->Render(screen, (screen.GetWidth() / 2) - (m_sprite->GetWidth() / 2), (screen.GetHeight() / 2) - (m_sprite->GetHeight() / 2));
    }
}
