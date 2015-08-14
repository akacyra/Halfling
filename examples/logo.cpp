#include <halfling/Console.h>

using namespace halfling;

int main()
{
    Console con("", 0, 0, 4*2, 5);
    Layer h(Layer::HalfWidth, 1*2, 1, 4, 3);
    Layer img(Layer::FullWidth, 0, 0, 4*2, 5);
    con.add_layer(&img);
    con.add_layer(&h);
    h.put_str(0, 0, "\xd2\x20\x20\xd2", COLOR_RED, COLOR_BLACK);
    h.put_str(0, 1, "\xc7\xc4\xc4\xb6", COLOR_RED, COLOR_BLACK);
    h.put_str(0, 2, "\xd0\x20\x20\xd0", COLOR_RED, COLOR_BLACK);
    img.put_str(0, 0, "\xda\xc4\xc4\xbf");
    img.put_str(0, 1, "\xb3\x20\x20\xb3");
    img.put_str(0, 2, "\xb3\x20\x20\xb3");
    img.put_str(0, 3, "\xb3\x20\x20\xb3");
    img.put_str(0, 4, "\xd4\xcd\xcd\xbe");
    /*
    for(int i = 0; i < 5; i++)
        img.put_str(0, i, "....", COLOR_WHITE, COLOR_BLACK);
        */
    /*
    Layer text_full(Layer::FullWidth, 0, 0, 8, 1);
    Layer text_half(Layer::HalfWidth, 0, 1, 2*8, 1);
    Layer symbol_full(Layer::FullWidth, 0, 2, 4, 4);
    Layer symbol_half(Layer::HalfWidth, 2*4, 2, 4, 4);
    con.add_layer(&text_full);
    con.add_layer(&text_half);
    con.add_layer(&symbol_full);
    con.add_layer(&symbol_half);
    text_full.put_str(0, 0, "H lfling", COLOR_RED, COLOR_BLACK);
    text_full.put_char(1, 0, '@');
    text_half.put_str(0, 0, "H lfling", COLOR_RED, COLOR_BLACK);
    text_half.put_char(1, 0, '@');
    symbol_full.put_str(0, 0, "\xc9\xcd\xcd\xbb");
    symbol_full.put_str(0, 1, "\xba\xc9\xbb\xba");
    symbol_full.put_str(0, 2, "\xba\xc8\xca\xbc");
    symbol_full.put_str(0, 3, "\xc8\xcd\xcd\xb5");
    symbol_half.put_str(0, 0, "\xc9\xcd\xcd\xbb");
    symbol_half.put_str(0, 1, "\xba\xc9\xbb\xba");
    symbol_half.put_str(0, 2, "\xba\xc8\xca\xbc");
    symbol_half.put_str(0, 3, "\xc8\xcd\xcd\xb5");
    */
    con.refresh();
    SDL_Delay(3000);
    return 0;
} // main()
