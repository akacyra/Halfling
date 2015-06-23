#ifndef __SDL_H__
#define __SDL_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <exception>
#include <iostream>

class InitError : public std::exception
{
    public:
        InitError();
        InitError(const std::string& msg);
        virtual ~InitError() throw();
        virtual const char* what() const throw();
    private:
        std::string msg;
};

class SDL
{
    public:
        SDL(Uint32 flags = 0) throw(InitError);
        virtual ~SDL();
        void log_error(std::ostream& os, const std::string& msg);
};

#endif 
