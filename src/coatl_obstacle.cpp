#include "coatl_pch.hpp"

#include "coatl_obstacle.hpp"

#include "coatl_snake.hpp"

namespace Coatl
{
    Obstacle::Obstacle(Game& game, unsigned int x, unsigned int y)
        :   GameAccess(game),
            CollisionObject(game.GetCollisionSystem(), x, y, CT_OBSTACLE)
    {

    }

    Obstacle::~Obstacle()
    {

    }

    void Obstacle::Collide(Snake& snake)
    {
        snake.Kill();
    }
}
