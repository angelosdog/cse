//
// Simple 2D graphics API.
//
// Outputs SVG to a file.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced August 2008.
//
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "graphics.h"
#include "util.h"

struct graphics {
  FILE *outf;
  char *filename;
  char *sourcefile;
};

graphics_t *graphics_init(char *sourcefile, char *filename)
{
  graphics_t *g = (graphics_t *)util_malloc(sizeof(graphics_t));

  g->sourcefile = sourcefile;  
  g->filename = filename;
  g->outf = fopen(filename, "w");

  // The SVG boilerplate. This is not ideal, but I'm not sure what is.
  fprintf(g->outf,
          "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>\n\
<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.0//EN\" \"http://www.w3.org/TR/SVG/DTD/svg10.dtd\">\n");

  return g;
}

void graphics_end(graphics_t **g)
{
  fclose((*g)->outf);

  util_free(*g);
  *g = NULL;
}

// As SVG's origin is in the top-left, we invert the Y coordinate with a transform matrix.
void graphics_picture_start(graphics_t *g, int width, int height)
{
  fprintf(g->outf,
          "<!-- Rendered by FLANGE from %s -->\n\
<svg\n\
   xmlns:svg=\"http://www.w3.org/2000/svg\"\n\
   xmlns=\"http://www.w3.org/2000/svg\"\n\
   version=\"1.0\"\n\
   viewBox=\"0 0 %d %d\"\n\
   width=\"100%%\"\n\
   height=\"100%%\">\n\
  <g stroke-width=\"1\" stroke=\"green\" transform=\"matrix(1 0 0 -1 0 %d)\">\n",
          g->sourcefile, width, height, height);
}

void graphics_picture_end(graphics_t *g)
{
  fprintf(g->outf, "  </g>\n</svg>\n");
}

void graphics_line(graphics_t *g, vector_t start, vector_t end)
{
  fprintf(g->outf,
          "    <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" />\n",
          (int)start.x, (int)start.y, (int)end.x, (int)end.y);
}

void graphics_ellipse(graphics_t *g,
                      vector_t centre,
                      double xradius, double yradius,
                      vector_t orientation)
{
  int angle = (180 * atan2(orientation.y, orientation.x)) / 3.14159; //M_PI;

  if ((int)xradius == (int)yradius) {
    fprintf(g->outf,
            "    <circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"none\" />\n",
            (int)centre.x, (int)centre.y, (int)xradius);
  } else if (angle == 0) {
    fprintf(g->outf,
            "    <ellipse cx=\"%d\" cy=\"%d\" rx=\"%d\" ry=\"%d\" fill=\"none\" />\n",
            (int)centre.x, (int)centre.y, (int)xradius, (int)yradius);
  } else {
    fprintf(g->outf,
            "    <g transform=\"translate(%d, %d)\">\n\
      <ellipse rx=\"%d\" ry=\"%d\" fill=\"none\" transform=\"rotate(%d)\"/>\n\
    </g>\n",
            (int)centre.x, (int)centre.y, (int)xradius, (int)yradius, angle);
  }
}
