#include "coatl_pch.hpp"

#include "coatl_collision.hpp"

namespace Coatl
{
    CollisionObject::CollisionObject(CollisionSystem& system, unsigned int x, unsigned int y, CollisionType type)
        :   m_system(system),
            m_x(x),
            m_y(y),
            m_type(type)
    {
        m_system.AddObject(this);
    }

    CollisionObject::~CollisionObject()
    {
        m_system.RemoveObject(m_x, m_y);
    }

    unsigned int CollisionObject::GetX() const
    {
        return m_x;
    }

    unsigned int CollisionObject::GetY() const
    {
        return m_y;
    }

    CollisionType CollisionObject::GetType() const
    {
        return m_type;
    }

    void CollisionObject::SetCollisionType(CollisionType type)
    {
        m_type = type;
    }

    CollisionSystem::CollisionSystem(Game& game)
        :   GameAccess(game),
            m_objects()
    {

    }

    CollisionSystem::~CollisionSystem()
    {

    }

    size_t CollisionSystem::GetObjectCount() const
    {
        return m_objects.size();
    }

    CollisionSystem::ObjectIterator CollisionSystem::GetObjectsBegin()
    {
        return m_objects.begin();
    }

    CollisionSystem::ObjectIterator CollisionSystem::GetObjectsEnd()
    {
        return m_objects.end();
    }

    CollisionSystem::ObjectConstIterator CollisionSystem::GetObjectsBegin() const
    {
        return m_objects.begin();
    }

    CollisionSystem::ObjectConstIterator CollisionSystem::GetObjectsEnd() const
    {
        return m_objects.end();
    }

    CollisionObject* CollisionSystem::GetObject(unsigned int x, unsigned int y)
    {
        CollisionObject* object = NULL;
        ObjectIterator itr = m_objects.find(std::make_pair(x, y));
        if (itr != m_objects.end())
        {
            object = itr->second;
        }
        return object;
    }

    const CollisionObject* CollisionSystem::GetObject(unsigned int x, unsigned int y) const
    {
        const CollisionObject* object = NULL;
        ObjectConstIterator itr = m_objects.find(std::make_pair(x, y));
        if (itr != m_objects.end())
        {
            object = itr->second;
        }
        return object;
    }

    void CollisionSystem::AddObject(CollisionObject* object)
    {
        m_objects.insert(std::make_pair(std::make_pair(object->GetX(), object->GetY()), object));
    }

    void CollisionSystem::RemoveObject(unsigned int x, unsigned int y)
    {
        ObjectIterator itr = m_objects.find(std::make_pair(x, y));
        if (itr != m_objects.end())
        {
            m_objects.erase(itr);
        }
    }
}
