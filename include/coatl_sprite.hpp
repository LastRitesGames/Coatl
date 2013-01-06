#ifndef COATL_SPRITE_HPP
#define COATL_SPRITE_HPP

namespace Coatl
{
    class FontManager;
    class GfxManager;
    class RenderContext;
    class Screen;
    class Texture;

    class Sprite
    {
        public:
            static Sprite* CreateFromSDLSurface(SDL_Surface* sdl_surface, const SDL_Rect& sdl_rect, bool owns_surface);
            static Sprite* CreateFromTexture(GfxManager& gfx_manager, const std::string& name);
            static Sprite* CreateFromTextureGroup(GfxManager& gfx_manager, const std::string& name);
            static Sprite* CreateFromText(GfxManager& gfx_manager, const std::string& font_name, const std::string& text, const SDL_Color& color);
            static void DestroySprite(Sprite* sprite);

        private:
            Sprite(SDL_Surface* sdl_surface, const SDL_Rect& rect, bool owns_surface);
        public:
            ~Sprite();

            unsigned int GetWidth() const;
            unsigned int GetHeight() const;
            const SDL_Rect& GetSDLRect() const;
            SDL_Surface* GetSDLSurface();
            const SDL_Surface* GetSDLSurface() const;

            void Render(Screen& sdl_screen, unsigned int x, unsigned int y);
            void Render(Screen& sdl_screen, unsigned int x, unsigned int y, unsigned int offset_x, unsigned int offset_y);

        private:
            SDL_Surface* m_sdl_surface;
            SDL_Rect m_sdl_rect;
            bool m_owns_surface;
    };
}

#endif // COATL_SPRITE_HPP
