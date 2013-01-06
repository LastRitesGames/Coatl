#include "coatl_pch.hpp"

#include "coatl_sfx.hpp"

namespace Coatl
{
    class SoundFinder
    {
        public:
            SoundFinder(const std::string& name) : m_name(name) {}
            ~SoundFinder() {}
            bool operator()(const Sound* Sound) { return Sound->GetName() == m_name; }
        private:
            std::string m_name;
    };

    Sound::Sound(const std::string& name, const std::string& file_name)
        :   m_name(name),
            m_file_name(file_name),
            m_chunk(NULL)
    {
        m_chunk = Mix_LoadWAV(m_file_name.c_str());
    }

    Sound::~Sound()
    {
        Mix_FreeChunk(m_chunk);
    }

    const std::string& Sound::GetName() const
    {
        return m_name;
    }

    const std::string& Sound::GetFileName() const
    {
        return m_file_name;
    }

    Mix_Chunk* Sound::GetChunk()
    {
        return m_chunk;
    }

    const Mix_Chunk* Sound::GetChunk() const
    {
        return m_chunk;
    }

    void Sound::Play()
    {
        Mix_PlayChannel(-1, m_chunk, 0);
    }

    SfxManager::SfxManager()
        :   m_sounds()
    {

    }

    SfxManager::~SfxManager()
    {
        for (SoundIterator itr = m_sounds.begin(); itr != m_sounds.end(); ++itr)
        {
            delete *itr;
        }
    }

    size_t SfxManager::GetSoundCount() const
    {
        return m_sounds.size();
    }

    SfxManager::SoundIterator SfxManager::GetSoundsBegin()
    {
        return m_sounds.begin();
    }

    SfxManager::SoundIterator SfxManager::GetSoundsEnd()
    {
        return m_sounds.end();
    }

    SfxManager::SoundConstIterator SfxManager::GetSoundsBegin() const
    {
        return m_sounds.begin();
    }

    SfxManager::SoundConstIterator SfxManager::GetSoundsEnd() const
    {
        return m_sounds.end();
    }

    Sound* SfxManager::GetSound(const std::string& name)
    {
        Sound* Sound = NULL;
        SoundIterator itr = std::find_if(m_sounds.begin(), m_sounds.end(), SoundFinder(name));
        if (itr != m_sounds.end())
        {
            Sound = *itr;
        }
        return Sound;
    }

    const Sound* SfxManager::GetSound(const std::string& name) const
    {
        const Sound* Sound = NULL;
        SoundConstIterator itr = std::find_if(m_sounds.begin(), m_sounds.end(), SoundFinder(name));
        if (itr != m_sounds.end())
        {
            Sound = *itr;
        }
        return Sound;
    }

    bool SfxManager::LoadJSON(const std::string& file_name)
    {
        std::ifstream file(file_name.c_str());
        if (!file.is_open())
        {
            return false;
        }

        Json::Value root;
        Json::Reader reader;
        if (!reader.parse(file, root, false))
        {
            return false;
        }

        Json::Value Sounds = root["sounds"];
        for (Json::Value::iterator itr_Sound = Sounds.begin(); itr_Sound != Sounds.end(); ++itr_Sound)
        {
            if (!CreateSound((*itr_Sound)["name"].asString(), (*itr_Sound)["file"].asString()))
            {
                return false;
            }
        }

        return true;
    }

    Sound* SfxManager::CreateSound(const std::string& name, const std::string& file_name)
    {
        Sound* sound = GetSound(name);
        if (!sound)
        {
            sound = new Sound(name, file_name);
            m_sounds.push_back(sound);
        }
        return sound;
    }
}
