#ifndef __CONSOLE_H__
#define __CONSOLE_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "SDL.h"
#include "Layer.h"


class Console 
{
    public:
    /*
     * Console creation.
     */
        /*
         * Construct and initialize a console window.
         * @title: console window title.
         * @x,y: coordinates (in pixels) of the window's top-left corner.
         * @w,h: width and height (in half-width cells) of the window.
         */
        Console(std::string title = "", int x = 0, int y = 0, 
                int w = 80, int h = 24); 
        ~Console();

    /*
     * Layer stack operations. Changes will not be visible until the
     * console is refreshed. Layers can be occluded by other Layers that
     * are higher in the stack than them.
     */
        // Adds the Layer to the top of the stack.
        void add_layer(Layer* layer);
        //  Removes the Layer from where it is in the stack.
        void remove_layer(Layer* layer);
        // Moves the Layer to the top of the stack.
        void bring_layer_to_front(Layer* layer);
        // Moves the Layer to the bottom of the stack.
        void send_layer_to_back(Layer* layer);
        // Removes all Layers from the stack.
        void remove_all_layers();

    /*
     * Console control operations.
     */
        // Clears all Layers in the stack to their default colors.
        void clear();
        // Renders all Layers in the stack.
        void refresh();

    /*
     * Console settings operations.
     */
        // Returns the size of the Console.
        Size get_size() const;
        /*
         * Defines a color for the Console.
         * @id: id to associate with the color.
         * @r,g,b: rgb content of the color.
         */
        void define_color(Color id, int r, int g, int b);

    private:
        const int FONTMAP_NUM_X = 16;
        const int FONTMAP_NUM_Y = 16;

        typedef std::unordered_map< Color, RGB > ColorMap;
        ColorMap colors;

        Size size, font_size_full;


        std::vector< Layer* > layers;

        SDL sdl;
        SDL_Window* win;
        SDL_Renderer* ren;
        SDL_Texture* font_full;
        SDL_Texture* font_half;

        void init_color_map(ColorMap& colormap);
        RGB get_color_rgb(Color color) const;
        void load_font(std::string fname_full_width, 
                       std::string fname_half_width, 
                       int size_x, int size_y);
};

#endif
