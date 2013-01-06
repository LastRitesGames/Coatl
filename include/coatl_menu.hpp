#ifndef COATL_MENU_HPP
#define COATL_MENU_HPP

#include "coatl_game.hpp"

namespace Coatl
{
    class Sprite;

    class Menu
        :   public GameAccess
    {
        public:
            Menu(Game& game);
            ~Menu();

            void Render();

        private:
            Sprite* m_sprite;
    };
}

#endif // COATL_MENU_HPP
