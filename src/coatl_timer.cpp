#include "coatl_pch.hpp"

#include "coatl_timer.hpp"

namespace Coatl
{
    Timer::Timer()
        :   m_current_tick(0),
            m_last_tick(0)
    {
        Reset();
    }

    Timer::~Timer()
    {

    }

    Uint32 Timer::GetCurrentTick() const
    {
        return m_current_tick;
    }

    Uint32 Timer::GetLastTick() const
    {
        return m_last_tick;
    }

    Uint32 Timer::GetInterval() const
    {
        return m_current_tick - m_last_tick;
    }

    void Timer::Reset()
    {
        m_current_tick = SDL_GetTicks();
        m_last_tick = m_current_tick;
    }

    void Timer::Update()
    {
        m_last_tick = m_current_tick;
        m_current_tick = SDL_GetTicks();
    }
}
