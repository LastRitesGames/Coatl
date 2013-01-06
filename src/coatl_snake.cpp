#include "coatl_pch.hpp"

#include "coatl_snake.hpp"

#include "coatl_apple.hpp"
#include "coatl_gfx.hpp"
#include "coatl_map.hpp"
#include "coatl_random.hpp"
#include "coatl_sfx.hpp"
#include "coatl_sprite.hpp"
#include "coatl_tile.hpp"
#include "coatl_timer.hpp"
#include "coatl_view.hpp"

namespace Coatl
{
    SnakeSegment::SnakeSegment(Game& game, bool player, unsigned int x, unsigned int y)
        :   GameAccess(game),
            CollisionObject(game.GetCollisionSystem(), x, y, player ? CT_SNAKE_PLAYER : CT_SNAKE_AI)
    {

    }

    SnakeSegment::~SnakeSegment()
    {

    }

    void SnakeSegment::Collide(Snake& snake)
    {
        snake.Kill();
    }

    Snake::Snake(Game& game, bool player, unsigned int x, unsigned int y, unsigned int size, SnakeDirection direction, unsigned int speed)
        :   GameAccess(game),
            m_player(player),
            m_segments(),
            m_direction(direction),
            m_speed(speed),
            m_interval(0),
            m_moved(false),
            m_updated(false),
            m_alive(true),
            m_grow(false),
            m_sprites()
    {
        GfxManager& gfx_manager = game.GetGfxManager();
        if (m_player)
        {
            m_sprites.insert(std::make_pair(SD_EAST | SST_HEAD, Sprite::CreateFromTexture(gfx_manager, "snake1_head_east")));
            m_sprites.insert(std::make_pair(SD_WEST | SST_HEAD, Sprite::CreateFromTexture(gfx_manager, "snake1_head_west")));
            m_sprites.insert(std::make_pair(SD_NORTH | SST_HEAD, Sprite::CreateFromTexture(gfx_manager, "snake1_head_north")));
            m_sprites.insert(std::make_pair(SD_SOUTH | SST_HEAD, Sprite::CreateFromTexture(gfx_manager, "snake1_head_south")));
            m_sprites.insert(std::make_pair(SD_EAST | SD_WEST | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake1_body_east_west")));
            m_sprites.insert(std::make_pair(SD_NORTH | SD_SOUTH | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake1_body_north_south")));
            m_sprites.insert(std::make_pair(SD_EAST | SD_NORTH | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake1_body_east_north")));
            m_sprites.insert(std::make_pair(SD_EAST | SD_SOUTH | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake1_body_east_south")));
            m_sprites.insert(std::make_pair(SD_WEST | SD_NORTH | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake1_body_west_north")));
            m_sprites.insert(std::make_pair(SD_WEST | SD_SOUTH | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake1_body_west_south")));
            m_sprites.insert(std::make_pair(SD_EAST | SST_TAIL, Sprite::CreateFromTexture(gfx_manager, "snake1_tail_east")));
            m_sprites.insert(std::make_pair(SD_WEST | SST_TAIL, Sprite::CreateFromTexture(gfx_manager, "snake1_tail_west")));
            m_sprites.insert(std::make_pair(SD_NORTH | SST_TAIL, Sprite::CreateFromTexture(gfx_manager, "snake1_tail_north")));
            m_sprites.insert(std::make_pair(SD_SOUTH | SST_TAIL, Sprite::CreateFromTexture(gfx_manager, "snake1_tail_south")));
        }
        else
        {
            m_sprites.insert(std::make_pair(SD_EAST | SST_HEAD, Sprite::CreateFromTexture(gfx_manager, "snake2_head_east")));
            m_sprites.insert(std::make_pair(SD_WEST | SST_HEAD, Sprite::CreateFromTexture(gfx_manager, "snake2_head_west")));
            m_sprites.insert(std::make_pair(SD_NORTH | SST_HEAD, Sprite::CreateFromTexture(gfx_manager, "snake2_head_north")));
            m_sprites.insert(std::make_pair(SD_SOUTH | SST_HEAD, Sprite::CreateFromTexture(gfx_manager, "snake2_head_south")));
            m_sprites.insert(std::make_pair(SD_EAST | SD_WEST | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake2_body_east_west")));
            m_sprites.insert(std::make_pair(SD_NORTH | SD_SOUTH | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake2_body_north_south")));
            m_sprites.insert(std::make_pair(SD_EAST | SD_NORTH | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake2_body_east_north")));
            m_sprites.insert(std::make_pair(SD_EAST | SD_SOUTH | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake2_body_east_south")));
            m_sprites.insert(std::make_pair(SD_WEST | SD_NORTH | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake2_body_west_north")));
            m_sprites.insert(std::make_pair(SD_WEST | SD_SOUTH | SST_BODY, Sprite::CreateFromTexture(gfx_manager, "snake2_body_west_south")));
            m_sprites.insert(std::make_pair(SD_EAST | SST_TAIL, Sprite::CreateFromTexture(gfx_manager, "snake2_tail_east")));
            m_sprites.insert(std::make_pair(SD_WEST | SST_TAIL, Sprite::CreateFromTexture(gfx_manager, "snake2_tail_west")));
            m_sprites.insert(std::make_pair(SD_NORTH | SST_TAIL, Sprite::CreateFromTexture(gfx_manager, "snake2_tail_north")));
            m_sprites.insert(std::make_pair(SD_SOUTH | SST_TAIL, Sprite::CreateFromTexture(gfx_manager, "snake2_tail_south")));
        }

        int offset_x = 0;
        int offset_y = 0;
        if (direction == SD_EAST)
        {
            offset_x = 1;
        }
        else if (direction == SD_WEST)
        {
            offset_x = -1;
        }
        else if (direction == SD_NORTH)
        {
            offset_y = 1;
        }
        else if (direction == SD_SOUTH)
        {
            offset_y = -1;
        }
        for (unsigned int i = 0; i < size; ++i)
        {
            m_segments.push_back(new SnakeSegment(game, m_player, x, y));
            x += offset_x;
            y += offset_y;
        }
    }

    Snake::~Snake()
    {
        for (std::map<unsigned int, Sprite*>::iterator itr = m_sprites.begin(); itr != m_sprites.end(); ++itr)
        {
            Sprite::DestroySprite(itr->second);
        }
        for (SegmentIterator itr = m_segments.begin(); itr != m_segments.end(); ++itr)
        {
            delete *itr;
        }
    }

    bool Snake::IsPlayer() const
    {
        return m_player;
    }

    SnakeDirection Snake::GetDirection() const
    {
        return m_direction;
    }

    size_t Snake::GetLength() const
    {
        return m_segments.size();
    }

    Snake::SegmentIterator Snake::GetSegmentsBegin()
    {
        return m_segments.begin();
    }

    Snake::SegmentIterator Snake::GetSegmentsEnd()
    {
        return m_segments.end();
    }

    Snake::SegmentConstIterator Snake::GetSegmentsBegin() const
    {
        return m_segments.begin();
    }

    Snake::SegmentConstIterator Snake::GetSegmentsEnd() const
    {
        return m_segments.end();
    }

    unsigned int Snake::GetX() const
    {
        return m_segments.front()->GetX();
    }

    unsigned int Snake::GetY() const
    {
        return m_segments.front()->GetY();
    }

    bool Snake::WasUpdated() const
    {
        return m_updated;
    }

    bool Snake::IsAlive() const
    {
        return m_alive;
    }

    void Snake::SetDirection(SnakeDirection direction)
    {
        if (!m_moved)
        {
            if ((direction == SD_EAST && m_direction != SD_EAST && m_direction != SD_WEST) ||
                (direction == SD_WEST && m_direction != SD_EAST && m_direction != SD_WEST) ||
                (direction == SD_NORTH && m_direction != SD_NORTH && m_direction != SD_SOUTH) ||
                (direction == SD_SOUTH && m_direction != SD_NORTH && m_direction != SD_SOUTH))
            {
                m_direction = direction;
                m_moved = true;
                if (m_player)
                {
                    GetGame().GetSfxManager().GetSound("turn")->Play();
                }
            }
        }
    }

    void Snake::OverrideDirection(SnakeDirection direction)
    {
        m_direction = direction;
    }

    unsigned int Snake::GetSpeed() const
    {
        return m_speed;
    }

    void Snake::SetSpeed(unsigned int speed)
    {
        m_speed = speed;
    }

    void Snake::Render()
    {
        SnakeSegment* prev = NULL;
        SnakeSegment* next = NULL;
        unsigned int mask;
        for (SegmentIterator itr = m_segments.begin(); itr != m_segments.end(); ++itr)
        {
            if (*itr != m_segments.back())
            {
                next = *(itr + 1);
            }

            if (*itr == m_segments.front())
            {
                mask = SST_HEAD;
            }
            else if (*itr == m_segments.back())
            {
                mask = SST_TAIL;
            }
            else
            {
                mask = SST_BODY;
            }

            int diff;
            if (next)
            {
                diff = static_cast<int>(next->GetX()) - static_cast<int>((*itr)->GetX());
                if (diff == -1)
                {
                    mask |= SD_EAST;
                }
                else if (diff == 1)
                {
                    mask |= SD_WEST;
                }
                diff = static_cast<int>(next->GetY()) - static_cast<int>((*itr)->GetY());
                if (diff == -1)
                {
                    mask |= SD_NORTH;
                }
                else if (diff == 1)
                {
                    mask |= SD_SOUTH;
                }
            }
            if (prev)
            {
                diff = static_cast<int>(prev->GetX()) - static_cast<int>((*itr)->GetX());
                if (diff == -1)
                {
                    mask |= SD_EAST;
                }
                else if (diff == 1)
                {
                    mask |= SD_WEST;
                }
                diff = static_cast<int>(prev->GetY()) - static_cast<int>((*itr)->GetY());
                if (diff == -1)
                {
                    mask |= SD_NORTH;
                }
                else if (diff == 1)
                {
                    mask |= SD_SOUTH;
                }
            }

            Game& game = GetGame();
            Map& map = game.GetMap();
            View& view = game.GetView();
            std::pair<int, int> screen_pos = view.GetScreenPosition((*itr)->GetX() * map.GetTileWidth(), (*itr)->GetY() * map.GetTileHeight());
            m_sprites[mask]->Render(game.GetScreen(), screen_pos.first, screen_pos.second);

            prev = *itr;
        }
    }

    void Snake::Grow()
    {
        m_grow = true;
    }

    void Snake::Kill()
    {
        m_alive = false;
        if (m_player)
        {
            GetGame().GetSfxManager().GetSound("gameover")->Play();
        }
        else
        {
            GetGame().GetSfxManager().GetSound("kill")->Play();
        }
    }

    void Snake::Update()
    {
        m_updated = false;
        Game& game = GetGame();
        m_interval += game.GetTimer().GetInterval();
        if (m_interval > m_speed)
        {
            m_interval -= m_speed;
            m_updated = true;
            if (m_alive)
            {
                unsigned int x = GetX();
                unsigned int y = GetY();
                if (m_direction == SD_EAST)
                {
                    --x;
                }
                else if (m_direction == SD_WEST)
                {
                    ++x;
                }
                else if (m_direction == SD_NORTH)
                {
                    --y;
                }
                else if (m_direction == SD_SOUTH)
                {
                    ++y;
                }
                CollisionSystem& collision_system = game.GetCollisionSystem();
                CollisionObject* object = collision_system.GetObject(x, y);
                if (object)
                {
                    object->Collide(*this);
                }

                if (m_alive)
                {
                    if (m_grow)
                    {
                        m_grow = false;
                    }
                    else
                    {
                        delete m_segments.back();
                        m_segments.pop_back();
                    }
                    m_segments.insert(m_segments.begin(), new SnakeSegment(game, m_player, x, y));
                    m_moved = false;
                }
            }
        }
    }

    SnakeController::SnakeController(Game& game, Snake& snake)
        :   GameAccess(game),
            m_snake(snake)
    {

    }

    SnakeController::~SnakeController()
    {

    }

    Snake& SnakeController::GetSnake()
    {
        return m_snake;
    }

    const Snake& SnakeController::GetSnake() const
    {
        return m_snake;
    }

    void SnakeController::KeyboardInput(Uint8* key_state)
    {

    }

    void SnakeController::Update()
    {

    }

    SnakeKeyboardController::SnakeKeyboardController(Game& game, Snake& snake, const SnakeKeyboardControls& controls)
        :   SnakeController(game, snake),
            m_controls(controls)
    {

    }

    SnakeKeyboardController::~SnakeKeyboardController()
    {

    }

    SnakeKeyboardControls& SnakeKeyboardController::GetControls()
    {
        return m_controls;
    }

    const SnakeKeyboardControls& SnakeKeyboardController::GetControls() const
    {
        return m_controls;
    }

    void SnakeKeyboardController::KeyboardInput(Uint8* key_state)
    {
        Snake& snake = GetSnake();
        if (key_state[m_controls.Left])
        {
            snake.SetDirection(SD_EAST);
        }
        if (key_state[m_controls.Right])
        {
            snake.SetDirection(SD_WEST);
        }
        if (key_state[m_controls.Up])
        {
            snake.SetDirection(SD_NORTH);
        }
        if (key_state[m_controls.Down])
        {
            snake.SetDirection(SD_SOUTH);
        }
    }

    SnakeAIController::SnakeAIController(Game& game, Snake& snake)
        :   SnakeController(game, snake),
            m_chance_to_turn(0.01f)
    {

    }

    SnakeAIController::~SnakeAIController()
    {

    }

    bool SnakeAIController::WillCollideSoon() const
    {
        const Snake& snake = GetSnake();
        unsigned int x = snake.GetX();
        unsigned int y = snake.GetY();
        if (snake.GetDirection() == SD_EAST)
        {
            --x;
        }
        else if (snake.GetDirection() == SD_WEST)
        {
            ++x;
        }
        else if (snake.GetDirection() == SD_NORTH)
        {
            --y;
        }
        else if (snake.GetDirection() == SD_SOUTH)
        {
            ++y;
        }
        const CollisionObject* object = GetGame().GetCollisionSystem().GetObject(x, y);
        return object && object->GetType() != CT_APPLE;
    }

    void SnakeAIController::Update()
    {
        Snake& snake = GetSnake();
        if (snake.WasUpdated())
        {
            float roll = Random::Float();
            if (WillCollideSoon() || roll <= m_chance_to_turn)
            {
                roll = Random::Float();
                SnakeDirection direction = snake.GetDirection();
                if (direction == SD_EAST || direction == SD_WEST)
                {
                    if (roll <= 0.5f)
                    {
                        snake.OverrideDirection(SD_NORTH);
                        if (WillCollideSoon())
                        {
                            snake.OverrideDirection(SD_SOUTH);
                            if (WillCollideSoon())
                            {
                                snake.OverrideDirection(direction);
                            }
                        }
                    }
                    else
                    {
                        snake.OverrideDirection(SD_SOUTH);
                        if (WillCollideSoon())
                        {
                            snake.OverrideDirection(SD_NORTH);
                            if (WillCollideSoon())
                            {
                                snake.OverrideDirection(direction);
                            }
                        }
                    }
                }
                else
                {
                    if (roll <= 0.5f)
                    {
                        snake.OverrideDirection(SD_EAST);
                        if (WillCollideSoon())
                        {
                            snake.OverrideDirection(SD_WEST);
                            if (WillCollideSoon())
                            {
                                snake.OverrideDirection(direction);
                            }
                        }
                    }
                    else
                    {
                        snake.OverrideDirection(SD_WEST);
                        if (WillCollideSoon())
                        {
                            snake.OverrideDirection(SD_EAST);
                            if (WillCollideSoon())
                            {
                                snake.OverrideDirection(direction);
                            }
                        }
                    }
                }
                m_chance_to_turn = 0.01f;
            }
            else
            {
                m_chance_to_turn += 0.01f;
            }
        }
    }
}
