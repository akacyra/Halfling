#ifndef __CONSOLE_H__
#define __CONSOLE_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "SDL.h"

typedef unsigned int Color;

const Color COLOR_BLACK   = 0;
const Color COLOR_BLUE    = 1;
const Color COLOR_GREEN   = 2;
const Color COLOR_CYAN    = 3;
const Color COLOR_RED     = 4;
const Color COLOR_MAGENTA = 5;
const Color COLOR_YELLOW  = 6;
const Color COLOR_WHITE   = 7;

class Console 
{
    public:
        Console(std::string title, int width = 80, int height = 24); 
        ~Console();

        void load_font(std::string filename, int size_x, int size_y);
        void show_cursor(bool shown);

        Console& move(int x, int y);
        Console& put_char(char c);
        Console& put_char(char c, Color fg, Color bg);
        Console& put_str(std::string str);
        Console& put_str(std::string str, Color fg, Color bg);
        Console& clear();
        Console& flush();

        int get_width() const;
        int get_height() const;

        void set_foreground(Color id);
        void set_background(Color id);
        void define_color(Color id, int r, int g, int b);

    private:
        const int FONTMAP_NUM_X = 16;
        const int FONTMAP_NUM_Y = 16;

        struct RGB { 
            int r, g, b; 
        };
        typedef std::unordered_map< Color, RGB > ColorMap;
        ColorMap colors;

        struct Cell {
            char ch;
            Color fg, bg;
        };
        typedef std::vector< std::vector< Cell > > Buffer;
        Buffer display_buffer;
        Buffer back_buffer;

        int width, height;
        int cursor_x, cursor_y;
        int font_size_x, font_size_y;
        bool cursor_shown;

        Color default_fg, default_bg;

        SDL sdl;
        SDL_Window* win;
        SDL_Renderer* ren;
        SDL_Texture* font;

        void init_color_map(ColorMap& colormap);
        RGB get_color_rgb(Color color) const;
};

#endif
