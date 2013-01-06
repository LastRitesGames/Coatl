#include "coatl_pch.hpp"

#include "coatl_view.hpp"

#include "coatl_apple.hpp"
#include "coatl_gfx.hpp"
#include "coatl_map.hpp"
#include "coatl_screen.hpp"
#include "coatl_snake.hpp"
#include "coatl_sprite.hpp"
#include "coatl_tile.hpp"

namespace Coatl
{
    ViewRect::ViewRect()
        :   Left(0),
            Right(0),
            Top(0),
            Bottom(0)
    {

    }

    ViewRect::ViewRect(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom)
        :   Left(left),
            Right(right),
            Top(top),
            Bottom(bottom)
    {

    }

    ViewRect::~ViewRect()
    {

    }

    bool ViewRect::Contains(unsigned int x, unsigned int y) const
    {
        return x >= Left && x <= Right && y >= Top && y <= Bottom;
    }

    bool ViewRect::Contains(const ViewRect& rect) const
    {
        return Contains(rect.Left, rect.Top) || Contains(rect.Left, rect.Bottom) || Contains(rect.Right, rect.Top) || Contains(rect.Right, rect.Bottom);
    }

    View::View(Game& game)
        :   GameAccess(game),
            m_focus_x(0),
            m_focus_y(0),
            m_rect(),
            m_sprite(NULL)
    {
        GfxManager& gfx_manager = game.GetGfxManager();
        TextureGroup* group_forest = gfx_manager.GetTextureGroup("forest");
        Texture* forest[16];
        forest[0] = group_forest->GetTexture("forest00");
        forest[1] = group_forest->GetTexture("forest01");
        forest[2] = group_forest->GetTexture("forest02");
        forest[3] = group_forest->GetTexture("forest03");
        forest[4] = group_forest->GetTexture("forest04");
        forest[5] = group_forest->GetTexture("forest05");
        forest[6] = group_forest->GetTexture("forest06");
        forest[7] = group_forest->GetTexture("forest07");
        forest[8] = group_forest->GetTexture("forest08");
        forest[9] = group_forest->GetTexture("forest09");
        forest[10] = group_forest->GetTexture("forest10");
        forest[11] = group_forest->GetTexture("forest11");
        forest[12] = group_forest->GetTexture("forest12");
        forest[13] = group_forest->GetTexture("forest13");
        forest[14] = group_forest->GetTexture("forest14");
        forest[15] = group_forest->GetTexture("forest15");
        TextureGroup* group_water = gfx_manager.GetTextureGroup("water");
        Texture* water[16];
        water[0] = group_water->GetTexture("water00");
        water[1] = group_water->GetTexture("water01");
        water[2] = group_water->GetTexture("water02");
        water[3] = group_water->GetTexture("water03");
        water[4] = group_water->GetTexture("water04");
        water[5] = group_water->GetTexture("water05");
        water[6] = group_water->GetTexture("water06");
        water[7] = group_water->GetTexture("water07");
        water[8] = group_water->GetTexture("water08");
        water[9] = group_water->GetTexture("water09");
        water[10] = group_water->GetTexture("water10");
        water[11] = group_water->GetTexture("water11");
        water[12] = group_water->GetTexture("water12");
        water[13] = group_water->GetTexture("water13");
        water[14] = group_water->GetTexture("water14");
        water[15] = group_water->GetTexture("water15");

        Map& map = game.GetMap();
        SDL_Surface* sdl_surface = SDL_CreateRGBSurface(SDL_HWSURFACE, map.GetWidth() * map.GetTileWidth(), map.GetHeight() * map.GetTileHeight(), game.GetScreen().GetBitsPerPixel(), 0x00000000, 0x00000000, 0x00000000, 0x00000000);
        SDL_Rect sdl_rect;
        sdl_rect.x = 0;
        sdl_rect.y = 0;
        sdl_rect.w = map.GetTileWidth();
        sdl_rect.h = map.GetTileHeight();
        TextureGroup* group_grass = gfx_manager.GetTextureGroup("grass");
        unsigned int mask;
        const Tile* tile;
        Texture* texture;
        for (Map::TileConstIterator itr = map.GetTilesBegin(); itr !=map.GetTilesEnd(); ++itr)
        {
            sdl_rect.x = itr->second->GetX() * map.GetTileWidth();
            sdl_rect.y = itr->second->GetY() * map.GetTileHeight();
            TileType type = itr->second->GetType();
            if (type == TT_FOREST || type == TT_WATER)
            {
                mask = 0;
                tile = map.GetTile(itr->second->GetX(), itr->second->GetY() - 1);
                if (tile && tile->GetType() != type)
                {
                    mask |= 1;
                }
                tile = map.GetTile(itr->second->GetX() + 1, itr->second->GetY());
                if (tile && tile->GetType() != type)
                {
                    mask |= 2;
                }
                tile = map.GetTile(itr->second->GetX(), itr->second->GetY() + 1);
                if (tile && tile->GetType() != type)
                {
                    mask |= 4;
                }
                tile = map.GetTile(itr->second->GetX() - 1, itr->second->GetY());
                if (tile && tile->GetType() != type)
                {
                    mask |= 8;
                }

                if (type == TT_FOREST)
                {
                    texture = forest[mask];
                }
                else if (type == TT_WATER)
                {
                    texture = water[mask];
                }
            }
            else
            {
                texture = group_grass->GetRandomTexture();
            }
            SDL_Rect dst = texture->GetSDLRect();
            SDL_BlitSurface(texture->GetSDLSurface(), &dst, sdl_surface, &sdl_rect);
        }
        sdl_rect.x = 0;
        sdl_rect.y = 0;
        sdl_rect.w = sdl_surface->w;
        sdl_rect.h = sdl_surface->h;
        m_sprite = Sprite::CreateFromSDLSurface(sdl_surface, sdl_rect, true);
    }

    View::~View()
    {
        Sprite::DestroySprite(m_sprite);
    }

    unsigned int View::GetX() const
    {
        return m_focus_x;
    }

    unsigned int View::GetY() const
    {
        return m_focus_y;
    }

    const ViewRect& View::GetRect() const
    {
        return m_rect;
    }

    std::pair<int, int> View::GetScreenPosition(unsigned int x, unsigned int y) const
    {
        return std::make_pair(x - m_rect.Left, y - m_rect.Top);
    }

    void View::SetFocus(unsigned int x, unsigned int y)
    {
        m_focus_x = x;
        m_focus_y = y;

        Game& game = GetGame();
        Map& map = game.GetMap();
        SDL_Surface* sdl_surface = game.GetScreen().GetSDLSurface();
        int xx = (x * map.GetTileWidth()) + (map.GetTileWidth() / 2) - (sdl_surface->w / 2);
        int yy = (y * map.GetTileHeight()) + (map.GetTileHeight() / 2) - (sdl_surface->h / 2);
        if (xx < 0)
        {
            xx = 0;
        }
        else if (xx + sdl_surface->w > static_cast<int>(map.GetWidth() * map.GetTileWidth()))
        {
            xx = (map.GetWidth() * map.GetTileWidth()) - sdl_surface->w;
        }
        if (yy < 0)
        {
            yy = 0;
        }
        else if (yy + sdl_surface->h > static_cast<int>(map.GetHeight() * map.GetTileHeight()))
        {
            yy = (map.GetHeight() * map.GetTileHeight()) - sdl_surface->h;
        }
        m_rect.Left = xx;
        m_rect.Right = xx + sdl_surface->w;
        m_rect.Top = yy;
        m_rect.Bottom = yy + sdl_surface->h;
    }

    void View::Render()
    {
        m_sprite->Render(GetGame().GetScreen(), 0, 0, m_rect.Left, m_rect.Top);

        Game& game = GetGame();
        Map& map = game.GetMap();
        for (Map::AppleIterator itr = map.GetApplesBegin(); itr != map.GetApplesEnd(); ++itr)
        {
            if (m_rect.Contains((*itr)->GetRect()))
            {
                (*itr)->Render();
            }
        }

        for (Map::SnakeIterator itr = map.GetSnakesBegin(); itr != map.GetSnakesEnd(); ++itr)
        {
            (*itr)->Render();
        }

        Snake& snake = game.GetSnake();
        snake.Render();
    }
}
