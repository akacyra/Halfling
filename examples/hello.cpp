#include <halfling/Console.h>

using namespace halfling;

int main()
{
    Console con("Hello", 0, 0, 80, 24);
    Layer screen(Layer::HalfWidth, 0, 0, 80, 24);
    con.add_layer(&screen);
    screen.put_str(40, 12, "Hello, world!");
    con.refresh();
    SDL_Delay(3000);
    return 0;
} // main()
