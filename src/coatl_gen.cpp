#include "coatl_pch.hpp"

#include "coatl_gen.hpp"

#include "coatl_map.hpp"
#include "coatl_tile.hpp"
#include "coatl_random.hpp"

namespace Coatl
{
    Map* Generator::CreateMap(Game& game, unsigned int width, unsigned int height, unsigned int tile_width, unsigned int tile_height)
    {
        Map* map = new Map(game, width, height, tile_width, tile_height);

        // Generate water tiles on the edge of the map
        noise::module::Perlin perlin;
        perlin.SetSeed(Random::UInt());
        perlin.SetNoiseQuality(noise::QUALITY_BEST);
        perlin.SetLacunarity(1.0f);
        Tile* tile;
        for (unsigned int x = 0; x != width; ++x)
        {
            double size = abs(perlin.GetValue(x * .02, 0, 0) * 16.0);
            for (unsigned int i = 0; i < size + 16; ++i)
            {
                tile = map->GetTile(x, i);
                if (tile)
                {
                    tile->SetType(TT_WATER);
                }
                tile = map->GetTile(x, height - i - 1);
                if (tile)
                {
                    tile->SetType(TT_WATER);
                }
            }
        }
        for (unsigned int y = 0; y != height; ++y)
        {
            double size = abs(perlin.GetValue(0, y * .02, 0) * 16.0);
            for (unsigned int i = 0; i < size + 16; ++i)
            {
                tile = map->GetTile(i, y);
                if (tile)
                {
                    tile->SetType(TT_WATER);
                }
                tile = map->GetTile(width - i - 1, y);
                if (tile)
                {
                    tile->SetType(TT_WATER);
                }
            }
        }

        // Generate forest or grass tiles on remaining tiles
        perlin.SetSeed(Random::UInt());
        perlin.SetNoiseQuality(noise::QUALITY_BEST);
        perlin.SetLacunarity(1.0f);
        for (unsigned int x = 1; x != width; ++x)
        {
            for (unsigned int y = 1; y != height; ++y)
            {
                tile = map->GetTile(x, y);
                if (tile->GetType() != TT_WATER)
                {
                    double value = perlin.GetValue(x * .02, y * .02, 0);
                    if (value <= -.3 || value >= .3)
                    {
                        tile->SetType(TT_FOREST);
                    }
                    else
                    {
                        tile->SetType(TT_GRASS);
                    }
                }
            }
        }

        // Remove unreachable tiles
        PostProcessMap(*map);

        return map;
    }

    void Generator::PostProcessMap(Map& map)
    {
        // Sort tiles by connectivity
        std::map<std::pair<unsigned int, unsigned int>, unsigned int> tiles;
        unsigned int id = 0;
        for (Map::TileIterator itr = map.GetTilesBegin(); itr != map.GetTilesEnd(); ++itr)
        {
            if (!itr->second->IsBlocked())
            {
                std::map<std::pair<unsigned int, unsigned int>, unsigned int>::iterator itr2 = tiles.find(itr->first);
                if (itr2 == tiles.end())
                {
                    tiles.insert(std::make_pair(itr->first, id));
                    PostProcessTile(map, *itr->second, tiles, id);
                    ++id;
                }
            }
        }

        // Find largest connected area
        std::vector<unsigned int> count;
        for (unsigned int i = 0; i != id; ++i)
        {
            count.push_back(0);
        }
        for (std::map<std::pair<unsigned int, unsigned int>, unsigned int>::iterator itr = tiles.begin(); itr != tiles.end(); ++itr)
        {
            ++count[itr->second];
        }
        unsigned int max = 0;
        unsigned int max_id = 0;
        for (unsigned int i = 0; i != id; ++i)
        {
            if (count[i] > max)
            {
                max = count[i];
                max_id = i;
            }
        }

        // Blcok all tiles that aren't connected to the largest connected area
        for (std::map<std::pair<unsigned int, unsigned int>, unsigned int>::iterator itr = tiles.begin(); itr != tiles.end(); ++itr)
        {
            if (itr->second != max_id)
            {
                Tile* tile = map.GetTile(itr->first.first, itr->first.second);
                tile->SetType(TT_FOREST);
            }
        }
    }

    void Generator::PostProcessTile(Map& map, Tile& tile, std::map<std::pair<unsigned int, unsigned int>, unsigned int>& tiles, unsigned int id)
    {
        unsigned int x = tile.GetX();
        unsigned int y = tile.GetY();
        Tile* tile2 = map.GetTile(x - 1, y);
        if (tile2 && !tile2->IsBlocked())
        {
            std::map<std::pair<unsigned int, unsigned int>, unsigned int>::iterator itr = tiles.find(std::make_pair(tile2->GetX(), tile2->GetY()));
            if (itr == tiles.end())
            {
                tiles.insert(std::make_pair(std::make_pair(tile2->GetX(), tile2->GetY()), id));
                PostProcessTile(map, *tile2, tiles, id);
            }
        }
        tile2 = map.GetTile(x + 1, y);
        if (tile2 && !tile2->IsBlocked())
        {
            std::map<std::pair<unsigned int, unsigned int>, unsigned int>::iterator itr = tiles.find(std::make_pair(tile2->GetX(), tile2->GetY()));
            if (itr == tiles.end())
            {
                tiles.insert(std::make_pair(std::make_pair(tile2->GetX(), tile2->GetY()), id));
                PostProcessTile(map, *tile2, tiles, id);
            }
        }
        tile2 = map.GetTile(x, y - 1);
        if (tile2 && !tile2->IsBlocked())
        {
            std::map<std::pair<unsigned int, unsigned int>, unsigned int>::iterator itr = tiles.find(std::make_pair(tile2->GetX(), tile2->GetY()));
            if (itr == tiles.end())
            {
                tiles.insert(std::make_pair(std::make_pair(tile2->GetX(), tile2->GetY()), id));
                PostProcessTile(map, *tile2, tiles, id);
            }
        }
        tile2 = map.GetTile(x, y + 1);
        if (tile2 && !tile2->IsBlocked())
        {
            std::map<std::pair<unsigned int, unsigned int>, unsigned int>::iterator itr = tiles.find(std::make_pair(tile2->GetX(), tile2->GetY()));
            if (itr == tiles.end())
            {
                tiles.insert(std::make_pair(std::make_pair(tile2->GetX(), tile2->GetY()), id));
                PostProcessTile(map, *tile2, tiles, id);
            }
        }
    }

    void Generator::DestroyMap(Map* map)
    {
        delete map;
    }
}
