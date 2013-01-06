#ifndef COATL_MAP_VIEW_HPP
#define COATL_MAP_VIEW_HPP

#include "coatl_game.hpp"

namespace Coatl
{
    class Map;
    class Sprite;

    class ViewRect
    {
        public:
            ViewRect();
            ViewRect(unsigned int left, unsigned int right, unsigned int top, unsigned int bottom);
            ~ViewRect();

            bool Contains(unsigned int x, unsigned int y) const;
            bool Contains(const ViewRect& rect) const;

            unsigned int Left;
            unsigned int Right;
            unsigned int Top;
            unsigned int Bottom;
    };

    class View
        :   public GameAccess
    {
        public:
            View(Game &game);
            virtual ~View();

            unsigned int GetX() const;
            unsigned int GetY() const;
            const ViewRect& GetRect() const;
            std::pair<int, int> GetScreenPosition(unsigned int x, unsigned int y) const;

            void SetFocus(unsigned int x, unsigned int y);

            void Render();

        private:
            unsigned int m_focus_x;
            unsigned int m_focus_y;
            ViewRect m_rect;
            Sprite* m_sprite;
    };
}

#endif // COATL_MAP_VIEW_HPP
