#include "coatl_pch.hpp"

#include "coatl_config.hpp"

namespace Coatl
{
    Configuration::Configuration()
        :   m_screen_width(800),
            m_screen_height(600),
            m_screen_bits_per_pixel(32),
            m_screen_windowed(true),
            m_sound_sample_rate(44100),
            m_sound_channels(2),
            m_sound_chunk_size(4096)
    {

    }

    Configuration::~Configuration()
    {

    }

    unsigned int Configuration::GetScreenWidth() const
    {
        return m_screen_width;
    }

    unsigned int Configuration::GetScreenHeight() const
    {
        return m_screen_height;
    }

    unsigned int Configuration::GetScreenBitsPerPixel() const
    {
        return m_screen_bits_per_pixel;
    }

    bool Configuration::IsScreenWindowed() const
    {
        return m_screen_windowed;
    }

    unsigned int Configuration::GetSoundSampleRate() const
    {
        return m_sound_sample_rate;
    }

    unsigned int Configuration::GetSoundChannels() const
    {
        return m_sound_channels;
    }

    unsigned int Configuration::GetSoundChunkSize() const
    {
        return m_sound_chunk_size;
    }

    bool Configuration::LoadJSON(const std::string& file_name)
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

        Json::Value screen = root["screen"];
        m_screen_width = screen["width"].asUInt();
        m_screen_height = screen["height"].asUInt();
        m_screen_bits_per_pixel = screen["bits per pixel"].asUInt();
        m_screen_windowed = screen["windowed"].asBool();

        Json::Value sound = root["sound"];
        m_sound_sample_rate = screen["sample rate"].asUInt();
        m_sound_channels = screen["channels"].asUInt();
        m_sound_chunk_size = screen["chunk size"].asUInt();

        return true;
    }
}
