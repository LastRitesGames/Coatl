#ifndef COATL_APPLE_HPP
#define COATL_APPLE_HPP

#include "coatl_collision.hpp"
#include "coatl_game.hpp"
#include "coatl_view.hpp"

namespace Coatl
{
    class Map;
    class Sound;
    class Sprite;
    class ViewRect;

    class Apple
        :   public GameAccess,
            public CollisionObject
    {
        public:
            Apple(Game& game, unsigned int x, unsigned int y);
            virtual ~Apple();

            const ViewRect& GetRect() const;
            bool WasUsed() const;

            void Render();

            virtual void Collide(Snake& snake);

        private:
            ViewRect m_rect;
            bool m_used;
            Sprite* m_sprite;
    };
}

#endif // COATL_APPLE_HPP
