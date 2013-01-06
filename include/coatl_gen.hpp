#ifndef COATL_MAP_GEN_HPP
#define COATL_MAP_GEN_HPP

namespace Coatl
{
    class Game;
    class Map;
    class Tile;

    class Generator
    {
        public:
            static Map* CreateMap(Game& game, unsigned int width, unsigned int height, unsigned int tile_width, unsigned int tile_height);
            static void PostProcessMap(Map& map);
            static void PostProcessTile(Map& map, Tile& tile, std::map<std::pair<unsigned int, unsigned int>, unsigned int>& tiles, unsigned int id);
            static void DestroyMap(Map* map);
    };
}

#endif // COATL_MAP_GEN_HPP
