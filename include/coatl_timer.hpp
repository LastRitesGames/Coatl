#ifndef COATL_TIMER_HPP
#define COATL_TIMER_HPP

namespace Coatl
{
    class Timer
    {
        public:
            Timer();
            ~Timer();

            Uint32 GetCurrentTick() const;
            Uint32 GetLastTick() const;
            Uint32 GetInterval() const;

            void Reset();

            void Update();

        private:
            Uint32 m_current_tick;
            Uint32 m_last_tick;
    };
}

#endif // COATL_TIMER_HPP
