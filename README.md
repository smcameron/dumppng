```
DUMPPNG(1)                       User Commands                      DUMPPNG(1)

NAME
       dumppng - dump PNG image files to C code

SYNOPSIS
       dumppng file.png [ ... ]

DESCRIPTION
       Generate  C  code from png image files for embedding images into C pro‐
       grams.

EXAMPLE
       $ ./dumppng star.png
       struct png_file_bytes {
            int w, h, a;
            unsigned char bytes[];
       };

       #pragma GCC diagnostic push
       #pragma GCC diagnostic ignored "-Wpedantic"
       static struct png_file_bytes star = {
            .w = 16,
            .h = 16,
            .a = 0,
            .bytes = {
                 0x15, 0x13, 0x09, 0x12, 0x11, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa2, 0x9a, 0x6a, 0x55, 0x50, 0x35, 0x00, 0x00, 0x00,

       (many lines omitted)

                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x2c, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x00,
            },
       };

       #pragma GCC diagnostic pop

NOTES
       This program is pretty rough and may not suit your needs at all.

AUTHOR
       Written by Stephen M. Cameron

dumppng                           MARCH 2025                        DUMPPNG(1)
```
