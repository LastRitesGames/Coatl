#include "coatl_pch.hpp"

#include "coatl_map.hpp"

#include "coatl_apple.hpp"
#include "coatl_random.hpp"
#include "coatl_snake.hpp"
#include "coatl_status.hpp"
#include "coatl_tile.hpp"
#include "coatl_timer.hpp"

namespace Coatl
{
    class ControllerFinder
    {
        public:
            ControllerFinder(const Snake* snake) : m_snake(snake) {}
            ~ControllerFinder() {}
            bool operator()(const SnakeAIController* controller) { return &controller->GetSnake() == m_snake; }
        private:
            const Snake* m_snake;
    };

    Map::Map(Game& game, unsigned int width, unsigned int height, unsigned int tile_width, unsigned int tile_height)
        :   GameAccess(game),
            m_width(width),
            m_height(height),
            m_tile_width(tile_width),
            m_tile_height(tile_height),
            m_tiles(),
            m_apples(),
            m_snakes(),
            m_controllers(),
            m_interval(0)
    {
        for (unsigned int x = 0; x != m_width; ++x)
        {
            for (unsigned int y = 0; y != m_height; ++y)
            {
                m_tiles.insert(std::make_pair(std::make_pair(x, y), new Tile(game, x, y, TT_GRASS)));
            }
        }
    }

    Map::~Map()
    {
        for (ControllerIterator itr = m_controllers.begin(); itr != m_controllers.end(); ++itr)
        {
            delete *itr;
        }
        for (SnakeIterator itr = m_snakes.begin(); itr != m_snakes.end(); ++itr)
        {
            delete *itr;
        }
        for (AppleIterator itr = m_apples.begin(); itr != m_apples.end(); ++itr)
        {
            delete *itr;
        }
        for (TileIterator itr = m_tiles.begin(); itr != m_tiles.end(); ++itr)
        {
            delete itr->second;
        }
    }

    unsigned int Map::GetWidth() const
    {
        return m_width;
    }

    unsigned int Map::GetHeight() const
    {
        return m_height;
    }

    unsigned int Map::GetTileWidth() const
    {
        return m_tile_width;
    }

    unsigned int Map::GetTileHeight() const
    {
        return m_tile_height;
    }

    size_t Map::GetTileCount() const
    {
        return m_tiles.size();
    }

    Map::TileIterator Map::GetTilesBegin()
    {
        return m_tiles.begin();
    }

    Map::TileIterator Map::GetTilesEnd()
    {
        return m_tiles.end();
    }

    Map::TileConstIterator Map::GetTilesBegin() const
    {
        return m_tiles.begin();
    }

    Map::TileConstIterator Map::GetTilesEnd() const
    {
        return m_tiles.end();
    }

    Tile* Map::GetTile(unsigned int x, unsigned int y)
    {
        Tile* tile = NULL;
        TileIterator itr = m_tiles.find(std::make_pair(x, y));
        if (itr != m_tiles.end())
        {
            tile = itr->second;
        }
        return tile;
    }

    const Tile* Map::GetTile(unsigned int x, unsigned int y) const
    {
        const Tile* tile = NULL;
        TileConstIterator itr = m_tiles.find(std::make_pair(x, y));
        if (itr != m_tiles.end())
        {
            tile = itr->second;
        }
        return tile;
    }

    size_t Map::GetAppleCount() const
    {
        return m_apples.size();
    }

    Map::AppleIterator Map::GetApplesBegin()
    {
        return m_apples.begin();
    }

    Map::AppleIterator Map::GetApplesEnd()
    {
        return m_apples.end();
    }

    Map::AppleConstIterator Map::GetApplesBegin() const
    {
        return m_apples.begin();
    }

    Map::AppleConstIterator Map::GetApplesEnd() const
    {
        return m_apples.end();
    }

    size_t Map::GetSnakeCount() const
    {
        return m_snakes.size();
    }

    Map::SnakeIterator Map::GetSnakesBegin()
    {
        return m_snakes.begin();
    }

    Map::SnakeIterator Map::GetSnakesEnd()
    {
        return m_snakes.end();
    }

    Map::SnakeConstIterator Map::GetSnakesBegin() const
    {
        return m_snakes.begin();
    }

    Map::SnakeConstIterator Map::GetSnakesEnd() const
    {
        return m_snakes.end();
    }

    void Map::RemoveApple(Apple* apple)
    {
        AppleIterator itr = std::find(m_apples.begin(), m_apples.end(), apple);
        if (itr != m_apples.end())
        {
            delete *itr;
            m_apples.erase(itr);
        }
    }

    void Map::Update()
    {
        Game& game = GetGame();
        for (AppleIterator itr = m_apples.begin(); itr != m_apples.end();)
        {
            if ((*itr)->WasUsed())
            {
                delete *itr;
                itr = m_apples.erase(itr);
            }
            else
            {
                ++itr;
            }
        }

        m_interval += game.GetTimer().GetInterval();
        if (m_interval > 1000)
        {
            m_interval -= 1000;
            Status &status = game.GetStatus();
            if (m_apples.size() < status.GetMaxAppleCount())
            {
                std::pair<unsigned int, unsigned int> spawn = game.FindAppleSpawn();
                m_apples.push_back(new Apple(game, spawn.first, spawn.second));
            }
            if (m_snakes.size() < status.GetMaxSnakeAICount())
            {
                std::pair<unsigned int, unsigned int> minmax = status.GetSnakeAILength();
                unsigned int length = Random::UInt(minmax.first, minmax.second);
                std::pair<unsigned int, unsigned int> spawn = game.FindSnakeSpawn(length, false);
                Snake* snake = new Snake(game, false, spawn.first, spawn.second, length, SD_NORTH, 100);
                m_snakes.push_back(snake);
                m_controllers.push_back(new SnakeAIController(game, *snake));
            }
        }

        for (SnakeIterator itr = m_snakes.begin(); itr != m_snakes.end();)
        {
            (*itr)->Update();
            if (!(*itr)->IsAlive())
            {
                ControllerIterator itr2 = std::find_if(m_controllers.begin(), m_controllers.end(), ControllerFinder(*itr));
                delete *itr2;
                m_controllers.erase(itr2);
                delete *itr;
                itr = m_snakes.erase(itr);
            }
            else
            {
                ++itr;
            }
        }

        for (ControllerIterator itr = m_controllers.begin(); itr != m_controllers.end(); ++itr)
        {
            (*itr)->Update();
        }
    }
}
