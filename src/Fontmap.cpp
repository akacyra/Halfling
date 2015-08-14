#include "Fontmap.h"
#include <iostream>

using namespace halfling;

Fontmap::Fontmap() : font(nullptr), num_horiz(0), num_vert(0), 
    glyph_size{ 0, 0 }
{
} // Fontmap()

Fontmap::Fontmap(SDL_Renderer* ren, std::string fname, int num_horiz, int num_vert) 
    : font(nullptr), num_horiz(num_horiz), num_vert(num_vert), 
      glyph_size{ 0, 0 } 
{
    if(!load(ren, fname, num_horiz, num_vert))
        std::cout << "Failure loading font." << std::endl;
} // Fontmap()

Fontmap::~Fontmap()
{
    SDL_DestroyTexture(font);
} // ~Fontmap()

bool Fontmap::load(SDL_Renderer* ren, std::string fname, int num_horiz, int num_vert)
{

    this->num_horiz = num_horiz;
    this->num_vert= num_vert;
    if(font != nullptr) {
        SDL_DestroyTexture(font);
    }
    SDL_Surface* surf = IMG_Load(fname.c_str());
    if(surf == nullptr) {
        return false;
    }
    Uint32 key = SDL_MapRGB(surf->format, 0, 0, 0);
    SDL_SetColorKey(surf, SDL_TRUE, key);
    font = SDL_CreateTextureFromSurface(ren, surf);
    if(font == nullptr) {
        return false;
    };
    glyph_size.w = surf->w / num_horiz;
    glyph_size.h = surf->h / num_vert;
    SDL_FreeSurface(surf);
    return true;
} // load()

const Size& Fontmap::get_glyph_size() const
{
    return glyph_size;
} // get_glyph_size()

SDL_Texture* Fontmap::get_font_texture() 
{
    return font;
} // get_font_texture()

bool Fontmap::is_valid() const
{
    if(font != nullptr ) 
        return true;
    else
        return false;
} // is_valid()

Rect Fontmap::operator[](unsigned char ch) const
{
    if(font == nullptr) {
        return { .x = -1, .y = -1, .w = 0, .h = 0 };
    }
    Rect r;
    r.y = ((int)ch / num_vert) * glyph_size.h;
    r.x = ((int)ch % num_horiz) * glyph_size.w;
    r.w = glyph_size.w;
    r.h = glyph_size.h;
    return r;
} // operator[]()
