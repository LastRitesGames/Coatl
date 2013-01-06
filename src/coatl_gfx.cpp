#include "coatl_pch.hpp"

#include "coatl_gfx.hpp"

#include "coatl_random.hpp"

namespace Coatl
{
    class ImageFinder
    {
        public:
            ImageFinder(const std::string& name) : m_name(name) {}
            ~ImageFinder() {}
            bool operator()(const Image* image) { return image->GetName() == m_name; }
        private:
            std::string m_name;
    };

    class TextureFinder
    {
        public:
            TextureFinder(const std::string& name) : m_name(name) {}
            ~TextureFinder() {}
            bool operator()(const Texture* texture) { return texture->GetName() == m_name; }
        private:
            std::string m_name;
    };

    class TextureGroupFinder
    {
        public:
            TextureGroupFinder(const std::string& name) : m_name(name) {}
            ~TextureGroupFinder() {}
            bool operator()(const TextureGroup* group) { return group->GetName() == m_name; }
        private:
            std::string m_name;
    };

    class FontFinder
    {
        public:
            FontFinder(const std::string& name) : m_name(name) {}
            ~FontFinder() {}
            bool operator()(const Font* font) { return font->GetName() == m_name; }
        private:
            std::string m_name;
    };

    Image::Image(const std::string& name, const std::string& file_name)
        :   m_name(name),
            m_file_name(file_name),
            m_sdl_surface(NULL)
    {
        m_sdl_surface = IMG_Load(m_file_name.c_str());
    }

    Image::~Image()
    {
        SDL_FreeSurface(m_sdl_surface);
    }

    const std::string& Image::GetName() const
    {
        return m_name;
    }

    const std::string& Image::GetFileName() const
    {
        return m_file_name;
    }

    SDL_Surface* Image::GetSDLSurface()
    {
        return m_sdl_surface;
    }

    const SDL_Surface* Image::GetSDLSurface() const
    {
        return m_sdl_surface;
    }

    Texture::Texture(const std::string& name, Image& image, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
        :   m_name(name),
            m_image(image),
            m_x(x),
            m_y(y),
            m_width(width),
            m_height(height),
            m_sdl_rect()
    {
        m_sdl_rect.x = static_cast<Sint16>(m_x);
        m_sdl_rect.y = static_cast<Sint16>(m_y);
        m_sdl_rect.w = static_cast<Uint16>(m_width);
        m_sdl_rect.h = static_cast<Uint16>(m_height);
    }

    Texture::~Texture()
    {

    }

    const std::string& Texture::GetName() const
    {
        return m_name;
    }

    Image& Texture::GetImage()
    {
        return m_image;
    }

    const Image& Texture::GetImage() const
    {
        return m_image;
    }

    unsigned int Texture::GetX() const
    {
        return m_x;
    }

    unsigned int Texture::GetY() const
    {
        return m_y;
    }

    unsigned int Texture::GetWidth() const
    {
        return m_width;
    }

    unsigned int Texture::GetHeight() const
    {
        return m_height;
    }

    SDL_Surface* Texture::GetSDLSurface()
    {
        return m_image.GetSDLSurface();
    }

    const SDL_Surface* Texture::GetSDLSurface() const
    {
        return m_image.GetSDLSurface();
    }

    SDL_Rect Texture::GetSDLRect() const
    {
        return m_sdl_rect;
    }

    TextureGroup::TextureGroup(const std::string& name)
        :   m_name(name),
            m_textures()
    {

    }

    TextureGroup::~TextureGroup()
    {

    }

    const std::string& TextureGroup::GetName() const
    {
        return m_name;
    }

    size_t TextureGroup::GetTextureCount() const
    {
        return m_textures.size();
    }

    TextureGroup::TextureIterator TextureGroup::GetTexturesBegin()
    {
        return m_textures.begin();
    }

    TextureGroup::TextureIterator TextureGroup::GetTexturesEnd()
    {
        return m_textures.end();
    }

    TextureGroup::TextureConstIterator TextureGroup::GetTexturesBegin() const
    {
        return m_textures.begin();
    }

    TextureGroup::TextureConstIterator TextureGroup::GetTexturesEnd() const
    {
        return m_textures.end();
    }

    Texture* TextureGroup::GetTexture(const std::string& name)
    {
        Texture* texture = NULL;
        TextureIterator itr = std::find_if(m_textures.begin(), m_textures.end(), TextureFinder(name));
        if (itr != m_textures.end())
        {
            texture = *itr;
        }
        return texture;
    }

    const Texture* TextureGroup::GetTexture(const std::string& name) const
    {
        const Texture* texture = NULL;
        TextureConstIterator itr = std::find_if(m_textures.begin(), m_textures.end(), TextureFinder(name));
        if (itr != m_textures.end())
        {
            texture = *itr;
        }
        return texture;
    }

    Texture* TextureGroup::GetRandomTexture()
    {
        Texture* texture = NULL;
        if (!m_textures.empty())
        {
            TextureIterator itr = m_textures.begin();
            std::advance(itr, Random::UInt(m_textures.size()));
            texture = *itr;
        }
        return texture;
    }

    const Texture* TextureGroup::GetRandomTexture() const
    {
        const Texture* texture = NULL;
        if (!m_textures.empty())
        {
            TextureConstIterator itr = m_textures.begin();
            std::advance(itr, Random::UInt(m_textures.size()));
        }
        return texture;
    }

    void TextureGroup::AddTexture(Texture& texture)
    {
        m_textures.push_back(&texture);
    }

    Font::Font(const std::string& name, const std::string& file_name, int size)
        :   m_name(name),
            m_file_name(file_name),
            m_size(size),
            m_ttf_font(NULL)
    {
        m_ttf_font = TTF_OpenFont(file_name.c_str(), size);
    }

    Font::~Font()
    {
        TTF_CloseFont(m_ttf_font);
    }

    const std::string& Font::GetName() const
    {
        return m_name;
    }

    const std::string& Font::GetFileName() const
    {
        return m_file_name;
    }

    int Font::GetSize() const
    {
        return m_size;
    }

    SDL_Surface* Font::RenderText(const std::string& text, const SDL_Color& color) const
    {
        return TTF_RenderText_Blended(m_ttf_font, text.c_str(), color);
    }

    GfxManager::GfxManager()
        :   m_images(),
            m_textures(),
            m_texture_groups(),
            m_fonts()
    {

    }

    GfxManager::~GfxManager()
    {
        for (TextureGroupIterator itr = m_texture_groups.begin(); itr != m_texture_groups.end(); ++itr)
        {
            delete *itr;
        }
        for (TextureIterator itr = m_textures.begin(); itr != m_textures.end(); ++itr)
        {
            delete *itr;
        }
        for (ImageIterator itr = m_images.begin(); itr != m_images.end(); ++itr)
        {
            delete *itr;
        }
        for (FontIterator itr = m_fonts.begin(); itr != m_fonts.end(); ++itr)
        {
            delete *itr;
        }
    }

    size_t GfxManager::GetImageCount() const
    {
        return m_images.size();
    }

    GfxManager::ImageIterator GfxManager::GetImagesBegin()
    {
        return m_images.begin();
    }

    GfxManager::ImageIterator GfxManager::GetImagesEnd()
    {
        return m_images.end();
    }

    GfxManager::ImageConstIterator GfxManager::GetImagesBegin() const
    {
        return m_images.begin();
    }

    GfxManager::ImageConstIterator GfxManager::GetImagesEnd() const
    {
        return m_images.end();
    }

    Image* GfxManager::GetImage(const std::string& name)
    {
        Image* image = NULL;
        ImageIterator itr = std::find_if(m_images.begin(), m_images.end(), ImageFinder(name));
        if (itr != m_images.end())
        {
            image = *itr;
        }
        return image;
    }

    const Image* GfxManager::GetImage(const std::string& name) const
    {
        const Image* image = NULL;
        ImageConstIterator itr = std::find_if(m_images.begin(), m_images.end(), ImageFinder(name));
        if (itr != m_images.end())
        {
            image = *itr;
        }
        return image;
    }

    size_t GfxManager::GetTextureCount() const
    {
        return m_textures.size();
    }

    GfxManager::TextureIterator GfxManager::GetTexturesBegin()
    {
        return m_textures.begin();
    }

    GfxManager::TextureIterator GfxManager::GetTexturesEnd()
    {
        return m_textures.end();
    }

    GfxManager::TextureConstIterator GfxManager::GetTexturesBegin() const
    {
        return m_textures.begin();
    }

    GfxManager::TextureConstIterator GfxManager::GetTexturesEnd() const
    {
        return m_textures.end();
    }

    Texture* GfxManager::GetTexture(const std::string& name)
    {
        Texture* texture = NULL;
        TextureIterator itr = std::find_if(m_textures.begin(), m_textures.end(), TextureFinder(name));
        if (itr != m_textures.end())
        {
            texture = *itr;
        }
        return texture;
    }

    const Texture* GfxManager::GetTexture(const std::string& name) const
    {
        const Texture* texture = NULL;
        TextureConstIterator itr = std::find_if(m_textures.begin(), m_textures.end(), TextureFinder(name));
        if (itr != m_textures.end())
        {
            texture = *itr;
        }
        return texture;
    }

    size_t GfxManager::GetTextureGroupCount() const
    {
        return m_texture_groups.size();
    }

    GfxManager::TextureGroupIterator GfxManager::GetTextureGroupsBegin()
    {
        return m_texture_groups.begin();
    }

    GfxManager::TextureGroupIterator GfxManager::GetTextureGroupsEnd()
    {
        return m_texture_groups.end();
    }

    GfxManager::TextureGroupConstIterator GfxManager::GetTextureGroupsBegin() const
    {
        return m_texture_groups.begin();
    }

    GfxManager::TextureGroupConstIterator GfxManager::GetTextureGroupsEnd() const
    {
        return m_texture_groups.end();
    }

    TextureGroup* GfxManager::GetTextureGroup(const std::string& name)
    {
        TextureGroup* group = NULL;
        TextureGroupIterator itr = std::find_if(m_texture_groups.begin(), m_texture_groups.end(), TextureGroupFinder(name));
        if (itr != m_texture_groups.end())
        {
            group = *itr;
        }
        return group;
    }

    const TextureGroup* GfxManager::GetTextureGroup(const std::string& name) const
    {
        const TextureGroup* group = NULL;
        TextureGroupConstIterator itr = std::find_if(m_texture_groups.begin(), m_texture_groups.end(), TextureGroupFinder(name));
        if (itr != m_texture_groups.end())
        {
            group = *itr;
        }
        return group;
    }

    size_t GfxManager::GetFontCount() const
    {
        return m_fonts.size();
    }

    GfxManager::FontIterator GfxManager::GetFontsBegin()
    {
        return m_fonts.begin();
    }

    GfxManager::FontIterator GfxManager::GetFontsEnd()
    {
        return m_fonts.end();
    }

    GfxManager::FontConstIterator GfxManager::GetFontsBegin() const
    {
        return m_fonts.begin();
    }

    GfxManager::FontConstIterator GfxManager::GetFontsEnd() const
    {
        return m_fonts.end();
    }

    Font* GfxManager::GetFont(const std::string& name)
    {
        Font* font = NULL;
        FontIterator itr = std::find_if(m_fonts.begin(), m_fonts.end(), FontFinder(name));
        if (itr != m_fonts.end())
        {
            font = *itr;
        }
        return font;
    }

    const Font* GfxManager::GetFont(const std::string& name) const
    {
        const Font* font = NULL;
        FontConstIterator itr = std::find_if(m_fonts.begin(), m_fonts.end(), FontFinder(name));
        if (itr != m_fonts.end())
        {
            font = *itr;
        }
        return font;
    }

    bool GfxManager::LoadJSON(const std::string& file_name)
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

        Json::Value images = root["images"];
        for (Json::Value::iterator itr_image = images.begin(); itr_image != images.end(); ++itr_image)
        {
            if (!CreateImage((*itr_image)["name"].asString(), (*itr_image)["file"].asString()))
            {
                return false;
            }
        }

        Json::Value textures = root["textures"];
        for (Json::Value::iterator itr_texture = textures.begin(); itr_texture != textures.end(); ++itr_texture)
        {
            if (!CreateTexture((*itr_texture)["name"].asString(),
                          (*itr_texture)["image"].asString(),
                          (*itr_texture)["x"].asUInt(),
                          (*itr_texture)["y"].asUInt(),
                          (*itr_texture)["width"].asUInt(),
                          (*itr_texture)["height"].asUInt()))
            {
                return false;
            }
        }

        Json::Value groups = root["groups"];
        for (Json::Value::iterator itr_group = groups.begin(); itr_group != groups.end(); ++itr_group)
        {
            TextureGroup* group = CreateTextureGroup((*itr_group)["name"].asString());
            if (!group)
            {
                return false;
            }
            for (Json::Value::iterator itr_texture = (*itr_group)["textures"].begin(); itr_texture != (*itr_group)["textures"].end(); ++itr_texture)
            {
                Texture* texture = GetTexture((*itr_texture).asString());
                if (texture)
                {
                    group->AddTexture(*texture);
                }
            }
        }

        Json::Value fonts = root["fonts"];
        for (Json::Value::iterator itr_font = fonts.begin(); itr_font != fonts.end(); ++itr_font)
        {
            if (!CreateFont((*itr_font)["name"].asString(),
                       (*itr_font)["file"].asString(),
                       (*itr_font)["size"].asInt()))
            {
                return false;
            }
        }

        return true;
    }

    Image* GfxManager::CreateImage(const std::string& name, const std::string& file_name)
    {
        Image* image = GetImage(name);
        if (!image)
        {
            image = new Image(name, file_name);
            m_images.push_back(image);
        }
        return image;
    }

    Texture* GfxManager::CreateTexture(const std::string& name, const std::string& image_name, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    {
        Texture* texture = GetTexture(name);
        if (!texture)
        {
            Image* image = GetImage(image_name);
            if (image)
            {
                texture = new Texture(name, *image, x, y, width, height);
                m_textures.push_back(texture);
            }
        }
        return texture;
    }

    TextureGroup* GfxManager::CreateTextureGroup(const std::string& name)
    {
        TextureGroup* group = GetTextureGroup(name);
        if (!group)
        {
            group = new TextureGroup(name);
            m_texture_groups.push_back(group);
        }
        return group;
    }

    Font* GfxManager::CreateFont(const std::string& name, const std::string& file_name, int size)
    {
        Font* font = GetFont(name);
        if (!font)
        {
            font = new Font(name, file_name, size);
            m_fonts.push_back(font);
        }
        return font;
    }
}
