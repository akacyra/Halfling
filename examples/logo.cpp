#include <halfling/Console.h>

int main(void)
{
    Console con("Halfling", 0, 0, 14, 6);
    Layer rogue(Layer::FullWidth, 0, 0, 7, 5);
    Layer text(Layer::HalfWidth, 0, 5, 14, 1);
    con.add_layer(&rogue);
    con.add_layer(&text);
    rogue.put_str(0, 0, "##o.o##");
    rogue.put_str(0, 1, "#.....#");
    rogue.put_str(0, 2, "#o...o#");
    rogue.put_str(0, 3, "#..@..#");
    rogue.put_str(0, 4, "##o.o##");
    rogue.put_char(3, 1, 'D', COLOR_RED, COLOR_BLACK);
    rogue.put_char(3, 0, '+', COLOR_YELLOW, COLOR_BLACK);
    rogue.put_char(3, 4, '+', COLOR_YELLOW, COLOR_BLACK);
    text.put_str(0, 0, "--[Halfling]--");
    con.refresh();
    while(true);
} // main()
