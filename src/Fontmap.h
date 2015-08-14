#ifndef __HALFLING_FONTMAP_H__
#define __HALFLING_FONTMAP_H__

#include "SDL.h"
#include "Util.h"
#include <string>

namespace halfling
{

    class Fontmap
    {
        public:
            Fontmap();
            Fontmap(SDL_Renderer* ren, std::string fname, int num_horiz, int num_vert);
            ~Fontmap();

            bool load(SDL_Renderer* ren, std::string fname, int num_horiz, int num_vert);

            const Size& get_glyph_size() const;
            SDL_Texture* get_font_texture();
            bool is_valid() const;
            Rect operator[](unsigned char ch) const;

        private:
            SDL_Texture* font;

            int num_horiz, num_vert;
            Size glyph_size;
    };

};

#endif 
