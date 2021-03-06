```
  ___ _  _____ _  _____ ___  _  _  
 | __| ||_   _| |/ / __/ _ \| \| | 
 | _|| |__| | | ' < (_| (_) | .` | 
 |_| |____|_| |_|\_\___\___/|_|\_| 
```

# Document License

    Copyright (C) 2018 The fltkcon authors

    Permission is granted to copy, distribute and/or modify this document
    under the terms of the GNU Free Documentation License, Version 1.3
    or any later version published by the Free Software Foundation;
    with no Invariant Sections, no Front-Cover Texts, and no Back-Cover Texts.
    A copy of the license is included in the section entitled "GNU
    Free Documentation License"

# License

The library is licensed under the FLTK License.  The FLTK License is the
GNU Library General Public License (LGPL) with exceptions.  Incidentally,
a small minority of source modules included in the library are Public
Domain, which is compatible with GNU LGPL.

# fltkcon

fltkcon is a port of the Fast Light Toolkit (FLTK) to terminal consoles.

+ 32-bit DOS Protected Mode
+ 32/64-bit Microsoft Windows (NT)
+ 32/64-bit GNU/Linux (ncurses)
+ 32/64-bit BSD variants (ncurses)

# Screen Shots

![valuator](/htmimg/valuator.png)


![menubar](/htmimg/menubar.png)

# Hello World example

```C++
#include "widget.h"
#include "win.h"
#include "btn.h"

static void
quit_cb(Fl_Widget*, void* userdata)
{
  Fl_Window* window = reinterpret_cast<Fl_Window*>(userdata);
  window->hide();
  return;
}

int
main(int argc, char** argv)
{

  Fl_Window window(1, 1, 20, 5,
                   reinterpret_cast<unsigned char const*>("Hello Example"));

  Fl_Button* btn = new Fl_Button(2, 2, 15, 1,
                                 reinterpret_cast<unsigned char const*>("Hello"));

  btn->when((Fl_Widget::FL_WHEN_CHANGED |
             Fl_Widget::FL_WHEN_ENTER_KEY_ALWAYS |
             Fl_Widget::FL_WHEN_RELEASE));

  btn->callback(quit_cb, &window);

  window.end();
  window.show(argc, argv);

  return Fl::run();
}
```

# Links

Fast Light Toolkit
[FLTK](http://www.fltk.org/)

DOS FLTK port
[FTLKAL](https://github.com/markjolesen/fltkal)

A cross platform C++ TUI inspired by FLTK
[FDOSTUI](http://www.nongnu.org/fdostui/)


