<img src=Halfling.png width=96 height=120>

# Halfling

Halfling is a library that lets you build  text-based user interfaces using two fonts, a full-width font and a half-width font, on the same emulated terminal screen.

### Features

- Stackable Layers than can be added or removed as needed
- Can mix full-width and half-width fonts
- Powered by SDL

### Installation

```Shell
./autogen.sh
./configure
make
make install
```

To compile examples:
```Shell
cd examples
make 
```

### Example

```C++
#include <halfling/Console.h>

...

Console con("Example", 0, 0, 80, 24);
Layer screen(Layer::HalfWidth, 0, 0, 80, 24);
con.add_layer(&screen);
screen.put_str(40, 12, "Hello, world!");
con.refresh();
```
Link with `-lhalfling`.
