#include "SDL.h"

using std::string;

InitError::InitError() : exception(), msg(SDL_GetError()) 
{ 
} // InitError()

InitError::InitError(const string& msg) : exception(), msg(msg)
{ 
} // InitError()

InitError::~InitError() throw() 
{ 
} // ~InitError()

const char* InitError::what() const throw() 
{ 
    return msg.c_str(); 
} // what()

SDL::SDL(Uint32 flags) throw(InitError) 
{ 
    if(SDL_Init(flags) != 0)
        throw InitError();
} // SDL()

SDL::~SDL() 
{ 
    SDL_Quit(); 
} // ~SDL()

void SDL::log_error(std::ostream& os, const string& msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
} // log_error()

char SDL::wait_for_key()
{
    while(true) {
        SDL_Event e;
        SDL_PollEvent(&e);
        if(e.type == SDL_KEYDOWN) {
            return e.key.keysym.sym;
        }
    }
} // wait_for_key()
