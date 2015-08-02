#ifndef __HALFLING_LAYER_H__
#define __HALFLING_LAYER_H__

#include <vector>
#include <string>
#include <boost/optional.hpp>
#include "Util.h"
#include "Color.h"

class Console;

class Layer
{
    friend Console;

    public:
    /*
     * Width of the font to use, which determines cell size.
     * Example: 
     *    if full-width font is 12x12, then half-width would be 6x12
     */
        enum Width {
            HalfWidth,
            FullWidth
        };

        struct Cell {
            char ch;
            Color fg, bg;
        };

    /* 
     * Layer creation.
     */
        /*
         * Construct and initialize layer.
         * @cell_width: width of the font to use.
         * @x,y: coordinates (in cells) of the top-left corner of the layer.
         * @w,h: width (in cells) of the Layer.
         *
         * The Console's coordinate system is based off the smallest unit,
         *  half-width cells. So (x,y) for Layers should always be in half-
         *  width cells. (w,h) are in the Layers own cell width.
         */
        Layer(Width cell_width, int x, int y, int w, int h);

    /*
     * Layer control operations.
     */
        // Clears the Layer using default foreground and background colors
        void clear();
        // Clears sublayers using default foreground and background colors
        void clear_sublayers();
        // Copies the Layer and its sublayers to the Console backbuffer.
        void touch();
        // Adds ch to the Layer at position (x,y) using default colors.
        void put_char(int x, int y, char ch);
        // Adds ch to the Layer at position (x,y) with specified colors.
        void put_char(int x, int y, char ch, Color fg, Color bg);
        // Adds str to the Layer starting at position (x,y) using default colors.
        void put_str(int x, int y, std::string str);
        // Adds str to the Layer starting at position (x,y) with specified colors.
        void put_str(int x, int y, std::string str, Color fg, Color bg);
        // Returns the char, fg color, and bg color of the cell at position (x,y).
        boost::optional< Cell > operator()(int x, int y) const;

    /*
     * Sublayer stack operations. Changes will not be visible until the
     * layer is refreshed. Sublayers can be occluded by other Layers that
     * are higher in the stack than them.
     */
        // Adds the sublayer to the top of the stack.
        void add_sublayer(Layer* layer);
        //  Removes the sublayer from where it is in the stack.
        void remove_sublayer(Layer* layer);
        // Removes all sublayers from the stack.
        void remove_all_sublayers();

        // Moves the layer to the top of its parent's layer stack.
        void bring_to_front();
        // Moves the layer to the bottom of its parent's layer stack.
        void send_to_back();


    /*
     * Layer settings operations.
     */
        // Returns the cell width of the Layer.
        Width cell_width() const;
        // Sets the default foreground color.
        void set_fg_color(Color id);
        // Sets the default background color.
        void set_bg_color(Color id);
        // Returns the default foreground color id.
        Color get_fg_color() const;
        // Returns the default background color id.
        Color get_bg_color() const;
        // Sets the origin (top-left corner) of the Layer.
        void set_origin(int x, int y);
        // Returns the bounds of the Layer.
        Rect get_bounds() const;

    private:
        Layer* parent;
        Console* console;

        std::vector< Layer* > sublayers;

        typedef std::vector< std::vector < Cell > > Buffer;
        Buffer frontbuffer, backbuffer;

        Rect bounds;
        Point abs_origin;
        Width cell_w;

        Color default_fg, default_bg;

        bool contains(int x, int y) const;
        void sublayer_to_front(Layer* layer);
        void sublayer_to_back(Layer* layer);
};

#endif
