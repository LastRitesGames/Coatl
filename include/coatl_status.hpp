#ifndef COATL_STATUS_HPP
#define COATL_STATUS_HPP

#include "coatl_game.hpp"

namespace Coatl
{
    class Sprite;

    class Status
        :   public GameAccess
    {
        public:
            Status(Game& game);
            ~Status();

            unsigned int GetLevel() const;
            unsigned int GetScore() const;
            unsigned int GetMaxAppleCount() const;
            unsigned int GetMaxSnakeAICount() const;
            std::pair<unsigned int, unsigned int> GetSnakeAILength() const;

            void AppleEaten();
            void LevelUp();

            void UpdateSprite();

            void Render();

        private:
            unsigned int m_level;
            unsigned int m_score;
            Sprite* m_sprite;
            Sprite* m_gameover;
            Sprite* m_pause;
    };
}

#endif // COATL_STATUS_HPP
