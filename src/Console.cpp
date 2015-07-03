#include "Console.h"
#include <algorithm>

using std::vector;
using std::string;

Console::Console(string title, int x, int y, int w, int h) 
    : size{ w, h }
{
    try {
        sdl = SDL_Init(SDL_INIT_VIDEO);
        win = SDL_CreateWindow(title.c_str(), x, y, 0, 0, SDL_WINDOW_SHOWN);
        if(win == nullptr) {
            sdl.log_error(std::cerr, "Window Creation");
        }
        ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if(ren == nullptr) {
            sdl.log_error(std::cerr, "Renderer Creation");
        }

        full_width.load(ren, "terminal_12x12.png", 16, 16);
        half_width.load(ren, "terminal_6x12.png", 16, 16);

        if(full_width.is_valid() && half_width.is_valid()) {
            int win_w = half_width.get_glyph_size().w * w;
            int win_h = half_width.get_glyph_size().h * h;
            SDL_SetWindowSize(win, win_w, win_h);
        } else {
            std::cout << "Font loading error." << std::endl;
        }

        init_color_map(colors);

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        SDL_RenderPresent(ren);

    } catch(InitError& err) {
        sdl.log_error(std::cerr, "SDL Initialization");
        throw;
    }
} // Console()

Console::~Console()
{
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
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
    int full_width_w = full_width.get_glyph_size().w;
    int full_width_h = full_width.get_glyph_size().h;
    int half_width_w = half_width.get_glyph_size().w;
    int half_width_h = half_width.get_glyph_size().h;
    src.h = dst.h = full_width_h;

    for(auto it = layers.begin(); it != layers.end(); it++) {
        Layer* layer = *it;
        Rect bounds = layer->bounds;
        Layer::Width width = layer->cell_width();
        Fontmap* fontmap;
        if(width == Layer::HalfWidth) {
            src.w = dst.w = half_width_w;
            fontmap = &half_width;
        } else {
            src.w = dst.w = full_width_w;
            fontmap = &full_width;
        }
        SDL_Texture* font_tex = fontmap->get_font_texture();
        for(int i = 0; i < bounds.h; i++) {
            for(int j = 0; j < bounds.w; j++) {
                Layer::Cell cell = layer->backbuffer[i][j];
                Rect r = (*fontmap)[(unsigned char)cell.ch];
                src = { .x = r.x, .y = r.y, .w = r.w, .h = r.h };
                dst.x = bounds.x * half_width_w + src.w * j;
                dst.y = bounds.y * half_width_h + src.h * i;
                RGB fg = get_color_rgb(cell.fg);
                RGB bg = get_color_rgb(cell.bg);
                SDL_SetRenderDrawColor(ren, bg.r, bg.g, bg.b, 255);
                SDL_RenderFillRect(ren, &dst);
                SDL_SetTextureColorMod(font_tex, fg.r, fg.g, fg.b);
                SDL_RenderCopy(ren, font_tex, &src, &dst);
            }
        }
        layer->frontbuffer = layer->backbuffer;
    }

    SDL_RenderPresent(ren);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

} // refresh()

void Console::set_custom_font(string fname, Layer::Width font_width, 
                              int num_horiz, int num_vert)
{
} // set_custom_font()

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
