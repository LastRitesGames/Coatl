#include "coatl_pch.hpp"

#include "coatl_tile.hpp"

#include "coatl_obstacle.hpp"

namespace Coatl
{
    Tile::Tile(Game& game, unsigned int x, unsigned int y, TileType type)
        :   GameAccess(game),
            m_x(x),
            m_y(y),
            m_type(type),
            m_obstacle(NULL)
    {

    }

    Tile::~Tile()
    {

    }

    unsigned int Tile::GetX() const
    {
        return m_x;
    }

    unsigned int Tile::GetY() const
    {
        return m_y;
    }

    TileType Tile::GetType() const
    {
        return m_type;
    }

    void Tile::SetType(TileType type)
    {
        m_type = type;
        if (m_type == TT_FOREST || m_type == TT_WATER)
        {
            if (!m_obstacle)
            {
                m_obstacle = new Obstacle(GetGame(), m_x, m_y);
            }
        }
        else
        {
            delete m_obstacle;
        }
    }

    bool Tile::IsBlocked() const
    {
        return m_obstacle != NULL;
    }
}
