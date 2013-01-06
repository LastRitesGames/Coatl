#ifndef COATL_GFX_HPP
#define COATL_GFX_HPP

namespace Coatl
{
    class Image
    {
        public:
            Image(const std::string& name, const std::string& file_name);
            ~Image();

            const std::string& GetName() const;
            const std::string& GetFileName() const;
            SDL_Surface* GetSDLSurface();
            const SDL_Surface* GetSDLSurface() const;

        private:
            std::string m_name;
            std::string m_file_name;
            SDL_Surface* m_sdl_surface;
    };

    class Texture
    {
        public:
            Texture(const std::string& name, Image& image, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
            ~Texture();

            const std::string& GetName() const;
            Image& GetImage();
            const Image& GetImage() const;
            unsigned int GetX() const;
            unsigned int GetY() const;
            unsigned int GetWidth() const;
            unsigned int GetHeight() const;
            SDL_Surface* GetSDLSurface();
            const SDL_Surface* GetSDLSurface() const;
            SDL_Rect GetSDLRect() const;

        private:
            std::string m_name;
            Image& m_image;
            unsigned int m_x;
            unsigned int m_y;
            unsigned int m_width;
            unsigned int m_height;
            SDL_Rect m_sdl_rect;
    };

    class TextureGroup
    {
        public:
            typedef std::vector<Texture*> TextureList;
            typedef TextureList::iterator TextureIterator;
            typedef TextureList::const_iterator TextureConstIterator;

            TextureGroup(const std::string& name);
            ~TextureGroup();

            const std::string& GetName() const;
            size_t GetTextureCount() const;
            TextureIterator GetTexturesBegin();
            TextureIterator GetTexturesEnd();
            TextureConstIterator GetTexturesBegin() const;
            TextureConstIterator GetTexturesEnd() const;
            Texture* GetTexture(const std::string& name);
            const Texture* GetTexture(const std::string& name) const;
            Texture* GetRandomTexture();
            const Texture* GetRandomTexture() const;

            void AddTexture(Texture& texture);

        private:
            std::string m_name;
            TextureList m_textures;
    };

    class Font
    {
        public:
            Font(const std::string& name, const std::string& file_name, int size);
            ~Font();

            const std::string& GetName() const;
            const std::string& GetFileName() const;
            int GetSize() const;

            SDL_Surface* RenderText(const std::string& text, const SDL_Color& color) const;

        private:
            std::string m_name;
            std::string m_file_name;
            int m_size;
            TTF_Font* m_ttf_font;
    };

    class GfxManager
    {
        public:
            typedef std::vector<Image*> ImageList;
            typedef ImageList::iterator ImageIterator;
            typedef ImageList::const_iterator ImageConstIterator;
            typedef std::vector<Texture*> TextureList;
            typedef TextureList::iterator TextureIterator;
            typedef TextureList::const_iterator TextureConstIterator;
            typedef std::vector<TextureGroup*> TextureGroupList;
            typedef TextureGroupList::iterator TextureGroupIterator;
            typedef TextureGroupList::const_iterator TextureGroupConstIterator;
            typedef std::vector<Font*> FontList;
            typedef FontList::iterator FontIterator;
            typedef FontList::const_iterator FontConstIterator;

            GfxManager();
            ~GfxManager();

            size_t GetImageCount() const;
            ImageIterator GetImagesBegin();
            ImageIterator GetImagesEnd();
            ImageConstIterator GetImagesBegin() const;
            ImageConstIterator GetImagesEnd() const;
            Image* GetImage(const std::string& name);
            const Image* GetImage(const std::string& name) const;
            size_t GetTextureCount() const;
            TextureIterator GetTexturesBegin();
            TextureIterator GetTexturesEnd();
            TextureConstIterator GetTexturesBegin() const;
            TextureConstIterator GetTexturesEnd() const;
            Texture* GetTexture(const std::string& name);
            const Texture* GetTexture(const std::string& name) const;
            size_t GetTextureGroupCount() const;
            TextureGroupIterator GetTextureGroupsBegin();
            TextureGroupIterator GetTextureGroupsEnd();
            TextureGroupConstIterator GetTextureGroupsBegin() const;
            TextureGroupConstIterator GetTextureGroupsEnd() const;
            TextureGroup* GetTextureGroup(const std::string& name);
            const TextureGroup* GetTextureGroup(const std::string& name) const;
            size_t GetFontCount() const;
            FontIterator GetFontsBegin();
            FontIterator GetFontsEnd();
            FontConstIterator GetFontsBegin() const;
            FontConstIterator GetFontsEnd() const;
            Font* GetFont(const std::string& name);
            const Font* GetFont(const std::string& name) const;

            bool LoadJSON(const std::string& file_name);
            Image* CreateImage(const std::string& name, const std::string& file_name);
            Texture* CreateTexture(const std::string& name, const std::string& image_name, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
            TextureGroup* CreateTextureGroup(const std::string& name);
            Font* CreateFont(const std::string& name, const std::string& file_name, int size);

        private:
            ImageList m_images;
            TextureList m_textures;
            TextureGroupList m_texture_groups;
            FontList m_fonts;
    };
}

#endif // COATL_GFX_HPP
