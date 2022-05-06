#include <stdio.h>
#include <stdlib.h>
#include "subway.h"


int main (int argc, char **argv)
{
        if (argc != 3) {
                printf("Error there should be exactly two arguments, got %d\n", argc-1);
                exit(1);
        }

        // Init
        cairo_objects_t co = init_cairo();
        subway_t subway = {NULL, NULL, NULL, NULL, 0, 0, 0, 0};

        // Read input and render the image
        read_input(co.cr, &subway, argv[1]);
        render_subway(co.cr, &subway);
        free_subway(&subway);

        // Save the image to the disk
        cairo_destroy(co.cr);
        cairo_surface_write_to_png(co.surface, argv[2]);
        cairo_surface_destroy(co.surface);
        cairo_debug_reset_static_data();
        return 0;
}
