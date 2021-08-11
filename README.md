# gimp-jpegxl-plugin
Plug-in for development GIMP 2.99 for loading/saving JPEG XL (JXL) images. 

## How to install

Ensure you have [libjxl](https://github.com/libjxl/libjxl) installed.

Run: `gimptool-2.99 --just-print --install file-jpegxl.c`

gimptool-2.99 will show you command line to build and install the plug-in. You’ll have to copy the command and to add at the end following parameters: **-ljxl -ljxl_threads**

Complete command may look following way (example):

_x86_64-pc-linux-gnu-gcc  -I/usr/include/gimp-3.0 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/lib64/libffi/include -I/usr/include/libpng16 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/harfbuzz -I/usr/include/uuid -I/usr/include/gegl-0.4 -I/usr/include/gio-unix-2.0 -I/usr/include/json-glib-1.0 -I/usr/include/babl-0.1 -I/usr/include/gtk-3.0 -I/usr/include/pango-1.0 -I/usr/include/fribidi -I/usr/include/atk-1.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/dbus-1.0 -I/usr/lib64/dbus-1.0/include -I/usr/include/at-spi-2.0 -pthread  -o '/home/**daniel**/.config/GIMP/2.99/plug-ins/file-jpegxl/file-jpegxl' 'file-jpegxl.c'  -lgimpui-3.0 -lgimpwidgets-3.0 -lgimpmodule-3.0 -lgimp-3.0 -lgimpmath-3.0 -lgimpconfig-3.0 -lgimpcolor-3.0 -lgimpbase-3.0 -lgegl-0.4 -lgegl-npd-0.4 -lm -Wl,--export-dynamic -lgmodule-2.0 -pthread -lglib-2.0 -ljson-glib-1.0 -lbabl-0.1 -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -lharfbuzz -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0 **-ljxl -ljxl_threads** -Og -ggdb3 -Wall_
