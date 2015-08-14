#include <halfling/Console.h>

using namespace halfling;

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

    Layer sub2(Layer::FullWidth, 2, 6, 3, 3);
    scr.add_sublayer(&sub2);
    sub2.put_str(0, 0, "###");
    sub2.put_str(0, 1, "###");
    sub2.put_str(0, 2, "###");

    Layer sub21(Layer::HalfWidth, 0, 1, 3*2, 1);
    sub2.add_sublayer(&sub21);
    sub21.put_str(0, 0, "foobar");

    con.refresh();

    for(int i = 0; i < 10; i++) {
        Rect bounds = sub2.get_bounds();
        sub2.set_origin(bounds.x + 1, bounds.y);
        con.refresh();
        SDL_Delay(250);
    }

    return 0;
} // main()
