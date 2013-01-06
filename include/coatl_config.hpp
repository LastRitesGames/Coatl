#ifndef COATL_CONFIG_HPP
#define COATL_CONFIG_HPP

namespace Coatl
{
    class Configuration
    {
        public:
            Configuration();
            ~Configuration();

            unsigned int GetScreenWidth() const;
            unsigned int GetScreenHeight() const;
            unsigned int GetScreenBitsPerPixel() const;
            bool IsScreenWindowed() const;
            unsigned int GetSoundSampleRate() const;
            unsigned int GetSoundChannels() const;
            unsigned int GetSoundChunkSize() const;

            bool LoadJSON(const std::string& file_name);

        private:
            unsigned int m_screen_width;
            unsigned int m_screen_height;
            unsigned int m_screen_bits_per_pixel;
            bool m_screen_windowed;
            unsigned int m_sound_sample_rate;
            unsigned int m_sound_channels;
            unsigned int m_sound_chunk_size;
    };
}

#endif // COATL_CONFIG_HPP
