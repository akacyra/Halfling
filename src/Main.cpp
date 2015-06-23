#include "Console.h"
#include <cctype>

int main(void)
{
    Console con("Console", 80, 24);
    con.load_font("terminal_6x12.png", 6, 12);
    //con.load_font("terminal_12x12.png", 12, 12);
    con.clear().flush();
    bool done = false;
    //unsigned int col = 1;
    while(!done) {
        SDL_Event e;
        bool redraw = false;
        int keycode;
        char ch;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_WINDOWEVENT:
                    if(e.window.event == SDL_WINDOWEVENT_CLOSE) {
                        done = true;
                    }
                    break;
                case SDL_KEYDOWN:
                    keycode = e.key.keysym.sym;
                    switch(keycode) {
                        case SDLK_RETURN:
                            break;
                        case SDLK_BACKSPACE:
                            break;
                    }
                    redraw = true;
                    break;
                case SDL_TEXTINPUT:
                    ch = e.text.text[0];
                    if(isprint(ch)) {
                        /*
                        if(col < 7) 
                            col++;
                        else 
                            col = 1;
                            */
                        con.put_char(ch);
                    }
                    redraw = true;
                    break;
            }
        }
        if(redraw) {
            con.flush();
            redraw = false;
        }
    }

    return 0;
} // main()
