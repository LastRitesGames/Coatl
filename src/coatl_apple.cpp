#include "coatl_pch.hpp"

#include "coatl_apple.hpp"

#include "coatl_map.hpp"
#include "coatl_random.hpp"
#include "coatl_sfx.hpp"
#include "coatl_snake.hpp"
#include "coatl_sprite.hpp"
#include "coatl_status.hpp"
#include "coatl_timer.hpp"

namespace Coatl
{
    Apple::Apple(Game& game, unsigned int x, unsigned int y)
        :   GameAccess(game),
            CollisionObject(game.GetCollisionSystem(), x, y, CT_APPLE),
            m_rect(),
            m_used(false)
    {
        Map& map = game.GetMap();
        m_rect.Left = x * map.GetTileWidth();
        m_rect.Right = m_rect.Left + map.GetTileWidth();
        m_rect.Top = y * map.GetTileHeight();
        m_rect.Bottom = m_rect.Top + map.GetTileHeight();
        m_sprite = Sprite::CreateFromTexture(game.GetGfxManager(), "apple");
    }

    Apple::~Apple()
    {
        delete m_sprite;
    }

    const ViewRect& Apple::GetRect() const
    {
        return m_rect;
    }

    bool Apple::WasUsed() const
    {
        return m_used;
    }

    void Apple::Render()
    {
        Game& game = GetGame();
        View& view = game.GetView();
        std::pair<int, int> screen_pos = view.GetScreenPosition(m_rect.Left, m_rect.Top);
        m_sprite->Render(game.GetScreen(), screen_pos.first, screen_pos.second);
    }

    void Apple::Collide(Snake& snake)
    {
        snake.Grow();
        m_used = true;
        if (snake.IsPlayer())
        {
            Game& game = GetGame();
            game.GetStatus().AppleEaten();
            game.GetSfxManager().GetSound("apple")->Play();
        }
    }
}
