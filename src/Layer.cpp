#include "Layer.h"
#include "Console.h"

using std::vector;
using std::string;
using boost::optional;


Layer::Layer(Width cell_width, int x, int y, int w, int h) 
    : parent(nullptr), bounds{ x, y, w, h }, cell_w(cell_width),
      default_fg(COLOR_WHITE), default_bg(COLOR_BLACK)
{
    Cell def = { .ch = ' ', .fg = COLOR_WHITE, .bg = COLOR_BLACK };
    backbuffer = vector< vector < Cell > >(bounds.h,
            vector< Cell >(bounds.w, def));
    frontbuffer = backbuffer;
} // Layer()

Layer::Width Layer::cell_width() const
{
    return cell_w;
} // cell_width()

void Layer::clear()
{
    Cell cleared = { .ch = ' ', .fg = default_fg, .bg = default_bg };
    for(int i = 0; i < bounds.h; i++)
        for(int j = 0; j < bounds.w; j++)
            backbuffer[i][j] = cleared;
} // clear() 

void Layer::put_char(int x, int y, char ch)
{
    put_char(x, y, ch, default_fg, default_bg);
} // put_char()

void Layer::put_char(int x, int y, char ch, Color fg, Color bg)
{
    if(contains(x, y))
        backbuffer[y][x] = { .ch = ch, .fg = fg, .bg = bg };
} // put_char()

void Layer::put_str(int x, int y, string str)
{
    put_str(x, y, str, default_fg, default_bg);
} // put_str()

void Layer::put_str(int x, int y, string str, Color fg, Color bg)
{
    int len = str.length();
    for(int i = 0; i < len && x + i < bounds.w; i++) {
        put_char(x + i, y, str[i], fg, bg);
    }
} // put_str()

optional< Layer::Cell > Layer::operator()(int x, int y) const
{
    if(contains(x, y)) {
        Cell c = frontbuffer[y][x];
        return optional< Cell >(c);
    }
    return optional< Cell >();
} // operator()()

void Layer::set_fg_color(Color id)
{
    default_fg = id;
} // set_foreground()

void Layer::set_bg_color(Color id)
{
    default_bg = id;
} // set_background()

Color Layer::get_fg_color() const
{
    return default_fg;
} // get_fg_color()

Color Layer::get_bg_color() const
{
    return default_bg;
} // get_bg_color()

void Layer::set_origin(int x, int y)
{
    bounds.x = x;
    bounds.y = y;
} // set_origin()

Rect Layer::get_bounds() const
{
    return bounds;
} // get_bounds()

bool Layer::contains(int x, int y) const
{
    return (x >= 0 && x <= bounds.w && y >= 0 && y <= bounds.h);
} // contains()
