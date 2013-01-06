#include "coatl_pch.hpp"

#include "coatl_game.hpp"

#include "coatl_apple.hpp"
#include "coatl_collision.hpp"
#include "coatl_config.hpp"
#include "coatl_gen.hpp"
#include "coatl_gfx.hpp"
#include "coatl_menu.hpp"
#include "coatl_map.hpp"
#include "coatl_random.hpp"
#include "coatl_screen.hpp"
#include "coatl_sfx.hpp"
#include "coatl_snake.hpp"
#include "coatl_sprite.hpp"
#include "coatl_status.hpp"
#include "coatl_tile.hpp"
#include "coatl_timer.hpp"
#include "coatl_view.hpp"

namespace Coatl
{
    void Game::Run()
    {
        Game* game = new Game();
        if (game->Init())
        {
            game->StartNewGame(true);
            game->Loop();
        }
        delete game;
    }

    Game::Game()
        :   m_configuration(NULL),
            m_screen(NULL),
            m_gfx_manager(NULL),
            m_sfx_manager(NULL),
            m_status(NULL),
            m_collision_system(NULL),
            m_map(NULL),
            m_view(NULL),
            m_snake(NULL),
            m_controller(NULL),
            m_timer(NULL),
            m_menu(NULL),
            m_autoplay(false),
            m_pause(false)
    {

    }

    Game::~Game()
    {
        delete m_timer;
        delete m_controller;
        delete m_snake;
        delete m_view;
        Generator::DestroyMap(m_map);
        delete m_collision_system;
        delete m_status;
        delete m_sfx_manager;
        delete m_gfx_manager;
        delete m_screen;
        delete m_configuration;
    }

    Configuration& Game::GetConfiguration()
    {
        return *m_configuration;
    }

    const Configuration& Game::GetConfiguration() const
    {
        return *m_configuration;
    }

    Screen& Game::GetScreen()
    {
        return *m_screen;
    }

    const Screen& Game::GetScreen() const
    {
        return *m_screen;
    }

    GfxManager& Game::GetGfxManager()
    {
        return *m_gfx_manager;
    }

    const GfxManager& Game::GetGfxManager() const
    {
        return *m_gfx_manager;
    }

    SfxManager& Game::GetSfxManager()
    {
        return *m_sfx_manager;
    }

    const SfxManager& Game::GetSfxManager() const
    {
        return *m_sfx_manager;
    }

    Status& Game::GetStatus()
    {
        return *m_status;
    }

    const Status& Game::GetStatus() const
    {
        return *m_status;
    }

    CollisionSystem& Game::GetCollisionSystem()
    {
        return *m_collision_system;
    }

    const CollisionSystem& Game::GetCollisionSystem() const
    {
        return *m_collision_system;
    }

    Map& Game::GetMap()
    {
        return *m_map;
    }

    const Map& Game::GetMap() const
    {
        return *m_map;
    }

    View& Game::GetView()
    {
        return *m_view;
    }

    const View& Game::GetView() const
    {
        return *m_view;
    }

    Snake& Game::GetSnake()
    {
        return *m_snake;
    }

    const Snake& Game::GetSnake() const
    {
        return *m_snake;
    }

    Timer& Game::GetTimer()
    {
        return *m_timer;
    }

    const Timer& Game::GetTimer() const
    {
        return *m_timer;
    }

    bool Game::IsPaused() const
    {
        return m_pause;
    }

    bool Game::Init()
    {
        // Random seed
        Random::Seed(static_cast<unsigned int>(time(NULL)));

        // Load configuration
        m_configuration = new Configuration();
        if (!m_configuration->LoadJSON("cfg.json"))
        {
            return false;
        }

        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) == -1)
        {
            return false;
        }
        atexit(SDL_Quit);
        if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        {
            return false;
        }
        atexit(IMG_Quit);
        if (TTF_Init() == -1)
        {
            return false;
        }
        atexit(TTF_Quit);
        if (Mix_Init(0) != 0)
        {
            return false;
        }
        atexit(Mix_Quit);
        if( Mix_OpenAudio(m_configuration->GetSoundSampleRate(), MIX_DEFAULT_FORMAT, m_configuration->GetSoundChannels(), m_configuration->GetSoundChunkSize()) == -1)
        {
            return false;
        }
        atexit(Mix_CloseAudio);

        // Initialize screen
        m_screen = new Screen(m_configuration->GetScreenWidth(), m_configuration->GetScreenHeight(), m_configuration->GetScreenBitsPerPixel(), m_configuration->IsScreenWindowed());
        if (!m_screen || !m_screen->Init())
        {
            return false;
        }
        SDL_WM_SetCaption("Coatl", NULL);

        // Initialize data
        m_gfx_manager = new GfxManager();
        m_gfx_manager->LoadJSON("gfx.json");
        m_sfx_manager = new SfxManager();
        m_sfx_manager->LoadJSON("sfx.json");

        m_timer = new Timer();

        return true;
    }

    void Game::StartNewGame(bool autoplay)
    {
        if (m_autoplay != autoplay)
        {
            m_autoplay = autoplay;
            if (m_autoplay && !m_menu)
            {
                m_menu = new Menu(*this);
            }
            else if (!m_autoplay && m_menu)
            {
                delete m_menu;
                m_menu = NULL;
            }
        }

        // Clean up previous game
        delete m_controller;
        delete m_snake;
        delete m_view;
        Generator::DestroyMap(m_map);
        delete m_collision_system;
        delete m_status;

        // Create game objects
        m_status = new Status(*this);
        m_collision_system = new CollisionSystem(*this);
        m_map = Generator::CreateMap(*this, 256, 256, 12, 12);
        m_view = new View(*this);
        std::pair<unsigned int, unsigned int> spawn = FindSnakeSpawn(10, true);
        m_snake = new Snake(*this, true, spawn.first, spawn.second, 10, SD_NORTH, 100);
        SnakeKeyboardControls controls = { SDLK_LEFT, SDLK_RIGHT, SDLK_UP, SDLK_DOWN };
        if (m_autoplay)
        {
            m_controller = new SnakeAIController(*this, *m_snake);
        }
        else
        {
            m_controller = new SnakeKeyboardController(*this, *m_snake, controls);
        }
        m_pause = false;

        if (!m_autoplay)
        {
            m_sfx_manager->GetSound("start")->Play();
        }

        m_timer->Reset();
    }

    void Game::Loop()
    {
        // Splash screen
        SDL_Surface* sdl_surface = m_screen->GetSDLSurface();
        SDL_FillRect(sdl_surface, 0, SDL_MapRGB(sdl_surface->format, 101, 192, 49));
        Sprite* sprite = Sprite::CreateFromTexture(*m_gfx_manager, "splash");
        sprite->Render(*m_screen, (m_screen->GetWidth() / 2) - (sprite->GetWidth() / 2), (m_screen->GetHeight() / 2) - (sprite->GetHeight() / 2));
        SDL_Flip(sdl_surface);
        SDL_Event event;
        bool running = true;
        bool exit = false;
        unsigned int interval = 0;
        m_timer->Reset();
        while (running && interval < 3000)
        {
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                        running = false;
                        exit = true;
                        break;
                    case SDL_KEYDOWN:
                        running = false;
                        break;
                    default:
                        break;
                }
            }
            m_timer->Update();
            interval += m_timer->GetInterval();
        }
        Sprite::DestroySprite(sprite);

        if (!exit)
        {
            m_timer->Reset();
            running = true;
            while (running)
            {
                while (SDL_PollEvent(&event))
                {
                    switch (event.type)
                    {
                        case SDL_QUIT:
                            running = false;
                            break;
                        case SDL_KEYDOWN:
                            // Exit game
                            if (event.key.keysym.sym == SDLK_ESCAPE)
                            {
                                if (m_menu)
                                {
                                    running = false;
                                }
                                else
                                {
                                    StartNewGame(true);
                                }
                            }
                            // New game
                            else if (event.key.keysym.sym == SDLK_F2)
                            {
                                StartNewGame(false);
                            }
                            // Pause game
                            else if (event.key.keysym.sym == SDLK_p)
                            {
                                if (!m_menu)
                                {
                                    m_pause = !m_pause;
                                }
                            }
                            // Sound on/off
                            else if (event.key.keysym.sym == SDLK_s)
                            {
                                int volume = Mix_Volume(-1, -1);
                                if (volume == 0)
                                {
                                    Mix_Volume(-1, MIX_MAX_VOLUME);
                                }
                                else
                                {
                                    Mix_Volume(-1, 0);
                                }
                            }
                            // Screenshot
                            else if (event.key.keysym.sym == SDLK_F11)
                            {
                                m_screen->Capture();
                            }
                            break;
                        default:
                            break;
                    }
                }

                // Update
                if (m_snake->IsAlive())
                {
                    m_timer->Update();
                    if (!m_pause)
                    {
                        Uint8* key_state = SDL_GetKeyState(NULL);
                        m_controller->KeyboardInput(key_state);
                        m_snake->Update();
                        m_map->Update();
                        m_controller->Update();
                    }
                }
                else if (m_autoplay)
                {
                    StartNewGame(true);
                }

                // Render scene
                m_view->SetFocus(m_snake->GetX(), m_snake->GetY());
                sdl_surface = m_screen->GetSDLSurface();
                SDL_FillRect(sdl_surface, 0, SDL_MapRGB(sdl_surface->format, 0, 0, 0));
                m_view->Render();
                if (!m_autoplay)
                {
                    m_status->Render();
                }
                if (m_menu)
                {
                    m_menu->Render();
                }
                SDL_Flip(sdl_surface);
            }
        }

        // Credits screen
        sdl_surface = m_screen->GetSDLSurface();
        SDL_FillRect(sdl_surface, 0, SDL_MapRGB(sdl_surface->format, 101, 192, 49));
        sprite = Sprite::CreateFromTexture(*m_gfx_manager, "credits");
        sprite->Render(*m_screen, (m_screen->GetWidth() / 2) - (sprite->GetWidth() / 2), (m_screen->GetHeight() / 2) - (sprite->GetHeight() / 2));
        SDL_Flip(sdl_surface);
        running = true;
        interval = 0;
        m_timer->Reset();
        while (running && interval < 3000)
        {
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_KEYDOWN:
                        running = false;
                        break;
                    default:
                        break;
                }
            }
            m_timer->Update();
            interval += m_timer->GetInterval();
        }
        Sprite::DestroySprite(sprite);
    }

    std::pair<unsigned int, unsigned int> Game::FindSnakeSpawn(unsigned int snake_length, bool player) const
    {
        bool found = false;
        unsigned int x;
        unsigned int y;
        while (!found)
        {
            found = true;
            const Map& map = GetMap();
            x = Random::UInt(snake_length, map.GetWidth() - snake_length - 1);
            y = Random::UInt(snake_length, map.GetHeight() - snake_length - 1);
            // AI snakes spawn out of sight
            if (!player)
            {
                if (m_view->GetRect().Contains(x * m_map->GetTileWidth(), y * m_map->GetTileHeight()))
                {
                    found = false;
                }
            }
            if (found)
            {
                // Check every snake segment
                for (unsigned int yy = y - snake_length; found && yy != y + snake_length; ++yy)
                {
                    // Not on blocked tile
                    if (map.GetTile(x, yy)->IsBlocked())
                    {
                        found = false;
                        break;
                    }
                    if (found && !player)
                    {
                        // Not on the player snake
                        const Snake& snake = GetSnake();
                        for (Snake::SegmentConstIterator itr = snake.GetSegmentsBegin(); itr != snake.GetSegmentsEnd(); ++itr)
                        {
                            if ((*itr)->GetX() == x && (*itr)->GetY() == yy)
                            {
                                found = false;
                                break;
                            }
                        }
                        if (found)
                        {
                            // Not on an AI snake
                            for (Map::SnakeConstIterator itr = map.GetSnakesBegin(); itr != map.GetSnakesEnd(); ++itr)
                            {
                                for (Snake::SegmentConstIterator itr2 = (*itr)->GetSegmentsBegin(); itr2 != (*itr)->GetSegmentsEnd(); ++itr2)
                                {
                                    if ((*itr2)->GetX() == x && (*itr2)->GetY() == yy)
                                    {
                                        found = false;
                                        break;
                                    }
                                }
                                if (!found)
                                {
                                    break;
                                }
                            }
                            if (found)
                            {
                                // Not on an apple
                                for (Map::AppleConstIterator itr = map.GetApplesBegin(); itr != map.GetApplesEnd(); ++itr)
                                {
                                    if ((*itr)->GetX() == x && (*itr)->GetY() == yy)
                                    {
                                        found = false;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return std::make_pair(x, y);
    }

    std::pair<unsigned int, unsigned int> Game::FindAppleSpawn() const
    {
        bool found = false;
        unsigned int x;
        unsigned int y;
        while (!found)
        {
            found = true;
            const Map& map = GetMap();
            x = Random::UInt(0, map.GetWidth() - 1);
            y = Random::UInt(0, map.GetHeight() - 1);
            // Not on a blocked tile
            if (map.GetTile(x, y)->IsBlocked())
            {
                found = false;
            }
            if (found)
            {
                // Make sure the tile can be reached properly
                unsigned int count = 0;
                if (!map.GetTile(x - 1, y)->IsBlocked())
                {
                    ++count;
                }
                if (!map.GetTile(x + 1, y)->IsBlocked())
                {
                    ++count;
                }
                if (!map.GetTile(x, y - 1)->IsBlocked())
                {
                    ++count;
                }
                if (!map.GetTile(x, y + 1)->IsBlocked())
                {
                    ++count;
                }
                if (count <= 1)
                {
                    found = false;
                }
                if (found)
                {
                    // Not on the player snake
                    const Snake& snake = GetSnake();
                    for (Snake::SegmentConstIterator itr = snake.GetSegmentsBegin(); itr != snake.GetSegmentsEnd(); ++itr)
                    {
                        if ((*itr)->GetX() == x && (*itr)->GetY() == y)
                        {
                            found = false;
                            break;
                        }
                    }
                    if (found)
                    {
                        // Not on an AI snake
                        for (Map::SnakeConstIterator itr = map.GetSnakesBegin(); itr != map.GetSnakesEnd(); ++itr)
                        {
                            for (Snake::SegmentConstIterator itr2 = (*itr)->GetSegmentsBegin(); itr2 != (*itr)->GetSegmentsEnd(); ++itr2)
                            {
                                if ((*itr2)->GetX() == x && (*itr2)->GetY() == y)
                                {
                                    found = false;
                                    break;
                                }
                            }
                            if (!found)
                            {
                                break;
                            }
                        }
                        if (found)
                        {
                            // Not on an apple
                            for (Map::AppleConstIterator itr = map.GetApplesBegin(); itr != map.GetApplesEnd(); ++itr)
                            {
                                if ((*itr)->GetX() == x && (*itr)->GetY() == y)
                                {
                                    found = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        return std::make_pair(x, y);
    }

    GameAccess::GameAccess(Game& game)
        :   m_game(game)
    {

    }

    GameAccess::~GameAccess()
    {

    }

    Game& GameAccess::GetGame()
    {
        return m_game;
    }

    const Game& GameAccess::GetGame() const
    {
        return m_game;
    }
}
