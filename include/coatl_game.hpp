#ifndef COATL_GAME_HPP
#define COATL_GAME_HPP

namespace Coatl
{
    class CollisionSystem;
    class Configuration;
    class GfxManager;
    class Map;
    class Menu;
    class Screen;
    class SfxManager;
    class Snake;
    class SnakeController;
    class Sprite;
    class Status;
    class Timer;
    class View;

    class Game
    {
        public:
            static void Run();

        private:
            Game();
        public:
            ~Game();

            Configuration& GetConfiguration();
            const Configuration& GetConfiguration() const;
            Screen& GetScreen();
            const Screen& GetScreen() const;
            GfxManager& GetGfxManager();
            const GfxManager& GetGfxManager() const;
            SfxManager& GetSfxManager();
            const SfxManager& GetSfxManager() const;
            CollisionSystem& GetCollisionSystem();
            Status& GetStatus();
            const Status& GetStatus() const;
            const CollisionSystem& GetCollisionSystem() const;
            Map& GetMap();
            const Map& GetMap() const;
            Snake& GetSnake();
            const Snake& GetSnake() const;
            Timer& GetTimer();
            const Timer& GetTimer() const;
            View& GetView();
            const View& GetView() const;
            bool IsPaused() const;

            bool Init();
            void StartNewGame(bool autoplay);

            std::pair<unsigned int, unsigned int> FindSnakeSpawn(unsigned int snake_length, bool player) const;
            std::pair<unsigned int, unsigned int> FindAppleSpawn() const;

        private:
            void Loop();

        private:
            Configuration* m_configuration;
            Screen* m_screen;
            GfxManager* m_gfx_manager;
            SfxManager* m_sfx_manager;
            Status* m_status;
            CollisionSystem* m_collision_system;
            Map* m_map;
            View *m_view;
            Snake* m_snake;
            SnakeController* m_controller;
            Timer* m_timer;
            Menu* m_menu;
            bool m_autoplay;
            bool m_pause;
    };

    class GameAccess
    {
        public:
            GameAccess(Game& game);
            ~GameAccess();

            Game& GetGame();
            const Game& GetGame() const;

        private:
            Game& m_game;
    };
}

#endif // COATL_GAME_HPP
