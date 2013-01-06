#ifndef COATL_OBSTACLE_HPP
#define COATL_OBSTACLE_HPP

#include "coatl_collision.hpp"
#include "coatl_game.hpp"

namespace Coatl
{
    class Obstacle
        :   public GameAccess,
            public CollisionObject
    {
        public:
            Obstacle(Game& game, unsigned int x, unsigned int y);
            virtual ~Obstacle();

            virtual void Collide(Snake& snake);
    };
}

#endif // COATL_OBSTACLE_HPP
