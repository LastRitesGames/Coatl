#ifndef COATL_MAP_HPP
#define COATL_MAP_HPP

#include "coatl_game.hpp"

namespace Coatl
{
    class Apple;
    class Map;
    class Snake;
    class SnakeAIController;
    class Tile;

    class Map
        :   public GameAccess
    {
        public:
            typedef std::map<std::pair<unsigned int, unsigned int>, Tile*> TileList;
            typedef TileList::iterator TileIterator;
            typedef TileList::const_iterator TileConstIterator;
            typedef std::vector<Apple*> AppleList;
            typedef AppleList::iterator AppleIterator;
            typedef AppleList::const_iterator AppleConstIterator;
            typedef std::vector<Snake*> SnakeList;
            typedef SnakeList::iterator SnakeIterator;
            typedef SnakeList::const_iterator SnakeConstIterator;
            typedef std::vector<SnakeAIController*> ControllerList;
            typedef ControllerList::iterator ControllerIterator;
            typedef ControllerList::const_iterator ControllerConstIterator;

            Map(Game& game, unsigned int width, unsigned int height, unsigned int tile_width, unsigned int tile_height);
            ~Map();

            unsigned int GetWidth() const;
            unsigned int GetHeight() const;
            unsigned int GetTileWidth() const;
            unsigned int GetTileHeight() const;
            size_t GetTileCount() const;
            TileIterator GetTilesBegin();
            TileIterator GetTilesEnd();
            TileConstIterator GetTilesBegin() const;
            TileConstIterator GetTilesEnd() const;
            Tile* GetTile(unsigned int x, unsigned int y);
            const Tile* GetTile(unsigned int x, unsigned int y) const;
            size_t GetAppleCount() const;
            AppleIterator GetApplesBegin();
            AppleIterator GetApplesEnd();
            AppleConstIterator GetApplesBegin() const;
            AppleConstIterator GetApplesEnd() const;
            size_t GetSnakeCount() const;
            SnakeIterator GetSnakesBegin();
            SnakeIterator GetSnakesEnd();
            SnakeConstIterator GetSnakesBegin() const;
            SnakeConstIterator GetSnakesEnd() const;

            void RemoveApple(Apple* apple);

            void Update();

        private:
            unsigned int m_width;
            unsigned int m_height;
            unsigned int m_tile_width;
            unsigned int m_tile_height;
            TileList m_tiles;
            AppleList m_apples;
            SnakeList m_snakes;
            ControllerList m_controllers;
            Uint32 m_interval;
    };
}

#endif // COATL_MAP_HPP
