#include <halfling/Console.h>

void border(Layer& layer) 
{
    Rect bounds = layer.get_bounds();
    for(int i = 1; i < bounds.w - 1; i++) {
        layer.put_char(i, 0, 196);
        layer.put_char(i, bounds.h -1, 196);
    }
    for(int i = 1; i < bounds.h - 1; i++) {
        layer.put_char(0, i, 179);
        layer.put_char(bounds.w -1, i, 179);
    }
    layer.put_char(0, 0, 218);
    layer.put_char(bounds.w - 1, bounds.h - 1, 217);
    layer.put_char(0, bounds.h - 1, 192);
    layer.put_char(bounds.w - 1, 0, 191);
} // border()

int main()
{

    Console con("Sublayers", 0, 0, 2*40, 25);

    Layer scr(Layer::FullWidth, 0, 0, 40, 25);
    con.add_layer(&scr);
    border(scr);

    Layer sub1(Layer::FullWidth, 5, 1, 5, 5);
    scr.add_sublayer(&sub1);
    border(sub1);

    Layer sub11(Layer::FullWidth, 1*2, 0, 3, 3);
    sub1.add_sublayer(&sub11);
    border(sub11);

    scr.touch();
    con.update();

    SDL_Delay(5000);
    return 0;
} // main()
