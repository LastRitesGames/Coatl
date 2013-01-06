#ifndef COATL_SCREEN_HPP
#define COATL_SCREEN_HPP

namespace Coatl
{
    class Screen
    {
        public:
            Screen(unsigned int width, unsigned int height, unsigned int bits_per_pixel, bool windowed);
            ~Screen();

            unsigned int GetWidth() const;
            unsigned int GetHeight() const;
            unsigned int GetBitsPerPixel() const;
            bool IsWindowed() const;
            SDL_Surface* GetSDLSurface();
            const SDL_Surface* GetSDLSurface() const;

            bool Init();

            void Capture();

        private:
            unsigned int m_width;
            unsigned int m_height;
            unsigned int m_bits_per_pixel;
            bool m_windowed;
            SDL_Surface* m_sdl_surface;
    };
}

#endif // COATL_SCREEN_HPP
