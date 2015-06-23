#include "Console.h"

using std::vector;
using std::string;

Console::Console(string title, int width, int height) 
    : width(width), height(height), cursor_x(0), cursor_y(0),
      font_size_x(12), font_size_y(12), cursor_shown(true),
      default_fg(COLOR_WHITE), default_bg(COLOR_BLACK)
{
    try {
        sdl = SDL_Init(SDL_INIT_VIDEO);
        win = SDL_CreateWindow(title.c_str(), 100, 100, width * font_size_x, height * font_size_y, SDL_WINDOW_SHOWN);
        if(win == nullptr) {
            sdl.log_error(std::cerr, "Window Creation");
        }
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if(ren == nullptr) {
            sdl.log_error(std::cerr, "Renderer Creation");
        }
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

        SDL_RenderClear(ren);
        SDL_RenderPresent(ren);
        load_font("terminal_12x12.png", font_size_x, font_size_y);

        init_color_map(colors);
        Cell cell = { .ch = ' ', .fg = COLOR_WHITE, .bg = COLOR_BLACK };
        back_buffer = vector< vector < Cell > >(height, vector< Cell >(width, cell));
        display_buffer = back_buffer;
    } catch(InitError& err) {
        sdl.log_error(std::cerr, "SDL Initialization");
        throw;
    }
} // Console()

Console::~Console()
{
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_DestroyTexture(font);
} // ~Console()

int Console::get_width() const
{
    return width;
} // get_width()

int Console::get_height() const
{
    return height;
} // get_height()

Console& Console::move(int x, int y)
{
    cursor_x = x;
    cursor_y = y;
    return *this;
} // move()

Console& Console::put_char(char c)
{
    return put_char(c, default_fg, default_bg);
} // put_char()

Console& Console::put_char(char c, Color fg, Color bg)
{
    back_buffer[cursor_y][cursor_x].ch = c;
    back_buffer[cursor_y][cursor_x].fg = fg;
    back_buffer[cursor_y][cursor_x].bg = bg;
    if(cursor_x < width - 1) {
        cursor_x++;
    } else {
        cursor_x = 0;
        if(cursor_y < height - 1) 
            cursor_y++;
        else 
            cursor_y = 0;
    }
    return *this;
} // put_char()

Console& Console::put_str(string str)
{
    return put_str(str, default_fg, default_bg);
} // put_str()

Console& Console::put_str(std::string str, Color fg, Color bg)
{
    int len = str.length();
    for(int i = 0; i < len && cursor_x + i < width; i++) {
        put_char(str[i], fg, bg);
    }
    return *this;
} // put_str()

Console& Console::clear()
{
    Cell cleared = { .ch = ' ', .fg = default_fg, .bg = default_bg };
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            back_buffer[i][j] = cleared;
    return *this;
} // clear()

Console& Console::flush()
{
    SDL_Rect src, dst;
    src.w = dst.w = font_size_x;
    src.h = dst.h = font_size_y;

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            Cell cell = back_buffer[i][j];
            int font_y = (int)cell.ch / FONTMAP_NUM_Y;
            int font_x = (int)cell.ch % FONTMAP_NUM_X;
            src.x = font_size_x * font_x;
            src.y = font_size_y * font_y;
            dst.x = font_size_x * j;
            dst.y = font_size_y * i;
            RGB fg = get_color_rgb(cell.fg);
            RGB bg = get_color_rgb(cell.bg);
            SDL_SetRenderDrawColor(ren, bg.r, bg.g, bg.b, 255);
            SDL_RenderFillRect(ren, &dst);
            SDL_SetTextureColorMod(font, fg.r, fg.g, fg.b);
            SDL_RenderCopy(ren, font, &src, &dst);
        }
    }
    if(cursor_shown) {
        Cell cell = back_buffer[cursor_y][cursor_x];
        int font_y = (int)cell.ch / FONTMAP_NUM_Y;
        int font_x = (int)cell.ch % FONTMAP_NUM_X;
        src.x = font_size_x * font_x;
        src.y = font_size_y * font_y;
        dst.x = font_size_x * cursor_x;
        dst.y = font_size_y * cursor_y;
        SDL_SetRenderDrawColor(ren, 127, 127, 127, 255);
        SDL_RenderFillRect(ren, &dst);
        RGB fg = get_color_rgb(cell.fg);
        SDL_SetTextureColorMod(font, fg.r, fg.g, fg.b);
        SDL_RenderCopy(ren, font, &src, &dst);
    }

    SDL_RenderPresent(ren);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    display_buffer = back_buffer;

    return *this;
} // flush()

void Console::load_font(string filename, int size_x, int size_y)
{
    SDL_Surface* surf = IMG_Load(filename.c_str());
    if(surf == nullptr) {
        sdl.log_error(std::cerr, "Font Loading");
    }
    Uint32 key = SDL_MapRGB(surf->format, 0, 0, 0);
    SDL_SetColorKey(surf, SDL_TRUE, key);
    font = SDL_CreateTextureFromSurface(ren, surf);
    if(font == nullptr) {
        sdl.log_error(std::cerr, "Font Loading");
    }
    font_size_x = size_x;
    font_size_y = size_y;
    SDL_SetWindowSize(win, width * font_size_x, height * font_size_y);
} // load_font()

void Console::show_cursor(bool shown)
{
    cursor_shown = shown;
} // show_cursor()

void Console::init_color_map(ColorMap& colormap)
{
    colormap[COLOR_BLACK]   = { 0  , 0  , 0   };
    colormap[COLOR_BLUE]    = { 0  , 0  , 255 };
    colormap[COLOR_GREEN]   = { 0  , 255, 0   };
    colormap[COLOR_CYAN]    = { 0  , 255, 255 };
    colormap[COLOR_RED]     = { 255, 0  , 0   };
    colormap[COLOR_MAGENTA] = { 255, 0  , 255 };
    colormap[COLOR_YELLOW]  = { 255, 255, 0   };
    colormap[COLOR_WHITE]   = { 255, 255, 255 };
} // init_color_map()

void Console::set_foreground(Color id)
{
    default_fg = id;
} // set_foreground()

void Console::set_background(Color id)
{
    default_bg = id;
} // set_background()

Console::RGB Console::get_color_rgb(Color color) const
{
    try {
        return colors.at(color);
    } catch(std::out_of_range& err) {
        return { 0, 0, 0 };
    }
} // get_color_rgb()

void Console::define_color(Color id, int r, int g, int b)
{
    colors[id] = { .r = r, .g = g, .b = b };
} // define_color()
