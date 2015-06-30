# Halfling

Halfling is a library that lets you build  text-based user interfaces using two fonts, a full-width font and a half-width font, on the same emulated terminal screen.

### Features

    - Stackable Layers than can be added or removed as needed
    - Can mix full-width and half-width fonts
    - Powered by SDL

### Example

```
#include "/path/to/Console.h"
...
Console con("Example", 0, 0, 80, 24);
Layer screen(&con, Layer::HalfWidth, 0, 0, 80, 24);
con.add_layer(&screen);
screen.put_str(40, 12, "Hello, world!");
con.refresh();
```