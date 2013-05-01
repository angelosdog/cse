//
// Simple 2D graphics API.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced August 2008.
//
//

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

typedef struct graphics graphics_t;

typedef struct {
  double x, y;
} vector_t;

// Create and destroy a graphics context: open the output file.
graphics_t *graphics_init(char *sourcefile, char *filename);
void graphics_end(graphics_t **g);

// Start and finish drawing a picture.
void graphics_picture_start(graphics_t *g, int width, int height);
void graphics_picture_end(graphics_t *g);

// Drawing primitives.

// Draw a line between two points.
void graphics_line(graphics_t *g, vector_t start, vector_t end);

// Draw an ellipse with the given centre and radii. (For a circle,
// make the radii equal.) Then rotate it to align with
// 'orientation'.
void graphics_ellipse(graphics_t *g,
                      vector_t centre,
                      double xradius, double yradius,
                      vector_t orientation);

#endif // _GRAPHICS_H_
