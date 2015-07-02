#include "Console.h"
#include <algorithm>

using std::vector;
using std::string;

Console::Console(string title, int x, int y, int w, int h) 
    : size{ w, h }, font_size_full{ 12, 12 }
{
    try {
        sdl = SDL_Init(SDL_INIT_VIDEO);
        win = SDL_CreateWindow(title.c_str(), x, y, size.w * font_size_full.w / 2, size.h * font_size_full.h, SDL_WINDOW_SHOWN);
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
        load_font("terminal_12x12.png", "terminal_6x12.png", font_size_full.w, font_size_full.h);

        init_color_map(colors);
    } catch(InitError& err) {
        sdl.log_error(std::cerr, "SDL Initialization");
        throw;
    }
} // Console()

Console::~Console()
{
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_DestroyTexture(font_full);
    SDL_DestroyTexture(font_half);
} // ~Console()

void Console::add_layer(Layer* layer)
{
    if(layer) {
        layers.push_back(layer);
        layer->parent = this;
    }
} // add_layer()

void Console::remove_layer(Layer* layer)
{
    if(layer) {
        auto it = find(layers.begin(), layers.end(), layer);
        if(it != layers.end())
            layers.erase(it);
        layer->parent = nullptr;
    }
} // remove_layer()

void Console::bring_layer_to_front(Layer* layer)
{
    remove_layer(layer);
    add_layer(layer);
} // bring_layer_to_front()

void Console::send_layer_to_back(Layer* layer)
{
    if(layer) {
        auto it = find(layers.begin(), layers.end(), layer);
        if(it != layers.end()) {
            layers.erase(it);
            layers.insert(layers.begin(), layer);
        }
    }
} // send_layer_to_back()

void Console::remove_all_layers()
{
    for(auto it = layers.begin(); it != layers.end(); it++) 
        (*it)->parent = nullptr;
    layers.clear();
} // remove_all_layers()

Size Console::get_size() const
{
    return size;
} // get_size()

void Console::clear()
{
    for(auto it = layers.begin(); it != layers.end(); it++) {
        (*it)->clear();
    }
} // clear()

void Console::refresh()
{
    SDL_Rect src, dst;
    src.h = dst.h = font_size_full.h;

    for(auto it = layers.begin(); it != layers.end(); it++) {
        Layer* layer = *it;
        Rect bounds = layer->bounds;
        Layer::Width width = layer->cell_width();
        SDL_Texture* font;
        if(width == Layer::HalfWidth) {
            src.w = dst.w = font_size_full.w / 2;
            font = font_half;
        } else {
            src.w = dst.w = font_size_full.w;
            font = font_full;
        }
        for(int i = 0; i < bounds.h; i++) {
            for(int j = 0; j < bounds.w; j++) {
                Layer::Cell cell = layer->backbuffer[i][j];
                int font_y = (int)cell.ch / FONTMAP_NUM_Y;
                int font_x = (int)cell.ch % FONTMAP_NUM_X;
                src.x = src.w * font_x;
                src.y = src.h * font_y;
                dst.x = bounds.x * font_size_full.w / 2 + src.w * j;
                dst.y = bounds.y * font_size_full.h + src.h * i;
                RGB fg = get_color_rgb(cell.fg);
                RGB bg = get_color_rgb(cell.bg);
                SDL_SetRenderDrawColor(ren, bg.r, bg.g, bg.b, 255);
                SDL_RenderFillRect(ren, &dst);
                SDL_SetTextureColorMod(font, fg.r, fg.g, fg.b);
                SDL_RenderCopy(ren, font, &src, &dst);
            }
        }
        layer->frontbuffer = layer->backbuffer;
    }

    SDL_RenderPresent(ren);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

} // refresh()

void Console::load_font(std::string fname_full_width, 
                         std::string fname_half_width, 
                         int size_x, int size_y)
{
    SDL_Surface* surf = IMG_Load(fname_full_width.c_str());
    if(surf == nullptr) {
        sdl.log_error(std::cerr, "Font (Full-Width) Loading");
    }
    Uint32 key = SDL_MapRGB(surf->format, 0, 0, 0);
    SDL_SetColorKey(surf, SDL_TRUE, key);
    font_full = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
    if(font_full == nullptr) {
        sdl.log_error(std::cerr, "Font (Full-Width) Loading");
    }
    surf = IMG_Load(fname_half_width.c_str());
    if(surf == nullptr) {
        sdl.log_error(std::cerr, "Font (Half-Width) Loading");
    }
    key = SDL_MapRGB(surf->format, 0, 0, 0);
    SDL_SetColorKey(surf, SDL_TRUE, key);
    font_half = SDL_CreateTextureFromSurface(ren, surf);
    SDL_FreeSurface(surf);
    if(font_full == nullptr) {
        sdl.log_error(std::cerr, "Font (Half-Width) Loading");
    }

    font_size_full.w = size_x;
    font_size_full.w = size_y;
} // load_font()

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

RGB Console::get_color_rgb(Color color) const
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
