#ifndef COATL_SFX_HPP
#define COATL_SFX_HPP

namespace Coatl
{
    class SfxManager;

    class Sound
    {
        public:
            Sound(const std::string& name, const std::string& file_name);
            ~Sound();

            const std::string& GetName() const;
            const std::string& GetFileName() const;
            Mix_Chunk* GetChunk();
            const Mix_Chunk* GetChunk() const;

            void Play();

        private:
            std::string m_name;
            std::string m_file_name;
            Mix_Chunk* m_chunk;
    };

    class SfxManager
    {
        public:
            typedef std::vector<Sound*> SoundList;
            typedef SoundList::iterator SoundIterator;
            typedef SoundList::const_iterator SoundConstIterator;

            SfxManager();
            ~SfxManager();

            size_t GetSoundCount() const;
            SoundIterator GetSoundsBegin();
            SoundIterator GetSoundsEnd();
            SoundConstIterator GetSoundsBegin() const;
            SoundConstIterator GetSoundsEnd() const;
            Sound* GetSound(const std::string& name);
            const Sound* GetSound(const std::string& name) const;

            bool LoadJSON(const std::string& file_name);
            Sound* CreateSound(const std::string& name, const std::string& file_name);

        private:
            SoundList m_sounds;
    };
}

#endif // COATL_SFX_HPP
