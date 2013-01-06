#include "coatl_pch.hpp"

#include "coatl_status.hpp"

#include "coatl_snake.hpp"
#include "coatl_screen.hpp"
#include "coatl_sfx.hpp"
#include "coatl_sprite.hpp"

namespace Coatl
{
    Status::Status(Game& game)
        :   GameAccess(game),
            m_level(1),
            m_score(0),
            m_sprite(NULL),
            m_gameover(NULL),
            m_pause(NULL)
    {
        UpdateSprite();
        SDL_Color color = {255, 0, 0, 255};
        m_gameover = Sprite::CreateFromText(game.GetGfxManager(), "gameover", "GAME OVER", color);
        m_pause = Sprite::CreateFromText(game.GetGfxManager(), "gameover", "PAUSED", color);
    }

    Status::~Status()
    {
        Sprite::DestroySprite(m_sprite);
        Sprite::DestroySprite(m_gameover);
    }

    unsigned int Status::GetLevel() const
    {
        return m_level;
    }

    unsigned int Status::GetScore() const
    {
        return m_score;
    }

    unsigned int Status::GetMaxAppleCount() const
    {
        return 8 + (m_level * 2);
    }

    unsigned int Status::GetMaxSnakeAICount() const
    {
        return m_level - 1;
    }

    std::pair<unsigned int, unsigned int> Status::GetSnakeAILength() const
    {
        const Snake& snake = GetGame().GetSnake();
        unsigned int min = snake.GetLength() / 2;
        unsigned int max = snake.GetLength();
        return std::make_pair(min, max);
    }

    void Status::AppleEaten()
    {
        m_score += 10;
        if (m_score % 50 == 0)
        {
            LevelUp();
        }
        UpdateSprite();
    }

    void Status::LevelUp()
    {
        ++m_level;
        Game& game = GetGame();
        Snake& snake = game.GetSnake();
        snake.SetSpeed(static_cast<unsigned int>(snake.GetSpeed() * 0.95f));
        game.GetSfxManager().GetSound("level")->Play();
    }

    void Status::UpdateSprite()
    {
        std::stringstream ss;
        ss << "LEVEL  " << m_level << "        SCORE  " << m_score;
        SDL_Color color = {255, 0, 0, 255};
        m_sprite = Sprite::CreateFromText(GetGame().GetGfxManager(), "score", ss.str(), color);
    }

    void Status::Render()
    {
        Game& game = GetGame();
        Screen& screen = game.GetScreen();
        m_sprite->Render(screen, 4, 0);
        if (!game.GetSnake().IsAlive())
        {
            m_gameover->Render(screen, (screen.GetWidth() / 2) - (m_gameover->GetWidth() / 2), (screen.GetHeight() / 2) - (m_gameover->GetHeight() / 2));
        }
        else if (game.IsPaused())
        {
            m_pause->Render(screen, (screen.GetWidth() / 2) - (m_pause->GetWidth() / 2), (screen.GetHeight() / 2) - (m_pause->GetHeight() / 2));
        }
    }
}
