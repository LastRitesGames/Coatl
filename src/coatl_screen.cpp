#include "coatl_pch.hpp"

#include "coatl_screen.hpp"

namespace Coatl
{
    Screen::Screen(unsigned int width, unsigned int height, unsigned int bits_per_pixel, bool windowed)
        :   m_width(width),
            m_height(height),
            m_bits_per_pixel(bits_per_pixel),
            m_windowed(windowed)
    {

    }

    Screen::~Screen()
    {

    }

    SDL_Surface* Screen::GetSDLSurface()
    {
        return m_sdl_surface;
    }

    const SDL_Surface* Screen::GetSDLSurface() const
    {
        return m_sdl_surface;
    }

    unsigned int Screen::GetWidth() const
    {
        return m_width;
    }

    unsigned int Screen::GetHeight() const
    {
        return m_height;
    }

    unsigned int Screen::GetBitsPerPixel() const
    {
        return m_bits_per_pixel;
    }

    bool Screen::IsWindowed() const
    {
        return m_windowed;
    }

    bool Screen::Init()
    {
        Uint32 flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
        if (!m_windowed)
        {
            flags |= SDL_FULLSCREEN;
        }
        m_sdl_surface = SDL_SetVideoMode(m_width, m_height, m_bits_per_pixel, flags);
        return m_sdl_surface != NULL;
    }

    void Screen::Capture()
    {
        SDL_SaveBMP(m_sdl_surface, "screenshot.bmp");
    }
}
