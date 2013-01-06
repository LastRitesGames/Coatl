#ifndef COATL_COLLISION_HPP
#define COATL_COLLISION_HPP

#include "coatl_game.hpp"

namespace Coatl
{
    typedef enum
    {
        CT_OBSTACLE,
        CT_SNAKE_PLAYER,
        CT_SNAKE_AI,
        CT_APPLE,

        CT_NUM
    } CollisionType;

    class CollisionObject
    {
        public:
            CollisionObject(CollisionSystem& system, unsigned int x, unsigned int y, CollisionType type);
            ~CollisionObject();

            unsigned int GetX() const;
            unsigned int GetY() const;
            CollisionType GetType() const;

            void SetCollisionType(CollisionType type);

            virtual void Collide(Snake& snake) = 0;

        private:
            CollisionSystem& m_system;
            unsigned int m_x;
            unsigned int m_y;
            CollisionType m_type;
    };

    class CollisionSystem
        :   public GameAccess
    {
        public:
            typedef std::map<std::pair<unsigned int, unsigned int>, CollisionObject*> ObjectList;
            typedef ObjectList::iterator ObjectIterator;
            typedef ObjectList::const_iterator ObjectConstIterator;

            CollisionSystem(Game& game);
            ~CollisionSystem();

            size_t GetObjectCount() const;
            ObjectIterator GetObjectsBegin();
            ObjectIterator GetObjectsEnd();
            ObjectConstIterator GetObjectsBegin() const;
            ObjectConstIterator GetObjectsEnd() const;
            CollisionObject* GetObject(unsigned int x, unsigned int y);
            const CollisionObject* GetObject(unsigned int x, unsigned int y) const;

            void AddObject(CollisionObject* object);
            void RemoveObject(unsigned int x, unsigned int y);

        private:
            ObjectList m_objects;
    };
}

#endif // COATL_COLLISION_HPP
