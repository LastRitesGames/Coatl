#ifndef COATL_SNAKE_HPP
#define COATL_SNAKE_HPP

#include "coatl_collision.hpp"
#include "coatl_game.hpp"

namespace Coatl
{
    class Sprite;

    typedef enum
    {
        SD_EAST = 1,
        SD_WEST = 2,
        SD_NORTH = 4,
        SD_SOUTH = 8,
    } SnakeDirection;

    typedef enum
    {
        SST_HEAD = 16,
        SST_BODY = 32,
        SST_TAIL = 64
    } SnakeSegmentType;

    class SnakeSegment
        :   public GameAccess,
            public CollisionObject
    {
        public:
            SnakeSegment(Game& game, bool player, unsigned int x, unsigned int y);
            virtual ~SnakeSegment();

            virtual void Collide(Snake& snake);
    };

    class Snake
        :   public GameAccess
    {
        public:
            typedef std::vector<SnakeSegment*> SegmentList;
            typedef SegmentList::iterator SegmentIterator;
            typedef SegmentList::const_iterator SegmentConstIterator;

            Snake(Game& game, bool player, unsigned int x, unsigned int y, unsigned int size, SnakeDirection direction, unsigned int speed);
            virtual ~Snake();

            bool IsPlayer() const;
            SnakeDirection GetDirection() const;
            size_t GetLength() const;
            SegmentIterator GetSegmentsBegin();
            SegmentIterator GetSegmentsEnd();
            SegmentConstIterator GetSegmentsBegin() const;
            SegmentConstIterator GetSegmentsEnd() const;
            unsigned int GetX() const;
            unsigned int GetY() const;
            bool WasUpdated() const;
            bool IsAlive() const;

            void SetDirection(SnakeDirection direction);
            void OverrideDirection(SnakeDirection direction);

            unsigned int GetSpeed() const;
            void SetSpeed(unsigned int speed);

            void Grow();
            void Kill();

            void Render();
            void Update();

        private:
            bool m_player;
            SegmentList m_segments;
            SnakeDirection m_direction;
            unsigned int m_speed;
            Uint32 m_interval;
            bool m_moved;
            bool m_updated;
            bool m_alive;
            bool m_grow;
            std::map<unsigned int, Sprite*> m_sprites;
    };

    class SnakeController
        :   public GameAccess
    {
        public:
            SnakeController(Game& game, Snake& snake);
            virtual ~SnakeController();

            Snake& GetSnake();
            const Snake& GetSnake() const;

            virtual void KeyboardInput(Uint8* key_state);
            virtual void Update();

        private:
            Snake& m_snake;
    };

    struct SnakeKeyboardControls
    {
        int Left;
        int Right;
        int Up;
        int Down;
    };

    class SnakeKeyboardController
        :   public SnakeController
    {
        public:
            SnakeKeyboardController(Game& game, Snake& snake, const SnakeKeyboardControls& controls);
            virtual ~SnakeKeyboardController();

            SnakeKeyboardControls& GetControls();
            const SnakeKeyboardControls& GetControls() const;

            virtual void KeyboardInput(Uint8* key_state);

        private:
            SnakeKeyboardControls m_controls;
    };

    class SnakeAIController
        :   public SnakeController
    {
        public:
            SnakeAIController(Game& game, Snake& snake);
            virtual ~SnakeAIController();

            bool WillCollideSoon() const;

            virtual void Update();

        private:
            float m_chance_to_turn;
    };
}

#endif // COATL_SNAKE_HPP
