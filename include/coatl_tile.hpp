#ifndef COATL_TILE_HPP
#define COATL_TILE_HPP

#include "coatl_game.hpp"

namespace Coatl
{
    class Obstacle;

    typedef enum
    {
        TT_GRASS,
        TT_FOREST,
        TT_WATER
    } TileType;

    class Tile
        :   public GameAccess
    {
        public:
            Tile(Game& game, unsigned int x, unsigned int y, TileType type);
            ~Tile();

            unsigned int GetX() const;
            unsigned int GetY() const;
            TileType GetType() const;
            bool IsBlocked() const;

            void SetType(TileType type);

        private:
            unsigned int m_x;
            unsigned int m_y;
            TileType m_type;
            Obstacle* m_obstacle;
    };
}

#endif // COATL_TILE_HPP
