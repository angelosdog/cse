//
// FLANGE, the FLAN graphics engine.
//
// Peter Gammie, peteg@cse.unsw.edu.au
// Commenced August 2008.
//
//

#ifndef _FLANGE_H_
#define _FLANGE_H_

#include "graphics.h"
#include "parser.h"

// Render the binding for "main" in the given program.
void flange_render(int type_check, graphics_t *g, program_t *program,
                   value_t *(*evaluate_program)(env_t *env, program_t *program));

#endif // _FLANGE_H_
