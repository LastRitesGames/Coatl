#include "coatl_pch.hpp"

#include "coatl_sprite.hpp"

#include "coatl_gfx.hpp"
#include "coatl_screen.hpp"

namespace Coatl
{
    Sprite* Sprite::CreateFromSDLSurface(SDL_Surface* sdl_surface, const SDL_Rect& sdl_rect, bool owns_surface)
    {
        return new Sprite(sdl_surface, sdl_rect, owns_surface);
    }

    Sprite* Sprite::CreateFromTexture(GfxManager& gfx_manager, const std::string& name)
    {
        Sprite* sprite = NULL;
        Texture* texture = gfx_manager.GetTexture(name);
        if (texture)
        {
            sprite = new Sprite(texture->GetSDLSurface(), texture->GetSDLRect(), false);
        }
        return sprite;
    }

    Sprite* Sprite::CreateFromTextureGroup(GfxManager& gfx_manager, const std::string& name)
    {
        Sprite* sprite = NULL;
        TextureGroup* group = gfx_manager.GetTextureGroup(name);
        if (group)
        {
            Texture* texture = group->GetRandomTexture();
            if (texture)
            {
                sprite = new Sprite(texture->GetSDLSurface(), texture->GetSDLRect(), false);
            }
        }
        return sprite;
    }

    Sprite* Sprite::CreateFromText(GfxManager& gfx_manager, const std::string& font_name, const std::string& text, const SDL_Color& color)
    {
        Sprite* sprite = NULL;
        Font* font = gfx_manager.GetFont(font_name);
        if (font)
        {
            SDL_Surface* sdl_surface = font->RenderText(text, color);
            SDL_Rect sdl_rect;
            sdl_rect.x = 0;
            sdl_rect.y = 0;
            sdl_rect.w = sdl_surface->w;
            sdl_rect.h = sdl_surface->h;
            sprite = new Sprite(sdl_surface, sdl_rect, true);
        }
        return sprite;
    }

    void Sprite::DestroySprite(Sprite* sprite)
    {
        delete sprite;
    }

    Sprite::Sprite(SDL_Surface* sdl_surface, const SDL_Rect& sdl_rect, bool owns_surface)
        :   m_sdl_surface(sdl_surface),
            m_sdl_rect(sdl_rect),
            m_owns_surface(owns_surface)
    {

    }

    Sprite::~Sprite()
    {
        if (m_owns_surface)
        {
            SDL_FreeSurface(m_sdl_surface);
        }
    }

    unsigned int Sprite::GetWidth() const
    {
        return m_sdl_rect.w;
    }

    unsigned int Sprite::GetHeight() const
    {
        return m_sdl_rect.h;
    }

    const SDL_Rect& Sprite::GetSDLRect() const
    {
        return m_sdl_rect;
    }

    SDL_Surface* Sprite::GetSDLSurface()
    {
        return m_sdl_surface;
    }

    const SDL_Surface* Sprite::GetSDLSurface() const
    {
        return m_sdl_surface;
    }

    void Sprite::Render(Screen& screen, unsigned int x, unsigned int y)
    {
        SDL_Rect dst;
        dst.x = x;
        dst.y = y;
        dst.w = m_sdl_rect.w;
        dst.h = m_sdl_rect.h;
        SDL_BlitSurface(m_sdl_surface, &m_sdl_rect, screen.GetSDLSurface(), &dst);
    }

    void Sprite::Render(Screen& screen, unsigned int x, unsigned int y, unsigned int offset_x, unsigned int offset_y)
    {
        SDL_Rect src = m_sdl_rect;
        src.x = offset_x;
        src.y = offset_y;
        SDL_Rect dst;
        dst.x = x;
        dst.y = y;
        dst.w = m_sdl_rect.w;
        dst.h = m_sdl_rect.h;
        SDL_BlitSurface(m_sdl_surface, &src, screen.GetSDLSurface(), &dst);
    }
}
