//
// FLANGE, the FLAN graphics engine.
//
// Based on: Peter Henderson, "Functional Geometry", Symposium on LISP
// and Functional Programming, 1982.
// http://doi.acm.org/10.1145/800068.802148
//
// A more recent account: Peter Henderson, "Functional Geometry",
// Higher-Order and Symbolic Computation 15(4): 349-365 (2002).
// http://dx.doi.org/10.1023/A:1022986521797
//
// Also: P. R. Eggert and K. P. Chow, "Logic programming graphics and
// infinite terms", Technical Report, Department of Computer Science,
// University of California, Santa Barbara, June 1983.
//
// Henderson's work implemented in Lisp, from which I scraped the
// concrete pictures:
//
//    http://www.frank-buss.de/lisp/functional.html
//
// There is also some information in Abelson and Sussman's "Structure
// and Interpretation of Computer Programs".
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced August 2008.
//
//

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "environment.h"
#include "errors.h"
#include "eval.h"
#include "flange.h"
#include "graphics.h"
#include "lexer.h"
#include "parser.h"
#include "symbols.h"
#include "types.h"
#include "values.h"

//**************************************
// Renderer state.

typedef struct {
  graphics_t *g;
  // Henderson's three vectors.
  vector_t origin; // a: Lower-left corner of the box.
  vector_t bottom; // b: Bottom of the picture.
  vector_t left;   // c: Left edge of the picture.
} render_t;


//**************************************
// Top-level: render a FLANGE picture.

static list_t *flange_types(void);
static env_t *flange_bindings(void);
static void f_render(render_t *r, value_t *v);

void flange_render(int type_check, graphics_t *g, program_t *program,
                   value_t *(*evaluate_program)(env_t *env, program_t *program))
{
  double width = 400, height = 400;
  list_t *fts = flange_types();
  env_t *env = flange_bindings();
  render_t r;

  // Append the FLANGE-specific types.
  list_append(&program->types, &fts);

  if (type_check && !types_check(program)) {
    fprintf(stdout, " ** Aborting on type error.\n");
    exit(1);
  }

  // Bottom-left coordinate is (0,0); top-right is (width,height).
  r.g = g;
  r.origin.x = 0;
  r.origin.y = 0;

  r.bottom.x = width;
  r.bottom.y = 0;

  r.left.x = 0;
  r.left.y = height;

  // Draw a picture.
  graphics_picture_start(g, width, height);
#if 1
  f_render(&r, evaluate_program(env, program));
#else
  print_value(stdout,evaluate_program(env, program));
  fprintf(stdout,"\n");
#endif
  graphics_picture_end(g);
}

//**************************************
// Vector arithmetic over vector_t.

static vector_t vec_0 = {0, 0};

static vector_t vec_add(vector_t a, vector_t b)
{
  vector_t vec_c = {a.x + b.x, a.y + b.y};
  return vec_c; // TODO: DONE
}

static vector_t vec_sub(vector_t a, vector_t b)
{
  vector_t vec_c = {a.x - b.x, a.y - b.y};
  return vec_c; // TODO: DONE
}

static vector_t vec_scale(vector_t a, double c)
{
  vector_t vec_c = {a.x*c, a.y*c};
  return vec_c; // TODO: DONE
}

static double vec_length(vector_t a)
{
  return sqrt(pow(a.x,2)+pow(a.y,2)); // TODO: DONE
}

static double vec_abs_cross_product(vector_t a, vector_t b)
{
  return abs(a.x*b.y - a.y*b.x); // TODO: DONE
}

//**************************************
// FLANGE drawing and geometry types and functions.
//
// Extend the standard builtin functions and types with
// FLANGE-specific ones. Use the parser so we don't have to build the
// data structures tediously by hand.
//
static char *flangeTypes =
  "data Shape = Circle (Num, Num) Num            \
              | Line (Num, Num) (Num, Num)       \
              | Rectangle (Num, Num) (Num, Num)  \
   data Picture                                  \
              = Above Num Num Picture Picture    \
              | Beside Num Num Picture Picture   \
              | Canvas Num Num [Shape]           \
              | Empty                            \
              | Flip Picture                     \
              | Overlay Picture Picture          \
              | Rotate Picture";
static list_t *flange_types(void)
{
  list_t *types;

  lex_string_start(flangeTypes);
  lex();
  types = typedefs();
  lex_string_end();

  return types;
}

static env_t *flange_bindings(void)
{
  return builtin_bindings();
}

//**************************************
// Render Canvas shapes.

typedef struct {
  render_t *r;
  double width, height;
} canvas_closure_t;

static void render_circle(canvas_closure_t *c, vector_t centre, double radius)
{
  render_t *r = c->r;
  // transformed origin and radius using render vectors
  vector_t newOrigin;
  double newxRadius, newyRadius;

  newOrigin = vec_add(
                  vec_add(r->origin, vec_scale(r->bottom, centre.x / c->width)),
                  vec_scale(r->left, centre.y / c->height));
  newxRadius = vec_length(vec_scale(r->bottom, radius / c->width));
  newyRadius = vec_length(vec_scale(r->left,   radius / c->height));

  graphics_ellipse(r->g, newOrigin, newxRadius, newyRadius, r->bottom);
}

static void render_line(canvas_closure_t *c, vector_t j, vector_t k)
{
  render_t *r = c->r;
  // transformed origin and radius using render vectors
  vector_t newStart, newEnd;

  newStart = vec_add(
                  vec_add(r->origin, vec_scale(r->bottom, j.x / c->width)),
                  vec_scale(r->left, j.y / c->height));
  newEnd = vec_add(vec_add(r->origin, vec_scale(r->bottom, k.x / c->width)),
                	vec_scale(r->left, k.y / c->height));

  graphics_line(r->g, newStart, newEnd);
}

static void render_rectangle(canvas_closure_t *c, vector_t j, vector_t k){
  vector_t corner1 = {j.x, j.y};
  vector_t corner2 = {j.x, k.y};
  vector_t corner3 = {k.x, j.y};
  vector_t corner4 = {k.x, k.y};
  render_line(c, corner1, corner2);
  render_line(c, corner1, corner3);
  render_line(c, corner2, corner4);
  render_line(c, corner3, corner4);
}

static int render_shape(canvas_closure_t *c, value_t *shape)
{
  switch (shape->type) {
  default:
  case v_unused:
    if (*(int *)NULL) { printf("should crash.\n"); }
    break;
  case v_datacons:
    if (strcmp(datacons_tag(shape), "Circle") == 0) {

      double center_x = num_val((value_t*)list_nth(tuple_val((value_t*)list_nth(datacons_params(shape),0)),0));
      double center_y = num_val((value_t*)list_nth(tuple_val((value_t*)list_nth(datacons_params(shape),0)),1));
      double radius = num_val((value_t *)list_nth(datacons_params(shape), 1));
      vector_t center = {center_x,center_y};
      // TODO: DONE!

      render_circle(c, center, radius);
    }
    else if (strcmp(datacons_tag(shape), "Line") == 0) {
      double start_x = num_val((value_t*)list_nth(tuple_val((value_t*)list_nth(datacons_params(shape),0)),0));
      double start_y = num_val((value_t*)list_nth(tuple_val((value_t*)list_nth(datacons_params(shape),0)),1));
      double end_x = num_val((value_t*)list_nth(tuple_val((value_t*)list_nth(datacons_params(shape),1)),0));
      double end_y = num_val((value_t*)list_nth(tuple_val((value_t*)list_nth(datacons_params(shape),1)),1));
      vector_t start = {start_x, start_y};
      vector_t end = {end_x,end_y};
      // TODO: DONE!
      render_line(c, start, end);

    }
    else if (strcmp(datacons_tag(shape), "Rectangle") == 0) {
      double start_x = num_val((value_t*)list_nth(tuple_val((value_t*)list_nth(datacons_params(shape),0)),0));
      double start_y = num_val((value_t*)list_nth(tuple_val((value_t*)list_nth(datacons_params(shape),0)),1));
      double end_x = num_val((value_t*)list_nth(tuple_val((value_t*)list_nth(datacons_params(shape),1)),0));
      double end_y = num_val((value_t*)list_nth(tuple_val((value_t*)list_nth(datacons_params(shape),1)),1));
      vector_t start = {start_x, start_y};
      vector_t end = {end_x,end_y};
      // TODO: DONE!
      render_rectangle(c, start, end);
    } else {
      printf("render_shape: unknown shape.\n");
      print_value(stdout, shape);
      error("");
    }
    break;
  }
  return 1;
}

//**************************************
// The recursive renderer.

static void f_render(render_t *r, value_t *v);

static void f_empty(render_t *r, value_t *v)
{
  // does nothing (intentionally left blank)
}

static void f_canvas(render_t *r, value_t *v)
{
  // Canvas width height shapes
  // place all shapes from list into width x height window

  list_t *params = datacons_params(v);
  double width  = num_val((value_t *)list_nth(params, 0));
  double height = num_val((value_t *)list_nth(params, 1));
  value_t *shapes = (value_t *)list_nth(params, 2);
  canvas_closure_t c;

  c.r = r;
  c.width = width;
  c.height = height;

  while (shapes->type == v_datacons
        && strcmp(datacons_tag(shapes), listConsTag) == 0) {
    render_shape(&c, list_nth(datacons_params(shapes), 0));
    shapes = (value_t *)list_nth(datacons_params(shapes), 1);
  }
}

static void f_above(render_t *r, value_t *v)
{

  // Above n m p q
  // places Picture p above Picture q
  // p occupies n/(n+m) of window height
  // q occupies m/(n+m) of window height
  list_t *params = datacons_params(v);
  double ratio1  = num_val((value_t *)list_nth(params, 0));
  double ratio2 = num_val((value_t *)list_nth(params, 1));
  value_t *shapeP = (value_t *)list_nth(params, 2);
  value_t *shapeQ = (value_t *)list_nth(params, 3);

  render_t rP;
  render_t rQ;
  //Make a copy of r
  rP.g = r->g;
  rP.origin = r->origin;
  rP.bottom = r->bottom;
  rP.left = r->left;

  rQ.g = r->g;
  rQ.origin = r->origin;
  rQ.bottom = r->bottom;
  rQ.left = r->left;

  rQ.left = vec_scale(rQ.left, ratio2/(ratio1+ratio2));
  rP.origin = vec_add(rQ.origin, rQ.left);
  rP.left = vec_sub(rP.left, rQ.left);

  f_render(&rP, shapeP);
  f_render(&rQ, shapeQ);
  // TODO: DONE
}

static void f_beside(render_t *r, value_t *v)
{
  // Beside n m p q
  // places Picture p beside Picture q
  // p occupies n/(n+m) of window width
  // q occupies m/(n+m) of window width
  list_t *params = datacons_params(v);
  double ratio1  = num_val((value_t *)list_nth(params, 0));
  double ratio2 = num_val((value_t *)list_nth(params, 1));
  value_t *shapeP = (value_t *)list_nth(params, 2);
  value_t *shapeQ = (value_t *)list_nth(params, 3);

  render_t rP;
  render_t rQ;
  //Make a copy of r
  rP.g = r->g;
  rP.origin = r->origin;
  rP.bottom = r->bottom;
  rP.left = r->left;

  rQ.g = r->g;
  rQ.origin = r->origin;
  rQ.bottom = r->bottom;
  rQ.left = r->left;

  rP.bottom = vec_scale(rP.bottom, ratio1/(ratio1+ratio2));
  rQ.origin = vec_add(rP.origin, rP.bottom);
  rQ.bottom = vec_sub(rQ.bottom, rP.bottom);

  f_render(&rP, shapeP);
  f_render(&rQ, shapeQ);

  // TODO: DONE
}

static void f_flip(render_t *r, value_t *v)
{
  // Flip p
  // gives a reflection of p in a vertical axis 
  // exactly bisecting the canvas

  list_t *params = datacons_params(v);
  value_t *shape = (value_t *)list_nth(params, 0);

  render_t rP;
  //Make a copy of r
  rP.g = r->g;
  rP.origin = r->origin;
  rP.bottom = r->bottom;
  rP.left = r->left;

  rP.origin = vec_add(rP.bottom,rP.origin);
  rP.bottom = vec_scale(rP.bottom, -1);

  f_render(&rP, shape);


  // TODO: DONE
}

static void f_overlay(render_t *r, value_t *v)
{
  // Overlay p q
  // picture containing all elements of p and q

  list_t *params = datacons_params(v);
  value_t *shapeP = (value_t *)list_nth(params, 0);
  value_t *shapeQ = (value_t *)list_nth(params, 1);

  f_render(r, shapeP);
  f_render(r, shapeQ);

  // TODO: DONE!
}

static void f_rotate(render_t *r, value_t *v)
{
  // Rotate p
  // rotate p anticlockwise by 90 degree

  list_t *params = datacons_params(v);
  value_t *shape = (value_t *)list_nth(params, 0);

  render_t rP;
  //Make a copy of r
  rP.g = r->g;
  rP.origin = r->origin;
  rP.bottom = r->bottom;
  rP.left = r->left;

  vector_t originalLeft = rP.left;
  rP.origin = vec_add(rP.bottom,rP.origin);
  rP.left = vec_scale(rP.bottom,-1);
  rP.bottom = originalLeft;

  f_render(&rP, shape);


  // TODO: DONE!
}

static void f_render(render_t *r, value_t *v)
{
  switch (v->type) {
  default:
  case v_unused:
    error("flange: Expected 'main' to be bound to an expression of type 'Picture'.\n");
    break;
  case v_datacons:
    if (strcmp(datacons_tag(v), "Above") == 0) {
      f_above(r, v);
    } else if (strcmp(datacons_tag(v), "Beside") == 0) {
      f_beside(r, v);
    } else if (strcmp(datacons_tag(v), "Canvas") == 0) {
      f_canvas(r, v);
    } else if (strcmp(datacons_tag(v), "Empty") == 0) {
      f_empty(r, v);
    } else if (strcmp(datacons_tag(v), "Flip") == 0) {
      f_flip(r, v);
    } else if (strcmp(datacons_tag(v), "Overlay") == 0) {
      f_overlay(r, v);
    } else if (strcmp(datacons_tag(v), "Rotate") == 0) {
      f_rotate(r, v);
    } else {
      error("flange: Unknown data constructor: '%s'.\n", datacons_tag(v));
    }
    break;
  }
}
