#include <halfling/Console.h>

int main()
{
    Console con("", 0, 0, 2*8, 6);
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
    con.refresh();
    SDL_Delay(3000);
    return 0;
} // main()
